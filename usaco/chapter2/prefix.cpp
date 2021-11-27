/*
ID: marcin.8
LANG: C++
TASK: prefix
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=JKjnBvvEhYI&S=prefix
*/

#include <fstream>
#include <queue>
#include <string>
#include <string.h>
#include <vector>

std::ifstream task_in("prefix.in");
std::ofstream task_out("prefix.out");

std::vector<std::string> primitives;
std::string pattern;
std::vector<bool> dp;

/**
 * @brief This cann be improved and work in constant time with polynomial hasher
 * 
 * @param ep Index of the position after the last character
 * @param text Sufix to search for
 * @return true if sufix matches pattern[ep-sz..ep)
 */
static bool match_tail(const int ep, const std::string& text) {
    const int tsz = int(text.size());
    if (ep < tsz)
        return false;
    return !strncmp(pattern.data() + ep - tsz, text.data(), tsz);
}

/**
 * @brief Queue lengths of identified valid prefixes. Pop and search for augmentation
 * 
 * @return int Maximal valid prefix length
 */
static int solve() {
    const int len = int(pattern.size());
    std::vector<bool> visited(len + 1, false);
    visited[0] = true;
    std::queue<int> qq;
    qq.push(0);
    while (!qq.empty()) {
        const int preflen = qq.front(); qq.pop();
        for (const auto& p : primitives) {
            const int next = preflen + int(p.size());
            if (next <= len && !visited[next] && match_tail(next, p)) {
                visited[next] = true;
                qq.push(next);
            }
        }
    }

    for (int i = len; i > 0; --i)
        if (visited[i])
            return i;

    return 0;
}

/**
 * @brief It works but this problem can be solved faster
 */
static bool WTS(const int ep) {
    if (dp[ep])
        return true;

    for (const auto& p : primitives)
        if (match_tail(ep, p)) {
            const int next = ep - int(p.size());
            if (WTS(next)) {
                dp[ep] = true;
                return true;
            }
        }

    return false;
}

int main(int, char**)
{
    std::string p;
    while (true) {
        task_in >> p;
        if (p == ".") break;
        primitives.push_back(p);
    }
    while (std::getline(task_in, p))
        pattern += p;

    const int len = int(pattern.size());
    dp.assign(len + 1, false);
    dp[0] = true;
    task_out << solve() << '\n';


    /*for (int i = len; i >= 0; --i) {
        if (working_too_slow(i)) {
        if (dp[i]) {
            task_out << i << '\n';
            return 0;
        }
    }*/
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 prefix.cpp -o prefix.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address prefix.cpp -o prefix

Input:

AH AS AZ BW CD CK CN CU CZ DB DH EC ED EN FJ GA GK GM GS GT 
HA HN HZ IN IR JB JD JM JZ KG KI LO LQ LU LW LY MJ MT MV ND 
NM NS OB OI OK OM PG PO PQ PZ QE QP RG RK RN RP RQ RR RS RU 
SA SF SJ SN TK TR TU TY UA UO US UW UX VH VL VO WF WH WL WS 
WZ XU XW XY YA YI YN YT ZF ZH ZJ ZL ZR ZX
.
ASCDCKCUEDFJGKGMHAIRJMKILQLWLYMJMTMVNSOIOKOMPOPQQPRGRNRPSJTU
TYWSWZXWYIYTAHAZBWCDCNCUDHECENGAGMGSGTHAHNKGLQLULWNMOBOIOKOM
PGPOPQPZRGRKRNRPRRSATRTUUWVLVOWFWLWSXUXYYAYIYNYTZFZJZRZXBWCK
CUFJGAGSHAHNIRJBJMKILQLUNDNMNSOKOMPQQERGRQRRRSSASFSJSNTUTYUA
UOUWVLWHXUXWXYYAYTZFZJZLZRZXASBWCDCNDBECEDFJGSHAHNHZINJMKGKI
LQMJNMNSOKOMPQQPRRRSSFSJSNTRTUTYUAUWUXVLVOWLWSWZXYYNYTZFZHZR
ZXAHASAZCDCKCNDBECENGMGSHAHZIRJDKGKILOLYOBOKPGPQQPRRSNTRTYUO
USUXVHVLVOWFWLWSXYYNZJZXZZAHAZBWCDCKCNDB

Output:

444

*/
