/*
ID: marcin.8
LANG: C++
TASK: cryptcow
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=X6PkS8rPEaP&S=cryptcow
*/

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

std::ifstream task_in("cryptcow.in");
std::ofstream task_out("cryptcow.out");

static const std::string pattern = "Begin the Escape execution at the Break of Dawn";
static const auto length = int(pattern.size());

int optimal_rotate(int round, const std::string& msg) {
    if (msg == pattern)
        return round;

    auto t = msg.rfind('W');
    auto c = msg.find('C', 0);

    if (c != std::string::npos && t != std::string::npos
     && std::equal(msg.begin(), msg.begin() + c, pattern.begin())
     && std::equal(msg.begin() + t + 1, msg.end(), pattern.end() - msg.size() + t + 1))
        for (; c != std::string::npos; c = msg.find('C', c + 1))
            for (auto o = msg.find('O', c + 1); o != std::string::npos; o = msg.find('O', o + 1))
                for (auto w = msg.find('W', o + 1); w != std::string::npos; w = msg.find('W', w + 1)) {
                    auto nmsg{msg};
                    nmsg.erase(nmsg.begin() + c);
                    nmsg.erase(nmsg.begin() + o - 1);
                    nmsg.erase(nmsg.begin() + w - 2);
                    std::rotate(nmsg.begin() + c, nmsg.begin() + o - 1, nmsg.begin() + w - 2);
                    const auto res = optimal_rotate(round + 1, nmsg);
                    if (~res)
                        return res;
                }

    return -1;
}

int main(int, char**)
{
    std::string msg;
    std::getline(task_in, msg);

    const auto ans = optimal_rotate(0, msg);
    if (~ans) {
        task_out << "1 " << ans << '\n';
        return 0;
    }

    task_out << "0 0\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 cryptcow.cpp -o cryptcow.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address cryptcow.cpp -o cryptcow

Run:
cryptcow.exe && type cryptcow.out

Input:

CaOBegin the EscWpe CnCak OexeOt the BWcutioWofO aCreW Dawn

Output:

1 4

*/
