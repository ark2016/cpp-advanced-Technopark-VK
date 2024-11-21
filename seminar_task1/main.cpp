#include <iostream>
#include <string>
#include <limits>
#include <concepts>
#include <cstdio>
#include <cstdint>

template<typename T>
concept IntegralExceptBool = std::is_integral_v<T> && !std::is_same_v<T, bool>;

template<typename T>
concept CStyleString = std::is_same_v<T, const char*> || std::is_same_v<T, char*>;

template<typename T>
std::string our_to_string(T val) {
    if constexpr (IntegralExceptBool<T>) {
        char buffer[100];
        if constexpr (std::is_unsigned_v<T>) {
            std::sprintf(buffer, "%llu", static_cast<unsigned long long>(val));
        } else {
            std::sprintf(buffer, "%lld", static_cast<long long>(val));
        }
        return std::string(buffer);
    } else if constexpr (std::is_floating_point_v<T>) {
        char buffer[1000];
        std::sprintf(buffer, "%f", val);
        return std::string(buffer);
    } else if constexpr (std::is_same_v<T, bool>) {
        return val ? "true" : "false";
    } else if constexpr (CStyleString<T>) {
        return std::string(val);
    } else {
        static_assert(std::is_void<T>::value, "Unsupported type for our_to_string");
    }
}

int main() {
    std::cout << our_to_string(42) << std::endl;
    std::cout << our_to_string(std::numeric_limits<uint64_t>::max()) << std::endl;
    std::cout << our_to_string(1.0f) << std::endl;
    std::cout << our_to_string(std::numeric_limits<double>::max()) << std::endl;
    std::cout << our_to_string("Lorem ipsum dolor sit amet, consectetur adipiscing elit") << std::endl;
//    int a = 42;
//    std::cout << our_to_string(&a) << std::endl;
    return 0;
}
/*
 * Output:
 * 42
 * 18446744073709551615
 * 1.000000
 * 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464
 * 234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559
 * 332123348274797826204144723168738177180919299881250404026184124858368.000000
 * Lorem ipsum dolor sit amet, consectetur adipiscing elit
 */