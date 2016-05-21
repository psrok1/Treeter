#include <algorithm>
#include "datamodel.h"
#include "mapgetter.h"

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
        return getKeys(this->users);
    }

    std::shared_ptr<User> DataModel::addUser(std::string login, std::string password)
    {
        std::lock_guard<std::recursive_mutex> lck(mu);

        if(this->users.find(login) == this->users.end())
            return nullptr;

        std::shared_ptr<User> user_ref(new User(login, password));

        this->users[login] = user_ref;

        // User should be added to root group
        this->rootGroup->addMember(user_ref, Group::MemberRole::Member);

        return user_ref;
    }

    bool DataModel::deleteUser(std::string login)
    {
        std::lock_guard<std::recursive_mutex> lck(mu);

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
        return true;
    }
}
