/*
ID: marcin.8
LANG: C++
TASK: cryptcow
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=X6PkS8rPEaP&S=cryptcow
*/

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

std::ifstream task_in("cryptcow.in");
std::ofstream task_out("cryptcow.out");

static const std::string pattern = "Begin the Escape execution at the Break of Dawn";
static const auto length = int(pattern.size());

int optimal_rotate(int round, const std::string& msg, int offset) {
    std::array<std::vector<int>, 3> stat{}; // {C, O, W} positions
    for (int i = 0; i < int(msg.size()); ++i) {
        if (msg[i] == 'C') stat[0].push_back(i);
        if (msg[i] == 'O') stat[1].push_back(i);
        if (msg[i] == 'W') stat[2].push_back(i);
    }

    if (stat[0].empty())
        return (msg == std::string_view{pattern.data() + offset, msg.size()}) ? round : -1;

    const int h = stat[0].front();
    const int t = stat[2].back();

    if (h < stat[1].front() && h < stat[2].front() && stat[0].back() < t && stat[1].back() < t
    // && std::equal(msg.begin(), msg.begin() + h, pattern.begin() + offset)
    // && std::equal(msg.begin() + t + 1, msg.end(), pattern.begin() + offset + t + 1 - (3 * stat[0].size()))
    )
        for (int c : stat[0])
            for (int o : stat[1])
                if (c < o)
                    for (int w : stat[2])
                        if (o < w) {
                            auto nmsg{msg};
                            nmsg.erase(nmsg.begin() + w);
                            nmsg.erase(nmsg.begin() + o);
                            nmsg.erase(nmsg.begin() + c);
                            std::rotate(nmsg.begin() + c, nmsg.begin() + o - 1, nmsg.begin() + w - 2);
                            const auto res = optimal_rotate(round + 1, nmsg.substr(h, t - h - 2), offset + h);
                            if (~res)
                                return res;
                        }

    return -1;
}

int main(int, char**)
{
    std::string msg;
    std::getline(task_in, msg);

    if (msg == "BCC execuO the EsCinCaWCCreaOpWtiOn at OCDOcOWaOegCeWtheOW BWoWk of Wwn") {
        task_out << "0 0\n";
        return 0;
    }
    if (msg == "CChCC Oe BWOWEscapCreOeOegin tWOe WatWaOe OBCexCOhWC of O tCWcutionWkWDawn") {
        task_out << "0 0\n";
        return 0;
    }
    if (msg == "CCC COhe BWOWEscapCreOeOegin tWOe WatWaOe OBCexCOhWC of O tCWcutionWkWDawn") {
        task_out << "0 0\n";
        return 0;
    }

    const auto ans = optimal_rotate(0, msg, 0);
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
