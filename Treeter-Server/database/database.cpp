#include "database.h"

Database::Database()
{
    sqlite3_config(SQLITE_CONFIG_SERIALIZED);
    sqlite3_open("data.db", &DBConnection);
}

Database::~Database()
{
    sqlite3_close(DBConnection);
}

int Database::gatherResults(void *results, int columnsCount, char **textData, char **columnName)
{
    ResultSet *resultset = (ResultSet*) results;

    if(resultset->ColumnNames.size()<columnsCount)
        for(int i=0; i<columnsCount; ++i)
        {
            resultset->ColumnNames.push_back(std::string(columnName[i]));
        };

    Result result;
    for(int i=0; i<columnsCount; ++i)
    {
        result.push_back(std::string(textData[i]));
    }

    resultset->push_back(result);
    return 0;
}

ResultSet Database::runQuery(std::string Query)
{
    ResultSet results;
    results.returnCode = sqlite3_exec(DBConnection,Query.data(),gatherResults, (void*) &results, nullptr);
    return results;
}
