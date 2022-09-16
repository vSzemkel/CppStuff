#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

/**
 * @brief Extracts object attribute names
 * 
 * @param expression Text to be searched
 * @param entities Object names collection
 * @return vector of attribute names
 */
static auto parse_object_attributes(const std::string& expression, const std::vector<const char*>& entities) {
    std::unordered_map<std::string, std::unordered_set<std::string>> ret;
    for (const auto table : entities) {
        auto pos = expression.find(table);
        const auto sz = std::strlen(table);
        while (pos != std::string::npos) {
            if (expression[pos + sz - 1] == '[') {
                const auto dot = expression.find("].", pos + sz);
                if (dot != std::string::npos) {
                    pos = dot - sz + 2;
                }
            }
            if (pos + sz < expression.size()) {
                const auto first = expression.data() + pos + sz;
                auto last = first;
                for (auto c = &*first; std::isalnum(*c) || *c == '_'; ++c) {
                    ++last;
                }
                ret[table].emplace(first, last);
            }
            pos = expression.find(table, pos + sz);
        }
    }

    return ret;
}

/**
 * @brief Result points to top level parentheses content after a functor
 * 
 * @param expression Text to be searched
 * @param functor A token and an openning parenthese
 * @return vector of references to expression substrings (no ownership)
 */
static auto parse_parentheses_content(const std::string& expression, const std::string& functor) {
    assert(!functor.empty() && functor.back() == '(');
    std::vector<std::string_view> ret;
    auto pos = expression.find(functor);
    const auto fnsz = functor.size();
    const auto size = expression.size();
    while (pos != std::string::npos) {
        int level{1};
        auto start = expression.data() + pos + fnsz, end = start;
        while (level && pos < size) {
            if (*end == '(')
                ++level;
            if (*end == ')')
                --level;
            if (level) {
                ++pos;
                ++end;
            }
        }

        const auto len = end - start;
        if (level == 0)
            ret.emplace_back(start, len);

        pos = expression.find(functor, pos + fnsz + len);
    }

    return ret;
}

int main(int, char**)
{
    std::string expression{"dupa.srupa == 5 and (dupa[2].blada < 42 || object.hasProperty)"};
    auto mapping = parse_object_attributes(expression, {"dupa.", "dupa[", "object."});
    assert(mapping.size() == 3);
    assert(mapping["dupa."].size() == 1);
    assert(mapping["dupa["].size() == 1);
    assert(mapping["object."].size() == 1);
    assert(*mapping["object."].begin() == "hasProperty");

    assert(parse_parentheses_content("------- fun(765)+++++++", "fun(").back() == "765");
    assert(parse_parentheses_content("------- fun(765)++++fun( baba() + 1)+++", "fun(").back() == " baba() + 1");
    std::cout << "PASSED\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 string_parse_helpers.cpp -o string_parse_helpers.exe

*/