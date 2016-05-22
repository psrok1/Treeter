#include "user.h"
#include "group.h"
#include "mapgetter.h"

namespace Model
{
    /**
    * @brief Group::validateName
    * Checks, whether name contains only allowed chars.
    */
    bool User::validateLogin(std::string login)
    {
        // @TODO
        (void)login;
        return true;
    }

    /**
     * @brief User::hashPassword
     * Performs hashing on password. Used by object constructor and password verifier.
     */
    std::string User::hashPassword(std::string plain_password)
    {
        // @TODO
        return plain_password;
    }

    User::User(std::string login, std::string password):
        invalidated(false), login(login), password(User::hashPassword(password)) { }

    /**
     * @brief User::addGroup
     * Private method: used internally by Group::addMember
     * Registers group in user's list
     * Fails, when group exists yet or when user object was invalidated.
     */
    bool User::addGroup(std::shared_ptr<Group> group)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        // Object is being deleted
        if(this->invalidated)
            return false;

        // getName is non-blocking
        std::string groupName = group->getName();

        if(this->groups.find(groupName) != this->groups.end())
            return false;

        this->groups[groupName] = group;
        return true;
    }

    /**
     * @brief User::addGroup
     * Private method: used internally by Group::deleteMember
     * Unregisters group from user's list
     */
    bool User::removeGroup(std::string groupName)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        auto it = this->groups.find(groupName);

        if(it == this->groups.end())
            return false;

        this->groups.erase(it);
        return true;
    }

    /**
     * @brief User::getLogin()
     * User login getter
     * Property is immutable, so critical section is unnecessary.
     * @NEEDS-THREAD-SAFETY-CHECK
     */
    std::string User::getLogin() const
    {
        return this->login;
    }

    /**
     * @brief User::verifyPassword
     * Performs hashing on argument and checks whether specified password is valid
     */
    bool User::verifyPassword(std::string pass) const
    {
        // Check whether reading from string is thread-safe
        // If password is immutable, keep without critical section
        return this->hashPassword(pass) == this->password;
    }

    /**
     * @brief User::getGroupByName
     * Gets register group by name.
     * Returns nullptr, when group wasn't registered
     */
    std::shared_ptr<Group> User::getGroupByName(std::string groupName)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        auto it = this->groups.find(groupName);

        if(it == this->groups.end())
            return nullptr;

        return (*it).second;
    }

    /**
     * @brief User::listGroupNames
     * Gets list of registered group names
     */
    std::list<std::string> User::listGroupNames() const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        return getKeys(this->groups);
    }

    /**
     * @brief User::listGroupReferences
     * Internal usage allowed only!
     * Gets list of registered group instances.
     */
    std::list<std::shared_ptr<Group>> User::listGroupReferences() const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        return getValues(this->groups);
    }
}
