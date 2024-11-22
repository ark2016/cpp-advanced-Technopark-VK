#include <iostream>
#include <string>
#include <limits>
#include <cstdio>
#include <cstdint>


template<typename T>
std::string our_to_string(T val) {
    if constexpr (std::is_integral_v<T> && !std::is_same_v<T, bool>) {
        char buffer[100];
        if constexpr (std::is_unsigned_v<T>) {
            std::sprintf(buffer, "%llu", static_cast<unsigned long long>(val));
        } else {
            std::sprintf(buffer, "%lld", static_cast<long long>(val));
        }
        return std::string(buffer);
    } else if constexpr (std::is_floating_point_v<T>) {
        char buffer[1000];
        std::sprintf(buffer, "%f", val); // можно  использовать "%.1f" вместо "%f" для вывода 1 занка
        // после запятой, но опираясь на std::to_string оставил "%f"
        return std::string(buffer);
    } else if constexpr (std::is_same_v<T, bool>) {
        return val ? "true" : "false";
    } else if constexpr (std::is_same_v<T, const char*> || std::is_same_v<T, char*>) {
        return std::string(val);
    } else {
        static_assert(std::is_void<T>::value, "Unsupported type for our_to_string");
    }
}

int main() {
    std::cout << our_to_string(42) << std::endl;
    std::cout << our_to_string(std::numeric_limits<uint64_t>::max()) << std::endl;
    std::cout << our_to_string(1.0f) << std::endl;
//    std::cout << std::to_string(1.0f) << std::endl;
    std::cout << our_to_string(std::numeric_limits<double>::max()) << std::endl;
    std::cout << our_to_string("Lorem ipsum dolor sit amet, consectetur adipiscing elit") << std::endl;
//    int a = 42;
//    std::cout << our_to_string(&a) << std::endl;
    return 0;
}