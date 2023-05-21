#ifndef CUSTOMVARIANT_H
#define CUSTOMVARIANT_H

#include <vector>
#include <variant>
#include <string>
#include <typeinfo>

namespace CustVar
{
    // All types defined in var must have their case defined in Database.cpp
    typedef std::variant<int, uint32_t, bool, float, double, std::string> var;
    typedef std::vector<var> varList;

    inline std::string VarToString(var v)
    {
        return std::visit([](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, int>) {
                    return std::to_string(arg);
                } else if constexpr (std::is_same_v<T, uint32_t>) {
                    return std::to_string(arg);
                } else if constexpr (std::is_same_v<T, bool>) {
                    return std::to_string((int)arg);
                } else if constexpr (std::is_same_v<T, float>) {
                    return std::to_string(arg);
                } else if constexpr (std::is_same_v<T, double>) {
                    return std::to_string(arg);
                } else if constexpr (std::is_same_v<T, std::string>) {
                    return arg;
                } else
                {
                    throw "VarToString unexptected type";
                }
            }, v);
    }
}
#endif // CUSTOMVARIANT_H
