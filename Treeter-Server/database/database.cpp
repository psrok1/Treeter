#include "database.h"

Database::Database()
{
    DBConnection = nullptr;
}

Database::~Database()
{
    sqlite3_close(DBConnection);
}

int Database::gatherResults(void *results, int columnsCount, char **textData, char **columnName)
{
    ResultSet *resultset = (ResultSet*) results;

    if(resultset->ColumnNames.size() < static_cast<size_t>(columnsCount))
    {
        for(int i=0; i<columnsCount; ++i)
        {
            resultset->ColumnNames.push_back(std::string(columnName[i]));
        }
    }

    Result result;
    for(int i=0; i<columnsCount; ++i)
    {
        result.push_back(std::string(textData[i]));
    }

    resultset->push_back(result);
    return 0;
}

ResultSet Database::runQuery(const char* Query, bool getResults)
{
    ResultSet results = ResultSet();
    if(getResults)
    {
        results.returnCode = sqlite3_exec(DBConnection,Query,gatherResults, (void*) &results, nullptr);
    }
    else
    {
        results.returnCode = sqlite3_exec(DBConnection,Query,nullptr, nullptr, nullptr);
    }
    return results;
}

ResultSet Database::importUsers()
{
    return runQuery("select login, password from users;",true);
}

ResultSet Database::importChildren(std::string groupPath)
{
    char* query = sqlite3_mprintf("select name from groups where parentpath='%q';",groupPath.data());
    ResultSet results = runQuery(query,true);
    sqlite3_free(query);
    return results;
}

ResultSet Database::importMembers(std::string groupPath)
{
    char* query = sqlite3_mprintf("select login, role from members where gpath='%q';",groupPath.data());
    ResultSet results = runQuery(query,true);
    sqlite3_free(query);
    return results;
}

ResultSet Database::importMessages(std::string groupPath)
{
    char* query = sqlite3_mprintf("select sender, text, timestamp from messages where channel='%q' order by timestamp asc;",groupPath.data());
    ResultSet results = runQuery(query,true);
    sqlite3_free(query);
    return results;
}

void Database::insertUser(std::string login, std::string password)
{
    char* query = sqlite3_mprintf("insert into users(login, password) values('%q','%q');",login.data(),password.data());
    runQuery(query);
    sqlite3_free(query);
}

void Database::deleteUser(std::string login)
{
    char* query = sqlite3_mprintf("delete from users where login='%q';",login.data());
    runQuery(query);
    sqlite3_free(query);
}

void Database::insertGroup(std::string name, std::string parentPath)
{

    char* query = sqlite3_mprintf("insert into groups(name, path, parentpath) values('%q','%q','%q');",name.data(),(parentPath+"/"+name).data(),parentPath.data());
    runQuery(query);
    sqlite3_free(query);
}

void Database::deleteGroup(std::string path)
{
    char* query = sqlite3_mprintf("delete from groups where path='%q';",path.data());
    runQuery(query);
    sqlite3_free(query);
}

void Database::insertMember(std::string login, std::string groupPath, int memberRole)
{
    char* query = sqlite3_mprintf("insert into members(login, gpath, role) values('%q','%q',%d);",login.data(),groupPath.data(),memberRole);
    runQuery(query);
    sqlite3_free(query);
}

void Database::deleteMember(std::string login, std::string groupPath)
{
    char* query = sqlite3_mprintf("delete from members where login='%q' and gpath='%q';",login.data(),groupPath.data());
    runQuery(query);
    sqlite3_free(query);
}

void Database::updateMemberRole(std::string login, std::string groupPath, int newRole)
{
    char* query = sqlite3_mprintf("update members set role=%d where login='%q' and gpath='%q';",newRole,login.data(),groupPath.data());
    runQuery(query);
    sqlite3_free(query);
}

void Database::insertMessage(std::string text, std::string senderLogin, std::string channelName, int timestamp)
{
    char* query = sqlite3_mprintf("insert into messages(text,sender,channel,timestamp) values('%q','%q','%q',%d);",text.data(),senderLogin.data(),channelName.data(),timestamp);
    runQuery(query);
    sqlite3_free(query);
}

void Database::init()
{
    sqlite3_config(SQLITE_CONFIG_SERIALIZED);
    sqlite3_open("data.db", &DBConnection);
}
