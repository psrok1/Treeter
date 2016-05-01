#include "config.h"
#include "json.hpp"

#include <fstream>

namespace Configuration
{
    const char* file_name = "settings.json";
    using json = nlohmann::json;
    json j;

    bool load()
    {
        std::ifstream file;
        file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try {
            file.open (file_name);
            std::string str((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());

            file.close();
            j = json::parse(str);
        }
        catch (std::ifstream::failure e) {
            std::cerr << "Exception opening/reading/closing file\n";
            return false;
        }
        catch (std::invalid_argument e) {
            std::cerr << "Exception parsing file\n";
            return false;
        }
        return true;
    }

    bool save()
    {
        try {
            std::ofstream file(file_name);
            file << j.dump();
            file.close();
        }
        catch(std::ofstream::failure e) {
            std::cerr << "Exception opening/writing/closing file\n";
            return false;
        }
        return true;
    }

    unsigned short getServerPort()
    {
        return j["serverPort"];
    }

    void setServerPort(unsigned short port)
    {
        j["serverPort"] = port;
    }

    std::string getDatabaseName()
    {
        return j["databaseName"];
    }

    void setDatabaseName(std::string dbname)
    {
        j["databaseName"] = dbname;
    }
}
