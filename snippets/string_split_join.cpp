#include <algorithm>
#include <cassert>
#include <sstream>
#include <string>
#include <vector>


constexpr std::vector<std::string> split(std::string_view strv, std::string_view delims = " ") {
    std::vector<std::string> ret;
    size_t pos{0}, sz = strv.size();

    while (pos < sz) {
        const auto end = strv.find_first_of(delims, pos);

        if (pos != end)
            ret.emplace_back(strv.substr(pos, end - pos));

        if (end == std::string_view::npos)
            break;

        pos = end + 1;
    }

    return ret;
}

template <class C, class S>
std::string join(const C& container, const S& separator) {
    if (container.empty()) {
        return {};
    }

    std::ostringstream ret;
    for (const auto& s : container) {
        ret << separator << s;
    }

    return ret.str().data() + separator.size();
}

template <class C, size_t N>
std::string join(const C& container, const char (&separator)[N]) {
    if (container.empty()) {
        return {};
    }

    std::ostringstream ret;
    for (const auto& s : container) {
        ret << separator << s;
    }

    return ret.str().data() + N - 1;
}

int main(int, char**)
{
    const char text[] = "aaa bbb ccc";
    const auto spl = split(text);
    assert(spl.size() == 3);
    assert(spl.back().front() == 'c');
    assert(join(spl, " ") == text);

    const std::string sep{", "};
    assert(join(split(text), sep) == "aaa, bbb, ccc");
    assert(join(split(text), ", ") == "aaa, bbb, ccc");
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 string_split_join.cpp -o string_split_join.exe

*/
