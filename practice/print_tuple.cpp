#include <string>
#include <iostream>
#include <tuple>

template <class T>
std::string to_string(T &&t)
{
    return std::to_string(t);
}

std::string to_string(const std::string &t)
{
    return t;
}

template <class Tuple, size_t... Is>
void to_tuple_string(std::string &ret, Tuple &&t, std::index_sequence<Is...>)
{
    ((ret += (Is == 0 ? "" : ",") + to_string(std::get<Is>(t))), ...);
}

template <class... Args>
std::string to_tuple_string(const std::tuple<Args...> &t)
{
    std::string ret = "(";
    to_tuple_string(ret, t, std::index_sequence_for<Args...>{});
    ret += ")";
    return ret;
}

template <class Tuple, size_t N = std::tuple_size<std::decay_t<Tuple>>::value>
std::string to_tuple_string2(Tuple &&t)
{
    std::string ret = "(";
    to_tuple_string(ret, t, std::make_index_sequence<N>{});
    ret += ")";
    return ret;
}
#if __cplusplus >= 201703L
template <class... Ts>
std::string to_tuple_string3(const std::tuple<Ts...> &t)
{
    std::string ret = "(";
    std::apply([&ret](const Ts &... tuple_arg) {
        size_t n = 0;
        ((ret += (n++ == 0 ? "" : ",") + to_string(tuple_arg)), ...);
    },
               t);
    ret += ")";
    return ret;
}
#else

template <class... Ts>
std::string to_tuple_string3(const std::tuple<Ts...> &t)
{
    return "need cpp17";
}
#endif

int main()
{
    auto t0 = std::make_tuple<int>(1);
    auto s0 = to_tuple_string(t0);
    std::cout << "t0: " << s0 << std::endl;

    auto t1 = std::make_tuple<int, double, std::string>(1, 3.3, "test");
    std::cout << "t1: " << to_tuple_string(t1) << std::endl;

    auto t2 = std::make_tuple<int, int, char>(1, 3.3, 'c');
    std::cout << "t2: " << to_tuple_string2(t2) << std::endl;

    auto t3 = t2;
    std::cout << "t3: " << to_tuple_string3(t3) << std::endl;
}
