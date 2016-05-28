#ifndef RESULTSET_H
#define RESULTSET_H

#include <string>
#include <vector>

typedef std::vector<std::string> Result;
class ResultSet: public std::vector<Result>
{
public:
    int returnCode;
    std::vector<std::string> ColumnNames;
    ResultSet();
    ~ResultSet();
};

#endif // RESULTSET_H
