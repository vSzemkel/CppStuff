/*
ID: marcin.8
LANG: C++
TASK: heritage
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=1hifHvFsbU1&S=heritage
*/

#include <fstream>
#include <string>

std::ifstream task_in("heritage.in");
std::ofstream task_out("heritage.out");

std::string convert(const std::string& ino, const std::string& preo) {
    if (ino.size() < 2) {
        auto ret = preo;
        std::swap(ret[0], ret[1]);
        return preo;
    }

    const char root = preo[0];
    const auto pos = ino.find(root);
    const auto left_nodes = ino.substr(0, pos);
    const auto right_nodes = ino.substr(pos + 1);

    return convert(left_nodes, preo.substr(1, left_nodes.size()))
         + convert(right_nodes, preo.substr(1 + left_nodes.size(), right_nodes.size()))
         + root;

}

int main(int, char**)
{
    std::string ino, preo;
    task_in >> ino >> preo >> std::ws;
    task_out << convert(ino, preo) << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 heritage.cpp -o heritage.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address heritage.cpp -o heritage

Input:

ABEDFCHG
CBADEFGH

Output:

AEFDBHGC

*/
