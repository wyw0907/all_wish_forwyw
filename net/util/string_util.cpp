#include "string_util.h"

namespace wish
{
    std::vector<std::string> split(const std::string &str, std::string::value_type separator)
    {
        std::vector<std::string> vec;
        std::string::const_iterator last_it = str.cbegin(), it = last_it;
        for (; it != str.cend(); ++it)
        {
            if (*it == separator)
            {
                vec.push_back(std::string(last_it, it));
                last_it = it + 1;
            }
        }

        if (last_it != it)
        {
            vec.push_back(std::string(last_it, it));
        }
        return vec;
    }
}