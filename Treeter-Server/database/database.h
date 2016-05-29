#ifndef TREETERDATABASE_H
#define TREETERDATABASE_H

#include "sqlite3.h"
#include "resultset.h"

class Database
{
    sqlite3 *DBConnection;

    static int gatherResults(void *ResultSet, int argc, char **argv, char **azColName);
    ResultSet runQuery(const char* Query, bool getResults = false);

public:
    ResultSet importUsers();
    ResultSet importChildren(std::string groupPath);
    ResultSet importMembers(std::string groupPath);
    ResultSet importMessages(std::string groupPath);
    void insertUser(std::string login, std::string password);
    void deleteUser(std::string login);
    void insertGroup(std::string name, std::string parentPath);
    void deleteGroup(std::string path);
    void insertMember(std::string login, std::string groupPath, int memberRole);
    void deleteMember(std::string login, std::string groupPath);
    void updateMemberRole(std::string login, std::string groupPath, int newRole);
    void insertMessage(std::string text, std::string senderLogin, std::string channelName, int timestamp);
    void init();
    Database();
    ~Database();
};

#endif
