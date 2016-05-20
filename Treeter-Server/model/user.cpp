#include "user.h"
#include "group.h"

namespace Model
{
    bool User::validateLogin(std::string login)
    {
        // @TODO
    }

    std::string User::hashPassword(std::string plain_password)
    {
        // @TODO
    }

    User::User(std::string login, std::string password):
        login(login), password(User::hashPassword(password)) { }

    bool User::addGroup(std::shared_ptr<Group> group)
    {
        std::lock_guard<std::recursive_mutex> lck(mu);

        std::string groupName = group->getName();
        auto it = this->groups.find(groupName);

        if(it == this->groups.end())
            return false;

        this->groups[groupName] = group;
        return true;
    }

    bool User::removeGroup(std::string groupName)
    {
        std::lock_guard<std::recursive_mutex> lck(mu);

        auto it = this->groups.find(groupName);

        if(it == this->groups.end())
            return false;

        this->groups.erase(it);
        return true;
    }

    std::string User::getLogin() const
    {
        return this->login;
    }

    bool User::verifyPassword(std::string pass) const
    {
        // Check whether reading from string is thread-safe
        // If password is immutable, keep without critical section
        return this->hashPassword(pass) == this->password;
    }

    std::shared_ptr<Group> User::getGroupByName(std::string groupName)
    {
        std::lock_guard<std::recursive_mutex> lck(mu);

        auto it = this->groups.find(groupName);

        if(it == this->groups.end())
            return nullptr;

        return (*it).second;
    }

    std::list<std::string> User::listGroupNames() const
    {
        std::lock_guard<std::recursive_mutex> lck(mu);
        // @TODO
    }
}
