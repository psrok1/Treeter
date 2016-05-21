#ifndef KEYGETTER_H
#define KEYGETTER_H

#include <algorithm>
#include <list>

namespace Model
{
    template<typename Map>
    std::list<Map::key_type> getKeys(const Map& m)
    {
        std::list<Map::key_type> keys;
        struct KeyGetter
        {
            template <typename T>
            typename T::first_type operator()(T keyValuePair) const
            {
                return keyValuePair.first;
            }
        };
        std::transform(m.begin(), m.end(), std::back_inserter(keys), KeyGetter());
        return keys;
    }

    template<typename Map>
    std::list<Map::value_type> getValues(const Map& m)
    {
        std::list<Map::value_type> values;
        struct ValueGetter
        {
            template <typename T>
            typename T::second_type operator()(T keyValuePair) const
            {
                return keyValuePair.second;
            }
        };
        std::transform(m.begin(), m.end(), std::back_inserter(values), ValueGetter());
        return values;
    }
}

#endif // KEYGETTER_H
