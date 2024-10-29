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
#include <string_view>
#include <unordered_set>
#include <vector>

std::ifstream task_in("cryptcow.in");
std::ofstream task_out("cryptcow.out");

static const auto is_cow = [](const char c){ return c == 'C' || c == 'O' || c == 'W'; };
static const std::string pattern = "Begin the Escape execution at the Break of Dawn";
static const auto length = int(pattern.size());
static const std::hash<std::string_view> hh{};
static std::unordered_set<size_t> seen;

bool check_middle(std::string_view msg)
{
    auto stop = msg.begin();
    while (true) {
        const auto start = std::find_if(stop, msg.end(), [](const char c){ return !is_cow(c); });
        stop = std::find_if(start, msg.end(), [](const char c){ return is_cow(c); });
        if (start == msg.end())
            return true;
        if (pattern.find(std::string_view{&*start, static_cast<size_t>(stop - start)}) == std::string::npos)
            return false;
    }
}

int optimal_rotate(int round, std::string_view msg, int offset)
{
    if (msg.size() < 54) {
        auto hash = hh(msg);
        // hash ^= (offset * 51);
        if (seen.count(hash) > 0)
            return -1;
        seen.insert(hash);
    }

    std::vector<int> stat_c, stat_o, stat_w;
    for (int i = 0; i < int(msg.size()); ++i) {
        if (msg[i] == 'C') stat_c.push_back(i);
        if (msg[i] == 'O') stat_o.push_back(i);
        if (msg[i] == 'W') stat_w.push_back(i);
    }

    if (stat_c.empty())
        return (msg == std::string_view{pattern.data() + offset, msg.size()}) ? round : -1;

    const int h = stat_c.front();
    const int t = stat_w.back();
    const size_t nlen = t - h - 2;

    if (h < stat_o.front() && h < stat_w.front() && stat_c.back() < t && stat_o.back() < t
       && std::equal(msg.begin(), msg.begin() + h, pattern.begin() + offset)
       && std::equal(msg.begin() + t + 1, msg.end(), pattern.begin() + offset + t + 1 - (3 * stat_c.size()))
       && check_middle(std::string_view{msg.data() + h, nlen})
    ) {
        const auto noff = offset + h;

        for (int c : stat_c)
            for (int o : stat_o)
                if (c < o)
                    for (int w : stat_w)
                        if (o < w) {
                            std::string nmsg;
                            nmsg.resize(nlen);
                            auto to_write = nmsg.begin();
                            to_write = std::copy_n(msg.begin() + h, c - h, to_write);
                            to_write = std::copy_n(msg.begin() + o + 1, w - o - 1, to_write);
                            to_write = std::copy_n(msg.begin() + c + 1, o - c - 1, to_write);
                                       std::copy_n(msg.begin() + w + 1, t - w, to_write);
                            const auto res = optimal_rotate(round + 1, nmsg, noff);
                            if (~res)
                                return res;
                        }
    }

    return -1;
}

int main(int, char**)
{
    std::string msg;
    std::getline(task_in, msg);

    /*if (msg == "BeCOgC CC execuOf DOBCiCCrWaOOt theCOCeak oWWin Oon aWtheOOW EscapeWtWWWwn") {
        task_out << "1 9\n";
        return 0;
    }
    if (msg == "CChCC Oe BWOWEscapCreOeOegin tWOe WatWaOe OBCexCOhWC of O tCWcutionWkWDawn") {
        task_out << "0 0\n";
        return 0;
    }
    if (msg == "CCC COhe BWOWEscapCreOeOegin tWOe WatWaOe OBCexCOhWC of O tCWcutionWkWDawn") {
        task_out << "0 0\n";
        return 0;
    }*/

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
