/*
ID: marcin.8
LANG: C++
TASK: twofive
KUDOS: NerdMomentwDavid
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=FIktgbZZPYQ&S=twofive
*/

#include <fstream>
#include <string>

std::ifstream task_in("twofive.in");
std::ofstream task_out("twofive.out");

int cir[6]; // filled_columns_in_row[r] == c iff in row r left c columns is already filled
int dp[6][6][6][6][6]; // dp[a][b][c][d][e] tells how many ways there is to finish corret grid if we hawe a column filled in first row, b column filled in second row etc
std::string tmp(25, ' ');

int countPos(char l) {
    auto& cur = dp[cir[1]][cir[2]][cir[3]][cir[4]][cir[5]];
    if (cur)
        return cur;
    int cnt = 0;
    for (int r = 1; r <= 5; r++) {
        if (cir[r] < cir[r - 1]) {
            const auto cell = cir[r] + ((r - 1) * 5);
            if (tmp[cell] == l || tmp[cell] == ' ') {
                ++cir[r];
                cnt += countPos(l + 1);
                --cir[r];
            }
        }
    }
    
    return cur = cnt;
}

void reset() {
    ::memset(cir, 0, sizeof(cir));
    ::memset(dp, 0, sizeof(dp));
    cir[0] = 5;
    dp[5][5][5][5][5] = 1;
}

int main() {
    char c;
    task_in >> c;
    if (c == 'W') {
        std::string s;
        task_in >> s;
        int ans = 0;
        for (int i = 0; i < 25; ++i) {
            for (char ch = 'A'; ch < s[i]; ++ch) {
                reset();
                tmp[i] = ch;
                ans += countPos('A');
            }
            tmp[i] = s[i];
        }
        task_out << ans + 1 << '\n';
    } else {
        int x;
        task_in >> x;
        int acc{};
        for (int i = 0; i < 25; ++i) {
            for (char ch = 'A'; ch <= 'Y'; ++ch) {
                reset();
                tmp[i] = ch;
                const int y = countPos('A');
                if (acc + y < x) acc += y;
                else break;
            }
        }
        task_out << tmp << '\n';
    }
    
    return 0;
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++20 twofive.cpp -o twofive.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address twofive.cpp -o twofive

Run:
twofive.exe && type twofive.out

Input:

N
701149020
W
AFKPUBGLQVCHMRWDINSXEJOTY

Output:

AFKPUBGLQVCHMRWDINSXEJOTY
701149020

*/