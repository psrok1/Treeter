#include <algorithm>
#include "datamodel.h"
#include "mapgetter.h"

namespace Model
{
    /**
     * @brief DataModel::DataModel
     * DataModel constructor, which creates root group
     */
    DataModel::DataModel()
    {
        this->rootGroup = std::shared_ptr<Group>(new Group("", nullptr));
    }

    /**
     * @brief DataModel::getRootGroup
     * Root group getter. Property is immutable, so critical section is unnecessary.
     * @NEEDS-THREAD-SAFETY-CHECK
     */
    std::shared_ptr<Group> DataModel::getRootGroup() const
    {
        return this->rootGroup;
    }

    /**
     * @brief DataModel::getUserByLogin
     * Gets pointer to user by login
     * Returns nullptr, if user doesn't exist
     */
    std::shared_ptr<User> DataModel::getUserByLogin(std::string login) const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        auto it = this->users.find(login);

        // If user doesn't exist: return nullptr
        if(it == this->users.end())
            return nullptr;

        return (*it).second;
    }

    /**
     * @brief DataModel::listUserLogins
     * Users list getter
     */
    std::list<std::string> DataModel::listUserLogins() const
    {
        std::unique_lock<std::recursive_mutex> lck(mu);
        return getKeys(this->users);
    }

    /**
     * @brief DataModel::addUser
     * Adds user using specified login and password
     * Returns pointer to brand-new user object, or nullptr if operation wasn't successful
     */
    std::shared_ptr<User> DataModel::addUser(std::string login, std::string password, bool plaintextPassword)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        // User is added yet: nullptr
        if(this->users.find(login) != this->users.end())
            return nullptr;

        std::shared_ptr<User> user_ref(new User(login, password, plaintextPassword));

        this->users[login] = user_ref;

        if(plaintextPassword)
        {
            user_ref->exportToDatabase();

            // User should be added to root group
            this->rootGroup->addMember(this->rootGroup, user_ref, MemberRole::Member);
        }

        return user_ref;
    }

    /**
     * @brief DataModel::deleteUser
     * Deletes user for users list
     * Invalidates user instance and remove references from all related groups
     * Invalidation cancels pending "addMember" operations
     *
     * Returns false, if user has been deleted yet.
     */
    bool DataModel::deleteUser(std::string login)
    {
        std::unique_lock<std::recursive_mutex> lck(mu);

        auto it = this->users.find(login);

        if(it == this->users.end())
            return false;

        std::shared_ptr<User> u = (*it).second;

        // Invalidate user object
        u->invalidate();

        // Detach from groups
        std::list<std::shared_ptr<Group>> groups = u->listGroupReferences();

        for(std::shared_ptr<Group>& group_ptr: groups)
            group_ptr->deleteMember(login);

        this->users.erase(it);

        // Remove from DB
        DB.deleteUser(login);

        return true;
    }

    void DataModel::importFromDatabase()
    {
        ResultSet userList = DB.importUsers();

        for(auto& userdata: userList)
        {
            this->addUser(userdata[0], userdata[1], false);
        }

        // Import root members
        ResultSet rootMembers = DB.importMembers("/");

        for(auto member: rootMembers)
        {
            this->rootGroup->addMember(this->rootGroup, this->getUserByLogin(member[0]), static_cast<MemberRole>(atoi(member[1].data())),true);
        }

        this->rootGroup->importFromDatabase(*this);
    }
}
