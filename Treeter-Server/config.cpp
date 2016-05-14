#include "config.h"
#include "message/json.hpp"

#include <fstream>

namespace Configuration
{
    const char* file_name = "settings.json";
    using json = nlohmann::json;
    json j;

    bool checkFields();

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
            if(!checkFields())
            {
                std::cerr << "Loading configuration failed: missing field or unexpected type of value\n";
                return false;
            }
        }
        catch (std::ifstream::failure e) {
            std::cerr << "Loading configuration failed: can't open file for reading\n";
            return false;
        }
        catch (std::invalid_argument e) {
            std::cerr << "Loading configuration failed: wrong JSON format\n";
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
            std::cerr << "Storing configuration failed: can't open file for writing\n";
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

    bool checkFields() {
        if(!j["databaseName"].is_string())
            return false;
        if(!j["serverPort"].is_number())
            return false;
        return true;
    }
}
