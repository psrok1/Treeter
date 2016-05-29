#ifndef USER_H
#define USER_H

#include <string>
#include <mutex>
#include <list>
#include <unordered_map>
#include <memory>
#include <atomic>
#include <database/database.h>

extern Database DB;

class Connection;

namespace Model
{
    class Group;

    class User
    {
        friend class Group;

        std::atomic<bool> invalidated;

        const std::string login;
        const std::string password;

        std::weak_ptr<Connection> connection;

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

        bool registerConnection(std::shared_ptr<Connection> connection);
        void unregisterConnection();
        std::shared_ptr<Connection> getConnection();

        void exportToDatabase();
    };
}

#endif // USER_H
