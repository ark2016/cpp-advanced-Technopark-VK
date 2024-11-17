#include <iostream>
#include <string_view>
#include <chrono>

using namespace std;

void println(string_view fmt) {
    cout << fmt << endl;
}

template<class T, class... Args>
void println(string_view fmt, T &&value, Args &&... args) {
    size_t pos = fmt.find("{}");
    if (pos != string_view::npos) {
        cout << fmt.substr(0, pos);
        cout << forward<T>(value);
        println(fmt.substr(pos + 2), forward<Args>(args)...);
    } else {
        cout << fmt;
    }
}

int main() {
    auto res = chrono::duration_cast<chrono::seconds>(
            chrono::system_clock::now().time_since_epoch()).count();
    println("{}, {}! Unix timestamp right now: {}", "Hello", "world", res);
    return 0;
}