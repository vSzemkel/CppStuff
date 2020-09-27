
#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

// translate expression of the form: ab{1,2}c{mn,op}d
// enumerating tokens in perenthesis

char g_patern[] = "ab{1,2,3}c{mn,op}d";

std::vector<std::string> parse()
{
    bool inBrace{false};
    int len = strlen(g_patern);
    std::vector<std::string> res = {""}, tok_in_paren;

    for (int i = 0; i < len; ++i) {
        const char c = g_patern[i];
        if (c == '{')
            inBrace = true;
        else if (c == '}') {
            inBrace = false;
            std::vector<std::string> tmp;
            for (const auto& s : res)
                for (const auto& t : tok_in_paren)
                    tmp.push_back(s + t);
            res = std::move(tmp);
        } else if (inBrace == false) {
            for (auto& s : res)
                s += c;
        } else { /* inBrace==true */
            const char delim[] = ",}";
            char* beg = g_patern + i;
            char* end = strchr(beg, '}');
            tok_in_paren.clear();
            tok_in_paren.emplace_back(strtok_s(beg, delim, &beg));
            while (beg < end)
                tok_in_paren.emplace_back(strtok_s(nullptr, delim, &beg));

            *end = '}';
            i = end - g_patern - 1;
        }
    }

    return res;
}

int main(int argc, char* argv[])
{
    for (const auto& s : parse())
        std::cout << s << '\n';

    return 0;
}

/* clang++.exe -Wall -g -std=c++17 parenthesis_quest.cpp -o parenthesis_quest.exe

Output:

ab1cmnd
ab1copd
ab2cmnd
ab2copd
ab3cmnd
ab3copd

*/