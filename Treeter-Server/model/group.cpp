#include "group.h"
#include "mapgetter.h"
#include <utility>
#include <algorithm>

namespace Model
{
    bool Group::validateName(std::string name)
    {
        // @TODO
        (void)name;
        return true;
    }

    Group::Group(std::string name, Group* parent):
        name(name), parent(parent) { }

    std::string Group::getName() const
    {
        return this->name;
    }

    void Group::sendNotification(MessageOutgoing::Reference msg)
    {
        this->connections.sendToAll(msg);
    }

    std::shared_ptr<Group> Group::createGroup(std::string name)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        if(this->children.find(name) != this->children.end())
            return nullptr;

        return this->children[name] = std::shared_ptr<Group>(new Group(name, this));
    }

    // Shouldn't be called recursive from the same instance
    bool Group::deleteGroup(std::string name)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        // Find group iterator
        auto it = this->children.find(name);

        if(it == this->children.end())
            return false;

        Group& group = *(it->second);

        /**
         * WARNING!!!! POSSIBLE DEADLOCK HERE?
         * Probably no, because locks will be still hierarchical (group hierarchy)
         * But.. it should be checked
         **/
        group.clean();

        // Erase from subgroup list
        this->children.erase(it);
        return true;
    }

    void Group::clean()
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        for(std::string group_name: this->listGroupNames())
            this->deleteGroup(group_name);

        for(std::string member_name: getKeys(this->members))
            this->deleteMember(member_name);
    }

    std::list<std::string> Group::listGroupNames() const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        return getKeys(this->children);
    }

    std::shared_ptr<Group> Group::getGroupByName(std::string name) const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        auto it = this->children.find(name);

        if(it == this->children.end())
            return nullptr;

        return (*it).second;
    }

    void Group::registerConnection(Connection::Reference connection)
    {
        // ConnectionList object is internally synchronized: there is no need to lock inside critical section
        this->connections.insert(connection);
    }

    void Group::unregisterConnection(Connection::Reference connection)
    {
        // ConnectionList object is internally synchronized: there is no need to lock inside critical section
        this->connections.remove(connection);
    }

    bool Group::addMember(std::shared_ptr<Group> group_ref, std::shared_ptr<User> user, MemberRole memberRole)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        std::string name = user->getLogin();

        if(this->members.find(name) != this->members.end())
            return false;

        // TODO: Addition
        (void)group_ref;
        (void)memberRole;
        return true;
    }

    bool Group::deleteMember(std::string memberLogin)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        auto it = this->members.find(memberLogin);

        if(it == this->members.end())
            return false;

        // TODO: Deletion
        return true;
    }

    Group::MemberRole Group::getMemberPermission(std::string memberLogin) const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        auto it = this->members.find(memberLogin);

        if(it == this->members.end())
            return MemberRole::NotAMember;

        const Member& member = (*it).second;

        return member.role;
    }

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

    std::list<std::string> Group::listOfPendingApprovals() const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        std::list<std::string> memberList = getKeys(this->members);
        std::remove_if(memberList.begin(), memberList.end(), [this](const std::string& memberLogin) -> bool {
            return this->members.at(memberLogin).role != MemberRole::PendingApproval;
        });

        return memberList;
    }

    std::list<std::pair<std::string, Group::MemberRole>> Group::listOfMembers() const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        std::list<std::pair<std::string, Group::MemberRole>> result;

        for(const std::pair<std::string, Member>& m: this->members)
            result.push_back(std::make_pair(m.first, m.second.role));

        return result;
    }

    void Group::sendMessage(GroupMessage message)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        // @TODO
        (void)message;
    }

    std::list<GroupMessage> Group::getMessages(GroupMessage::Timestamp fromPoint) const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        // @TODO
        (void)fromPoint;
        return std::list<GroupMessage>();
    }

    std::list<GroupMessage> Group::getMessages() const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        // @TODO
        return std::list<GroupMessage>();
    }
}
