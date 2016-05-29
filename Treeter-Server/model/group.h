#ifndef GROUP_H
#define GROUP_H

#include <string>
#include <memory>
#include <unordered_map>
#include <list>
#include <mutex>

#include "../connectionlist.h"
#include "groupmessage.h"
#include "memberrole.h"
#include "../message/messageoutgoing.h"

extern Database DB;

namespace Model
{
    class DataModel;
    class User;

    struct Group
    {
        struct Member
        {
            std::weak_ptr<User> user;
            MemberRole role;

            Member() {}
            Member(std::weak_ptr<User> user, MemberRole role)
                : user(user), role(role) { }
        };
    private:
        std::atomic<bool> invalidated;

        const std::string name;
        const std::string absolutePath;

        Group* parent;
        std::unordered_map<std::string, std::shared_ptr<Group>> children;

        std::list<GroupMessage> messages;
        std::unordered_map<std::string, Member> members;

        mutable std::recursive_mutex mu;

        void sendNotification(MessageOutgoing::Reference msg);
        void clean();
    public:
        Group(std::string name, Group* parent);
        void invalidate() { this->invalidated = true; }

        Group(const Group&) = delete;
        Group& operator=(const Group&) = delete;

        static bool validateName(std::string name);
        static std::pair<std::string, std::string> splitParentPath(std::string path);
        std::string getGroupName() const;
        std::string getAbsolutePath() const;

        std::shared_ptr<Group> createGroup(std::string name, bool imported = false);
        bool deleteGroup(std::string name);
        std::list<std::string> listGroupNames() const;
        std::shared_ptr<Group> getGroupByName(std::string name) const;

        bool addMember(std::shared_ptr<Group> group_ref, std::shared_ptr<User> user, MemberRole memberRole = MemberRole::PendingApproval, bool imported = false);
        bool deleteMember(std::string memberLogin);
        MemberRole getMemberPermission(std::string memberLogin) const;
        bool setMemberPermission(std::string memberLogin, MemberRole memberRole);
        bool hasModerator() const;

        std::list<std::string> listOfPendingApprovals() const;
        std::list<std::pair<std::string, MemberRole>> listOfMembers() const;

        void sendMessage(GroupMessage message);

        std::list<GroupMessage> getMessages(GroupMessage::Timestamp fromPoint) const;
        std::list<GroupMessage> getMessages() const;

        void importFromDatabase(DataModel& model);
    };
}

#endif // GROUP_H
