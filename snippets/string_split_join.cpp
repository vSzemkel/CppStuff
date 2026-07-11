#include <algorithm>
#include <cassert>
#include <charconv>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_set>
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

    const auto tail = container.size() - 1;
    const auto first = std::begin(container);
    if (tail == 0) {
        return *first;
    }

    std::ostringstream ret(*first, std::ios_base::out | std::ios_base::ate);
    std::for_each_n(std::next(first), tail, [&](const auto& s) {
        ret << separator << s;
    });

    return ret.str();
}

std::vector<int> parse_ints(std::string_view _sv) {
    int value;
    std::vector<int> result;
    const auto end = _sv.data() + _sv.size();

    for (auto p = _sv.data(); p < end; ++p) {
        const auto [p2, ec] = std::from_chars(p, end, value);
        if (ec == std::errc()) {
            result.push_back(value);
            p = p2;
        }
    }

    return result;
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
    assert(join(std::vector<std::string>{}, "sep").empty());
    assert(join(std::vector<std::string>{"abcde","01234","q","ppp"}, "*").size() + 1 == sizeof("abcde*01234*q*ppp"));
    assert(join(std::unordered_set<std::string>{"abcde","01234","q","ppp"}, "*").size() + 1 == sizeof("abcde*01234*q*ppp"));

    assert((parse_ints(",23.76,45,,&67,") == std::vector<int>{23,76,45,67}));
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 string_split_join.cpp

*/
