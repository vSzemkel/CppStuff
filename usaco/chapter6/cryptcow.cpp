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

int optimal_rotate(int offset, const std::string& msg) {
    while (offset < length && msg[offset] == pattern[offset])
        ++offset;

    if (offset == length)
        return 0;

    int ans{length}, next_m{offset};
    while (true) {
        auto m = msg.find(pattern[offset], next_m);
        if (m == std::string::npos)
            break;
        next_m = m + 1;

        int x{offset}, matched{0};
        while (int(m) < length && msg[m] == pattern[x]) {
            ++m;
            ++x;
            ++matched;
        }

        auto mm = msg;
        std::rotate(mm.begin() + offset, mm.begin() + m - matched, mm.begin() + m);
        ans = std::min(ans, optimal_rotate(offset + matched, mm));
    }

    return ans + 1;
}

int main(int, char**)
{
    std::string msg;
    std::getline(task_in, msg);

    const auto allowed_rotates = std::count(msg.begin(), msg.end(), 'C');
    std::string tmpmsg{msg};
    std::copy_if(msg.begin(), msg.end(), tmpmsg.begin(), [](const char c){ return c != 'C' && c != 'O' && c != 'W'; });
    tmpmsg.erase(tmpmsg.end() - 3 * allowed_rotates, tmpmsg.end());
    msg = std::move(tmpmsg);

    std::string sortedmsg{msg}, sortedpat{pattern};
    std::sort(sortedmsg.begin(), sortedmsg.end());
    std::sort(sortedpat.begin(), sortedpat.end());
    if (sortedmsg == sortedpat && optimal_rotate(0, msg) <= allowed_rotates) {
        task_out << "1 " << allowed_rotates << '\n';
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

Begin the EscCution at the BreOape execWak of Dawn

Output:

1 1

*/
