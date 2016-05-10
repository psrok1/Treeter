#ifndef BASE64_H
#define BASE64_H

#include <string>

namespace Base64
{
    extern std::string encode(const std::string &bindata);
    extern std::string decode(const std::string &ascdata);
}

#endif // BASE64_H
