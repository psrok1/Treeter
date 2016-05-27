#ifndef TREETERDATABASE_H
#define TREETERDATABASE_H

#include "sqlite3.h"
#include "resultset.h"

class Database
{
    sqlite3 *DBConnection;

    static int gatherResults(void *ResultSet, int argc, char **argv, char **azColName);

public:
    ResultSet runQuery(std::string Query);
    Database();
    ~Database();
};

#endif TREETERDATABASE_H
