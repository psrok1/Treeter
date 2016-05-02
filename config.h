#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace Configuration
{
    extern bool load();
    extern bool save();

    extern unsigned short getServerPort();
    extern void setServerPort(unsigned short port);

    extern std::string getDatabaseName();
    extern void setDatabaseName(std::string dbname);

    extern bool checkFields();
}

#endif // CONFIG_H

