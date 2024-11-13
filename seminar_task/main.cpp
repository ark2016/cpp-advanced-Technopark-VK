#include <iostream>
#include <string_view>
#include <chrono>

// Базовый случай, когда нет аргументов для замены
void println(std::string_view fmt) {
    std::cout << fmt << std::endl;
}

// Рекурсивный шаблон для обработки одного аргумента и последующих
template<class T, class... Args>
void println(std::string_view fmt, T &&value, Args &&... args) {
    // Находим первое вхождение "{}" в форматной строке
    std::size_t pos = fmt.find("{}");

    if (pos != std::string_view::npos) {
        // Печатаем часть строки до "{}"
        std::cout << fmt.substr(0, pos);
        // Печатаем текущее значение
        std::cout << std::forward<T>(value);
        // Вызываем функцию снова для оставшейся строки и оставшихся аргументов
        println(fmt.substr(pos + 2), std::forward<Args>(args)...);
    } else {
        // Если "{}" нет, просто печатаем остаток строки
        std::cout << fmt;
    }
}

int main() {
    auto res = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();

    println("{}, {}! Unix timestamp right now: {}", "Hello", "world", res);

    return 0;
}