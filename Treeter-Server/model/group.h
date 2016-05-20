#ifndef GROUP_H
#define GROUP_H

#include <string>
#include <memory>
#include <unordered_map>
#include <list>
#include <mutex>

#include "../connectionlist.h"
#include "groupmessage.h"
#include "../message/messagebase.h"

namespace Model
{
    class User;

    struct Group
    {
        enum class MemberRole
        {
            NotAMember,
            PendingApproval,
            Member,
            Moderator
        };

        struct Member
        {
            std::weak_ptr<User> user;
            MemberRole role;
        };
    private:
        const std::string name;

        Group* parent;
        std::unordered_map<std::string, std::shared_ptr<Group>> children;

        std::list<GroupMessage> messages;
        ConnectionList connections;
        std::unordered_map<std::string, Member> members;

        mutable std::recursive_mutex mu;

        void sendNotification(MessageBase::Reference msg);
    public:
        Group(std::string name, Group* parent);

        static bool validateName(std::string name);

        std::shared_ptr<Group> createGroup(std::string name);
        bool deleteGroup(std::string name);
        std::list<std::string> listGroupNames() const;
        std::shared_ptr<Group> getGroupByName(std::string name) const;

        void registerConnection(Connection::Reference connection);
        void unregisterConnection(Connection::Reference connection);

        bool addMember(std::shared_ptr<User> user, MemberRole memberRole = MemberRole::PendingApproval);
        bool deleteMember(std::string memberLogin);
        MemberRole getMemberPermission(std::string memberLogin) const;
        void setMemberPermission(std::string memberLogin, MemberRole memberRole);

        std::list<std::string> listOfPendingApprovals() const;
        std::list<std::pair<std::string, MemberRole>> listOfMembers() const;

        void sendMessage(GroupMessage message);

        std::list<GroupMessage> getMessages(GroupMessage::Timestamp fromPoint) const;
        std::list<GroupMessage> getMessages() const;
    };
}

#endif // GROUP_H
