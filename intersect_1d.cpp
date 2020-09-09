
#include <iostream>
#include <optional>
#include <string>

// find intersection between two sections
using section_t = std::pair<int,int>;

std::optional<section_t> find_intersection(section_t s1, section_t s2, bool normalize = true)
{
    if (normalize) {
        if (s1.first > s2.first)
            std::swap(s1, s2);
        if (s1.first > s1.second)
            std::swap(s1.first, s1.second);
        if (s2.first > s2.second)
            std::swap(s2.first, s2.second);
    }

    std::optional<section_t> ret;
    if (s1.second >= s2.first)
        ret = {std::max(s1.first, s2.first), std::min(s1.second, s2.second)};    

    return ret;
}

std::string print_section(const section_t& s)
{
    char buf[32];
    sprintf_s(buf, sizeof(buf), "[%i, %i]", s.first, s.second);
    return buf;
}

void print_intersection(const section_t& s1, const section_t& s2)
{
    std::string ss1 = print_section(s1);
    std::string ss2 = print_section(s2);
    auto inter = find_intersection(s1, s2);
    if (inter) {
        const auto& val = inter.value();
        if (val.first == val.second)
            std::cout << ss1 << " and " << ss2 << " are connected in " << val.first;
        else
            std::cout << ss1 << " and " << ss2 << " overlap in " << print_section(val);
    } else
        std::cout << ss1 << " and " << ss2 << " do not intersect";
    std::cout << "\n";
}

int main(int argc, char* argv[])
{
    print_intersection({20, 30}, {0, 10});
    print_intersection({20, 30}, {0, 20});
    print_intersection({0, 20}, {10, 30});
    print_intersection({0, 30}, {10, 20});
    return 0;
}

/* clang++.exe -Wall -g -std=c++17 intersect_1d.cpp -o intersect_1d.exe

Output:

[20, 30] and [0, 10] do not intersect
[20, 30] and [0, 20] are connected in 20
[0, 20] and [10, 30] overlap in [10, 20]
[0, 30] and [10, 20] overlap in [10, 20]

*/