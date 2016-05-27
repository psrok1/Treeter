#ifndef USER_H
#define USER_H

#include <string>
#include <mutex>
#include <list>
#include <unordered_map>
#include <memory>
#include <atomic>

namespace Model
{
    class Group;

    class User
    {
        friend class Group;

        std::atomic<bool> invalidated;

        const std::string login;
        const std::string password;

        std::unordered_map<std::string, std::shared_ptr<Group>> groups;

        mutable std::recursive_mutex mu;

        static std::string hashPassword(std::string plain_password);

        // Can only be called by Group methods
        // Double-locked: first group, second user
        bool addGroup(std::shared_ptr<Group> group);
        bool removeGroup(std::string groupPath);
    public:
        User(std::string login, std::string password, bool plaintextPassword = true);
        void invalidate() { this->invalidated = true; }

        User(const User&) = delete;
        User& operator=(const User&) = delete;

        static bool validateLogin(std::string login);

        std::string getLogin() const;
        bool verifyPassword(std::string pass) const;

        std::shared_ptr<Group> getGroupByPath(std::string groupPath);
        std::list<std::string> listGroupPaths() const;
        std::list<std::shared_ptr<Group>> listGroupReferences() const;

        void importFromDatabase();
        void exportToDatabase();
    };
}

#endif // USER_H
