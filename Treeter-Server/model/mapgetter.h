#ifndef KEYGETTER_H
#define KEYGETTER_H

#include <algorithm>
#include <list>

namespace Model
{
    template<typename Map>
    std::list<typename Map::key_type> getKeys(const Map& m)
    {
        std::list<typename Map::key_type> keys;

        std::transform(m.begin(), m.end(), std::back_inserter(keys),
            [](typename Map::allocator_type::value_type keyValuePair) -> typename Map::key_type {
                return keyValuePair.first;
        });
        return keys;
    }

    template<typename Map>
    std::list<typename Map::mapped_type> getValues(const Map& m)
    {
        std::list<typename Map::mapped_type> values;

        std::transform(m.begin(), m.end(), std::back_inserter(values),
            [](typename Map::allocator_type::value_type keyValuePair) -> typename Map::mapped_type {
                return keyValuePair.second;
        });
        return values;
    }
}

#endif // KEYGETTER_H
