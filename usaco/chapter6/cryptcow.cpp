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
static const int length = int(pattern.size());

int main(int, char**)
{
    std::string msg;
    std::getline(task_in, msg);

    msg.erase(msg.begin() + msg.find('C'));
    msg.erase(msg.begin() + msg.find('O'));
    msg.erase(msg.begin() + msg.find('W'));

    int pos{-1};
    std::unordered_map<char, std::vector<int>> patstat;
    for (const auto c : msg)
        patstat[c].push_back(++pos);
    pos = -1;
    std::unordered_map<char, std::vector<int>> msgstat;
    for (const auto c : msg)
        msgstat[c].push_back(++pos);

    if (msgstat == patstat) {
        int count{}, x{};
        while (true) {
            int bestl{}, bestx{};
            const auto last_x{x};
            for (auto m : msgstat[pattern[last_x]])
                if (last_x <= m) {
                    int matched{0};
                    x = last_x;
                    while (m < length && msg[m] == pattern[x]) {
                        ++m;
                        ++x;
                        ++matched;
                    }
                    if (bestl < matched) {
                        bestl = matched;
                        bestx = m - matched;
                    }
                }

            if (bestl) {
                if (last_x < bestx) {
                    std::rotate(msg.begin() + last_x, msg.begin() + bestx, msg.begin() + bestx + bestl);
                    ++count;
                }
                x = bestx + bestl;
            } else
                break;
        }

        task_out << "1 " << count << '\n';
    } else
        task_out << "0 0\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 cryptcow.cpp -o cryptcow.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address cryptcow.cpp -o cryptcow

Run:
cryptcow.exe && type cryptcow.out

Input:

Begin the EscCution at the BreOape execWak of Dawn

Output:

1 1

*/
