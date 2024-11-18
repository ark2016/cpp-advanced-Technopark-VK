// main.cpp

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include "lru.hpp"

bool is_valid_q(const std::string& q) {
    for (char c : q) {
        if (c == '\n' || c == '\r' || c == '\t' || c == '\0' || c == ' ') {
            return false;
        }
    }
    for (unsigned char c : q) {
        if (static_cast<unsigned char>(c) >= 0x80 && static_cast<unsigned char>(c) <= 0xBF) {
            return false;
        }
    }
    return true;
}

bool parse_floats(const std::string& s, std::vector<float>& floats) {
    std::istringstream iss(s);
    float f;
    while (iss >> f) {
        floats.push_back(f);
    }
    if (floats.size() != 64) {
        return false;
    }
    std::string remaining;
    if (iss >> remaining) {
        return false;
    }
    return true;
}

int main() {
    std::size_t N;
    std::size_t Size;

    if (!(std::cin >> N >> Size)) {
        std::cerr << "Failed to read N and Size\n";
        return 1;
    }
    std::string line;
    std::getline(std::cin, line);

    LRUCache<std::string, std::vector<float>> cache(N, Size);

    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            continue;
        }

        std::string key;
        std::string rest;

        std::size_t tab_pos = line.find('\t');
        if (tab_pos != std::string::npos) {
            key = line.substr(0, tab_pos);
            rest = line.substr(tab_pos + 1);

            if (!is_valid_q(key)) {
                std::cout << "!STORERR!" << std::endl;
                continue;
            }

            std::vector<float> embed;
            if (!parse_floats(rest, embed)) {
                std::cout << "!STORERR!" << std::endl;
                continue;
            }

            std::size_t key_size = key.size();
            std::size_t value_size = embed.size() * sizeof(float);

            bool res = cache.put(key, embed, key_size, value_size);
            if (res) {
                std::cout << "!STORED!" << std::endl;
            } else {
                std::cout << "!STORERR!" << std::endl;
            }
        } else {
            key = line;
            if (!is_valid_q(key)) {
                std::cout << "!NOEMBED!" << std::endl;
                continue;
            }

            std::vector<float> embed;
            bool res = cache.get(key, embed);
            if (res) {
                for (size_t i = 0; i < embed.size(); ++i) {
                    if (i > 0) {
                        std::cout << ' ';
                    }
                    std::cout << embed[i];
                }
                std::cout << std::endl;
            } else {
                std::cout << "!NOEMBED!" << std::endl;
            }
        }
    }
    return 0;
}
