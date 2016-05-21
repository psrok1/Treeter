#include "group.h"

namespace Model
{
    bool Group::validateName(std::string name)
    {

    }

    Group::Group(std::string name, Group* parent)
    {

    }

    std::string Group::getName() const
    {
        return this->name;
    }

    void Group::sendNotification(MessageBase::Reference msg)
    {

    }

    std::shared_ptr<Group> Group::createGroup(std::string name)
    {

    }

    bool Group::deleteGroup(std::string name)
    {

    }

    std::list<std::string> Group::listGroupNames() const
    {

    }

    std::shared_ptr<Group> Group::getGroupByName(std::string name) const
    {

    }

    void Group::registerConnection(Connection::Reference connection)
    {

    }

    void Group::unregisterConnection(Connection::Reference connection)
    {

    }

    bool Group::addMember(std::shared_ptr<User> user, MemberRole memberRole)
    {

    }

    bool Group::deleteMember(std::string memberLogin)
    {

    }

    Group::MemberRole Group::getMemberPermission(std::string memberLogin) const
    {

    }

    void Group::setMemberPermission(std::string memberLogin, MemberRole memberRole)
    {

    }

    std::list<std::string> Group::listOfPendingApprovals() const
    {

    }

    std::list<std::pair<std::string, Group::MemberRole>> Group::listOfMembers() const
    {

    }

    void Group::sendMessage(GroupMessage message)
    {

    }

    std::list<GroupMessage> Group::getMessages(GroupMessage::Timestamp fromPoint) const
    {

    }

    std::list<GroupMessage> Group::getMessages() const
    {

    }
}
