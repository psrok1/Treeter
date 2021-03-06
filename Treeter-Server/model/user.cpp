#include "user.h"
#include "group.h"
#include "mapgetter.h"

#include "crypto/crypto.h"

namespace Model
{
    /**
    * @brief Group::validateName
    * Checks, whether name contains only allowed chars.
    */
    bool User::validateLogin(std::string login)
    {
        return std::all_of(login.begin(), login.end(), [](const char c) {
           return std::isalnum(c) || (c == '_');
        });
    }

    /**
     * @brief User::hashPassword
     * Performs hashing on password. Used by object constructor and password verifier.
     */
    std::string User::hashPassword(std::string plain_password)
    {
        return Crypto::sha256(plain_password);
    }

    User::User(std::string login, std::string password, bool plaintextPassword):
        invalidated(false), login(login),
        password(plaintextPassword ? User::hashPassword(password) : password) { }

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

        // getAbsolutePath is non-blocking
        std::string groupPath = group->getAbsolutePath();

        if(this->groups.find(groupPath) != this->groups.end())
            return false;

        this->groups[groupPath] = group;
        return true;
    }

    /**
     * @brief User::addGroup
     * Private method: used internally by Group::deleteMember
     * Unregisters group from user's list
     */
    bool User::removeGroup(std::string groupPath)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        auto it = this->groups.find(groupPath);

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
     * @brief User::getGroupByPath
     * Gets register group by path.
     * Returns nullptr, when group wasn't registered
     */
    std::shared_ptr<Group> User::getGroupByPath(std::string groupPath)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        auto it = this->groups.find(groupPath);

        if(it == this->groups.end())
            return nullptr;

        return (*it).second;
    }

    /**
     * @brief User::listGroupPaths
     * Gets list of registered group paths
     */
    std::list<std::string> User::listGroupPaths() const
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

    /**
     * @brief User::registerConnection
     * Registers connection subscription in user object
     * Returns false, if other connection is already registered
     */
    bool User::registerConnection(std::shared_ptr<Connection> connection)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        if(!this->connection.expired())
            return false;

        this->connection = connection;
        return true;
    }

    /**
     * @brief User::unregisterConnection
     * Removes subscription
     */
    void User::unregisterConnection()
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        this->connection.reset();
    }

    /**
     * @brief User::getConnection
     * Returns locked connection pointer
     */
    std::shared_ptr<Connection> User::getConnection()
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        return this->connection.lock();
    }


    void User::exportToDatabase()
    {
        DB.insertUser(login,password);
    }
}
