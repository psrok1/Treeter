#include "group.h"
#include "datamodel.h"
#include "mapgetter.h"
#include "message/messageoutgoing.h"
#include <utility>
#include <algorithm>

namespace Model
{
    /**
     * @brief Group::validateName
     * Checks, whether name contains only allowed chars.
     */
    bool Group::validateName(std::string name)
    {
        return std::all_of(name.begin(), name.end(), [](const char c) {
           return std::isalnum(c) || (c == '_') || (c == ' ');
        });
    }

    std::pair<std::string, std::string> Group::splitParentPath(std::string path)
    {
        size_t slash_pos = path.find_last_of('/');
        return std::make_pair(path.substr(0, slash_pos), path.substr(slash_pos+1));
    }

    Group::Group(std::string name, Group* parent):
        invalidated(false), name(name),
        absolutePath((parent != nullptr ? parent->absolutePath : "")+"/"+name), parent(parent) { }

    /**
     * @brief Group::getName
     * Group name getter
     * Property is immutable, so critical section is unnecessary.
     * @NEEDS-THREAD-SAFETY-CHECK
     */
    std::string Group::getGroupName() const
    {
        return this->name;
    }

    /**
     * @brief Group::getAbsolutePath
     * Group absolute path getter
     * Property is immutable, so critical section is unnecessary.
     * @NEEDS-THREAD-SAFETY-CHECK
     */
    std::string Group::getAbsolutePath() const
    {
        return this->absolutePath;
    }

    /**
     * @brief Group::sendNotification
     * Sends message to all associated connections
     */
    void Group::sendNotification(MessageOutgoing::Reference msg)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        for(Member& member: getValues(this->members))
        {
            std::shared_ptr<User> userRef = member.user.lock();
            // User could be invalidated
            if(!userRef)
                continue;

            // User might be not priviliged
            if(member.role == MemberRole::PendingApproval)
                continue;

            Connection::Reference connection = userRef->getConnection();
            if(connection)
                connection->sendMessage(msg);
        }
    }

    /**
     * @brief Group::createGroup
     * Creates new subgroup and returns its pointer.
     * If operation is forbidden because of object invalidation - method fails.
     * If group exists yet: also nullptr returned
     */
    std::shared_ptr<Group> Group::createGroup(std::string name, bool imported)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        if(invalidated)
            return nullptr;

        if(this->children.find(name) != this->children.end())
            return nullptr;
        //Update the DB, unless it's just an import operation
        if(!imported)
        {
            DB.insertGroup(name,this->absolutePath);
            // Send notification to all
            this->sendNotification(MessageOutgoing::Reference(new AddedSubgroupNotification(this->absolutePath, name)));
        }

        return this->children[name] = std::shared_ptr<Group>(new Group(name, this));
    }

    /**
     * @brief Group::deleteGroup
     * Deletes subgroup (and recursively whole subtree)
     * Returns false, if subgroup was deleted yet.
     */
    bool Group::deleteGroup(std::string name)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        // Find group iterator
        auto it = this->children.find(name);

        if(it == this->children.end())
            return false;

        Group& group = *(it->second);

        // Warning: hierarchical lock!
        group.clean();

        // Erase from subgroup list
        this->children.erase(it);

        // Delete from DB
        DB.deleteGroup(this->absolutePath+"/"+name);

        // Send notification to all
        this->sendNotification(MessageOutgoing::Reference(new RemovedSubgroupNotification(this->absolutePath, name)));

        return true;
    }

    /**
     * @brief Group::clean
     * Performs group deletion.
     * Invalidates instance (cancels pending "add" operations)
     * Recursively removes all subgroups and all members.
     */
    void Group::clean()
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        this->invalidate();

        for(std::string group_name: this->listGroupNames())
            this->deleteGroup(group_name);

        for(std::string member_name: getKeys(this->members))
            this->deleteMember(member_name);
    }

    /**
     * @brief Group::listGroupNames
     * Returns list of subgroup names
     */
    std::list<std::string> Group::listGroupNames() const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        return getKeys(this->children);
    }

    /**
     * @brief Group::getGroupByName
     * Gets subgroup pointer by name.
     * Returns nullptr, if group doesn't exist
     */
    std::shared_ptr<Group> Group::getGroupByName(std::string name) const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        auto it = this->children.find(name);

        if(it == this->children.end())
            return nullptr;

        return (*it).second;
    }

    /**
     * @brief Group::addMember
     * Adds member into group (also registers group in user object)
     * This method should fail, if one of three cases occures:
     * - group instance is invalidated
     * - member exists yet
     * - user instance is invalidated
     * If failure: method returns false
     *
     * First argument is smart "this" pointer, provided by parent subgroup or member's user instance
     */
    bool Group::addMember(std::shared_ptr<Group> group_ref, std::shared_ptr<User> user, MemberRole memberRole, bool imported)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        if(invalidated)
            return false;

        std::string login = user->getLogin();

        if(this->members.find(login) != this->members.end())
            return false;

        if(!user->addGroup(group_ref))
            return false;

        this->members[login] = Member(user, memberRole);

        //Update the DB, unless it's just an import operation
        if(!imported)
        {
            DB.insertMember(user->getLogin(),group_ref->getAbsolutePath(),static_cast<int> (memberRole));

            // Send notification to all
            this->sendNotification(MessageOutgoing::Reference(new AddedToGroupNotification(this->absolutePath, login, memberRole)));
        }

        return true;
    }

    /**
     * @brief Group::deleteMember
     * Deletes member from group (also unregisters group from user object)
     * This method fails, when member doesn't exist
     *
     * User is linked with group object by weak pointer, so user instance can be destroyed during member deletion.
     * This method tries to lock smart pointer: on failure, it ignores this fact and omits "unregistration" stage.
     */
    bool Group::deleteMember(std::string memberLogin)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        auto it = this->members.find(memberLogin);

        if(it == this->members.end())
            return false;

        std::shared_ptr<User> user_ref = (it->second).user.lock();

        if(user_ref)
            user_ref->removeGroup(this->absolutePath);

        this->members.erase(it);

        DB.deleteMember(memberLogin,this->absolutePath);

        // Send notification to all
        this->sendNotification(MessageOutgoing::Reference(new RemovedFromGroupNotification(this->absolutePath, memberLogin)));

        return true;
    }

    /**
     * @brief Group::getMemberPermission
     * MemberRole getter.
     * MemberRole::NotAMember is returned, when member doesn't exist
     */
    MemberRole Group::getMemberPermission(std::string memberLogin) const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        auto it = this->members.find(memberLogin);

        if(it == this->members.end())
            return MemberRole::NotAMember;

        const Member& member = (*it).second;

        return member.role;
    }

    /**
     * @brief Group::setMemberPermission
     * MemberRole setter.
     * If member doesn't exist: method returns false.
     */
    bool Group::setMemberPermission(std::string memberLogin, MemberRole memberRole)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        auto it = this->members.find(memberLogin);

        if(it == this->members.end())
            return false;

        Member& member = (*it).second;

        member.role = memberRole;
        DB.updateMemberRole(memberLogin,this->absolutePath,static_cast<int>(memberRole));

        // Send notification to all
        this->sendNotification(MessageOutgoing::Reference(new ModifiedMemberPermissionNotification(memberLogin, this->absolutePath, memberRole)));

        return true;
    }

    /**
     * @brief Group::hasModerator
     * Checks, whether group has at least one moderator
     */
    bool Group::hasModerator() const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        for(auto& m: this->members)
            if(m.second.role == MemberRole::Moderator)
                return true;
        return false;
    }


    /**
     * @brief Group::listOfPendingApprovals
     * Gets list of members, who wait for approval.
     */
    std::list<std::string> Group::listOfPendingApprovals() const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        std::list<std::string> memberList = getKeys(this->members);
        std::remove_if(memberList.begin(), memberList.end(), [this](const std::string& memberLogin) -> bool {
            return this->members.at(memberLogin).role != MemberRole::PendingApproval;
        });

        return memberList;
    }

    /**
     * @brief Group::listOfMembers
     * Gets full list of members with roles.
     */
    std::list<std::pair<std::string, MemberRole>> Group::listOfMembers() const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        std::list<std::pair<std::string, MemberRole>> result;

        for(const std::pair<std::string, Member>& m: this->members)
            result.push_back(std::make_pair(m.first, m.second.role));

        return result;
    }

    /**
     * @brief Group::sendMessage
     * Adds message to list of messages.
     * Sends notification to all users.
     */
    void Group::sendMessage(GroupMessage message)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        // @TODO: Adding msg to db
        this->messages.push_back(message);

        MessageOutgoing::Reference newMsgNotification(new NewMessageNotification(this->absolutePath, message));
        sendNotification(newMsgNotification);
    }

    /**
     * @brief Group::getMessages
     * Gets messages from specified point of time.
     */
    std::list<GroupMessage> Group::getMessages(GroupMessage::Timestamp fromPoint) const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        std::list<GroupMessage> messageList;

        std::copy_if(this->messages.begin(), this->messages.end(), std::back_inserter(messageList),
                     [this, &fromPoint](const GroupMessage& msg) -> bool {
            return msg.getTimestamp() >= fromPoint;
        });
        return messageList;
    }

    /**
     * @brief Group::getMessages
     * Gets list of all messages in group.
     */
    std::list<GroupMessage> Group::getMessages() const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        return this->messages;
    }

    void Group::importFromDatabase(DataModel& model)
    {
        ResultSet groupNames = DB.importChildren(this->absolutePath);

        for(auto& groupName: groupNames)
            this->createGroup(groupName[0]);

        for(auto& child: this->children)
        {
            const std::shared_ptr<Group>& groupPtr = child.second;

            std::list<std::pair<std::string, MemberRole>> groupMembers;
            ResultSet members = DB.importMembers(this->absolutePath);

            for(auto member: members)
                this->addMember(groupPtr, model.getUserByLogin(member[0]), static_cast<MemberRole>(atoi(member[1].data())),true);

            groupPtr->importFromDatabase(model);
        }
    }
}
