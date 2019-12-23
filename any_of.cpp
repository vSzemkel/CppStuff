#include <iostream>
#include <array>
#include <algorithm>

struct Bucket {
    int xx;
    char code;
};

std::array<Bucket, 3> g_buckets = {
    Bucket{0, 'D'},
    Bucket{1, 'E'},
    Bucket{2, 'F'}
};

int main() {
    Bucket pattern;
    std::cout << "Enter a single digit key: ";
    std::cin >> pattern.xx;
    std::cout << "Enter single letter code: ";
    std::cin >> pattern.code;

    const auto test = [&pattern](const Bucket& b){ return b.xx == pattern.xx && b.code == pattern.code;};
    if(std::none_of(std::begin(g_buckets), std::end(g_buckets), test))
        std::cout << "not ";
        
    std::cout << "found\n";
}