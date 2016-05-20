#include <algorithm>
#include "datamodel.h"

namespace Model
{
    DataModel::DataModel()
    {
        this->rootGroup = std::shared_ptr<Group>(new Group("", nullptr));
    }

    std::shared_ptr<Group> DataModel::getRootGroup() const
    {
        return this->rootGroup;
    }

    std::shared_ptr<User> DataModel::getUserByLogin(std::string login) const
    {
        std::lock_guard<std::recursive_mutex> lck(mu);

        auto it = this->users.find(login);

        if(it == this->users.end())
            return nullptr;

        return (*it).second;
    }

    std::list<std::string> DataModel::listUserLogins() const
    {
        std::lock_guard<std::recursive_mutex> lck(mu);

//        std::list<std::string> result_list;
//        result_list.resize(this->users.size());

//        typedef std::pair<std::string, std::shared_ptr<User>> UserEntry;
//        std::transform(this->users.begin(), this->users.end(), this->users.begin(), result_list.begin(),
//                       [](UserEntry user_entry, UserEntry) -> std::string { return user_entry.second; });

//        return result_list;
    }

    std::shared_ptr<User> DataModel::addUser(std::string login, std::string password)
    {
        std::lock_guard<std::recursive_mutex> lck(mu);

        if(this->users.find(login) == this->users.end())
            return nullptr;

        return (this->users[login] = std::shared_ptr<User>(new User(login, password)));
    }

    bool DataModel::deleteUser(std::string login)
    {
        std::lock_guard<std::recursive_mutex> lck(mu);

        auto it = this->users.find(login);

        if(it == this->users.end())
            return false;

        this->users.erase(it);
        return true;
    }
}
