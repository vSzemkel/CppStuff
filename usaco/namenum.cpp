/*
ID: marcin.8
LANG: C++
TASK: namenum
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=DZQhOcp1zgs&S=namenum
*/

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

std::ifstream task_in("namenum.in");
std::ofstream task_out("namenum.out");

int main(int, char**)
{
    std::unordered_map<char, char> mapping = {
        {'A', '2'}, {'B', '2'}, {'C', '2'},
        {'D', '3'}, {'E', '3'}, {'F', '3'},
        {'G', '4'}, {'H', '4'}, {'I', '4'},
        {'J', '5'}, {'K', '5'}, {'L', '5'},
        {'M', '6'}, {'N', '6'}, {'O', '6'},
        {'P', '7'}, {'R', '7'}, {'S', '7'},
        {'T', '8'}, {'U', '8'}, {'V', '8'},
        {'W', '9'}, {'X', '9'}, {'Y', '9'}
    };

    std::string number; task_in >> number;
    std::ifstream dict_in("dict.txt");
    while (dict_in.good()) {
        std::string tr, n; dict_in >> n;
        std::transform(n.begin(), n.end(), std::back_inserter(tr), [&mapping](const char c){ return mapping[c]; });
        if (tr == number)
            task_out << n << "\n";
    }

    if (task_out.tellp() == 0)
        task_out << "NONE\n";
}

void not_so_smart()
{
    std::unordered_map<char, std::vector<char>> mapping = {
        {'2', {'A', 'B', 'C'}},
        {'3', {'D', 'E', 'F'}},
        {'4', {'G', 'H', 'I'}},
        {'5', {'J', 'K', 'L'}},
        {'6', {'M', 'N', 'O'}},
        {'7', {'P', 'R', 'S'}},
        {'8', {'T', 'U', 'V'}},
        {'9', {'W', 'X', 'Y'}}
    };

    std::vector<std::string> dict;
    std::string number; task_in >> number;
    std::ifstream dict_in("dict.txt");
    while (dict_in.good()) {
        std::string n; dict_in >> n;
        dict.push_back(std::move(n));
    }

    std::vector<std::string> candidates(1);
    for (const char c : number) {
        std::vector<std::string> new_ones;
        for (const char l : mapping[c])
            for (auto& name : candidates) {
                const auto prefix = name + l;
                const auto it = std::lower_bound(dict.begin(), dict.end(), prefix);
                if (it != dict.end() && it->find(prefix) == 0)
                    new_ones.push_back(prefix);
            }
        candidates = new_ones;
    }

    std::vector<std::string> ret;
    std::sort(candidates.begin(), candidates.end());
    std::set_intersection(candidates.begin(), candidates.end(), dict.begin(), dict.end(), std::back_inserter(ret));
    if (ret.empty())
        ret.push_back("NONE");
    for (const auto& s : ret)
        task_out << s << "\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 namenum.cpp -o namenum.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 namenum.cpp -o namenum.o

Input:

4734

Output:

GREG

*/
