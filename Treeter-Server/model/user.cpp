#include "user.h"
#include "group.h"
#include "mapgetter.h"

namespace Model
{
    bool User::validateLogin(std::string login)
    {
        // @TODO
        (void)login;
        return true;
    }

    std::string User::hashPassword(std::string plain_password)
    {
        // @TODO
        return plain_password;
    }

    bool User::isInvalidated() const
    {
        return this->isInvalidated();
    }

    void User::invalidate()
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        this->invalidated = true;
    }

    User::User(std::string login, std::string password):
        login(login), password(User::hashPassword(password)) { }

    bool User::addGroup(std::shared_ptr<Group> group)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        // getName is non-blocking
        std::string groupName = group->getName();

        if(this->groups.find(groupName) != this->groups.end())
            return false;

        this->groups[groupName] = group;
        return true;
    }

    bool User::removeGroup(std::string groupName)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

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
        std::unique_lock<std::recursive_mutex> lck(mu);

        auto it = this->groups.find(groupName);

        if(it == this->groups.end())
            return nullptr;

        return (*it).second;
    }

    std::list<std::string> User::listGroupNames() const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        return getKeys(this->groups);
    }

    std::list<std::shared_ptr<Group>> User::listGroupReferences() const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        return getValues(this->groups);
    }
}
