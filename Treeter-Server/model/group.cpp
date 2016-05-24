#include "group.h"
#include "mapgetter.h"
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

    Group::Group(std::string name, Group* parent):
        invalidated(false), name(name), parent(parent) { }

    /**
     * @brief Group::getName
     * Group name getter
     * Property is immutable, so critical section is unnecessary.
     * @NEEDS-THREAD-SAFETY-CHECK
     */
    std::string Group::getName() const
    {
        return this->name;
    }

    /**
     * @brief Group::sendNotification
     * Sends message to all associated connections
     * ConnectionList object is internally synchronized, so critical section is unnecessary
     */
    void Group::sendNotification(MessageOutgoing::Reference msg)
    {
        this->connections.sendToAll(msg);
    }

    /**
     * @brief Group::createGroup
     * Creates new subgroup and returns its pointer.
     * If operation is forbidden because of object invalidation - method fails.
     * If group exists yet: also nullptr returned
     */
    std::shared_ptr<Group> Group::createGroup(std::string name)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        if(invalidated)
            return nullptr;

        if(this->children.find(name) != this->children.end())
            return nullptr;

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
     * @brief Group::registerConnection
     * Adds new connection (observer) into group.
     * ConnectionList object is internally synchronized, so critical section is unnecessary
     */
    void Group::registerConnection(Connection::Reference connection)
    {
        this->connections.insert(connection);
    }

    /**
     * @brief Group::registerConnection
     * Removes connection (observer) from group.
     * ConnectionList object is internally synchronized, so critical section is unnecessary
     */
    void Group::unregisterConnection(Connection::Reference connection)
    {
        // ConnectionList object is internally synchronized: there is no need to lock inside critical section
        this->connections.remove(connection);
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
    bool Group::addMember(std::shared_ptr<Group> group_ref, std::shared_ptr<User> user, MemberRole memberRole)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        if(invalidated)
            return false;

        std::string name = user->getLogin();

        if(this->members.find(name) != this->members.end())
            return false;

        if(!user->addGroup(group_ref))
            return false;

        this->members[name] = Member(user, memberRole);
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
            user_ref->removeGroup(this->name);

        this->members.erase(it);

        return true;
    }

    /**
     * @brief Group::getMemberPermission
     * MemberRole getter.
     * MemberRole::NotAMember is returned, when member doesn't exist
     */
    Group::MemberRole Group::getMemberPermission(std::string memberLogin) const
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
    std::list<std::pair<std::string, Group::MemberRole>> Group::listOfMembers() const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        std::list<std::pair<std::string, Group::MemberRole>> result;

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
        // @TODO
        (void)message;
        // sendNotification
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
}
