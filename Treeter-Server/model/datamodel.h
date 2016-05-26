#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <memory>
#include <unordered_map>
#include <list>
#include <mutex>

#include "group.h"
#include "user.h"

namespace Model
{
    class DataModel
    {
        std::shared_ptr<Group> rootGroup;
        std::unordered_map<std::string, std::shared_ptr<User>> users;

        mutable std::recursive_mutex mu;
    public:
        DataModel();

        DataModel(const DataModel&) = delete;
        DataModel& operator=(const DataModel&) = delete;

        std::shared_ptr<User> getUserByLogin(std::string login) const;
        std::list<std::string> listUserLogins() const;
        std::shared_ptr<User> addUser(std::string login, std::string password);
        bool deleteUser(std::string login);

        std::shared_ptr<Group> getRootGroup() const;
    };
}

#endif // DATAMODEL_H
