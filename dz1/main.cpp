// main.cpp
#include <bits/stdc++.h>
#include "lru.hpp"

// Function to calculate size in bytes of a key-value pair
size_t calculate_entry_size(const std::string& key, const std::vector<float>& embed) {
    return key.size() + embed.size() * sizeof(float);
}

// Function to parse a line of type (a)
bool parse_line_a(const std::string& line, std::string& q, std::vector<float>& embed) {
    size_t tab_pos = line.find('\t');
    if (tab_pos == std::string::npos) {
        return false;
    }
    q = line.substr(0, tab_pos);
    std::string embed_str = line.substr(tab_pos + 1);
    std::istringstream iss(embed_str);
    float num;
    while (iss >> num) {
        embed.push_back(num);
    }
    return !embed.empty();
}

// Function to parse a line of type (b)
bool parse_line_b(const std::string& line, std::string& q) {
    q = line;
    return true;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    size_t N;
    size_t Size;
    std::cin >> N >> Size;
    std::cin.ignore(); // ignore the remaining newline after Size

    LRUCache<std::string, std::vector<float>> cache(N, Size);

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        if (line.find('\t') != std::string::npos) {
            // Type (a)
            std::string q;
            std::vector<float> embed;
            if (parse_line_a(line, q, embed)) {
                size_t entry_size = calculate_entry_size(q, embed);
                if (cache.put(q, embed, entry_size)) {
                    std::cout << "!STORED!\n";
                } else {
                    std::cout << "!STORERR!\n";
                }
            } else {
                std::cout << "!STORERR!\n";
            }
        } else {
            // Type (b)
            std::string q;
            if (parse_line_b(line, q)) {
                std::vector<float> embed;
                if (cache.get(q, embed)) {
                    std::ostringstream oss;
                    for (size_t i = 0; i < embed.size(); ++i) {
                        if (i > 0) oss << ' ';
                        oss << embed[i];
                    }
                    oss << '\n';
                    std::cout << oss.str();
                } else {
                    std::cout << "!NOEMBED!\n";
                }
            } else {
                std::cout << "!NOEMBED!\n";
            }
        }
    }

    return 0;
}