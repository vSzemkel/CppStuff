
#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Arithmetic expression target
// Given a set of numbers construct an expression evaluating as close as possible to target value

constexpr const int N = 15, T = 307;
const std::array<int, N> data = {13, 61, 9, 75, 18, 911, 21, 1, 22, 4, 80, 53, 71, 44, 102};

int main(int, char**)
{
    assert(N > 0);
    const int tmpsz = N * 15;
    char tmp[tmpsz];
    const int all = 1 << N;
    std::vector<std::unordered_map<int, std::string>> ans(all); // subset -> {value, expr}

    for (int i = 0; i < N; ++i)
        ans[1 << i][data[i]] = std::to_string(data[i]);
    for (int i = 3; i < all; ++i) {
        if (!ans[i].empty())
            continue;
        for (int l = 1; l < i; ++l)
            if ((i & l) == l) { // l is a proper subset of i
                const int r = i ^ l;
                for (const auto& [vl, el] : ans[l])
                    for (const auto& [vr, er] : ans[r]) {
                        if (vl == T) {
                            std::cout << el << " = " << T << '\n';
                            return 0;
                        }
                        auto& a = ans[i];
                        a[vl] = el;
                        if (vl > vr) { // difference can not be negative
                            sprintf(tmp, "(%s - %s)", el.c_str(), er.c_str());
                            a.emplace(vl - vr, tmp);
                            if (vr > 0 && (vl % vr) == 0) {
                                sprintf(tmp, "%s / %s", el.c_str(), er.c_str());
                                a.emplace(vl / vr, tmp);
                            }
                            sprintf(tmp, "(%s + %s)", el.c_str(), er.c_str());
                            a.emplace(vl + vr, tmp);
                            if (vl * vr < T * T) { // do not collect very big numbers
                                sprintf(tmp, "%s * %s", el.c_str(), er.c_str());
                                a.emplace(vl * vr, tmp);
                            }
                        }
                    }
            }
    }

    int del{1}; // there is no exact solution
    const auto& answer = ans[all - 1];
    while (true) {
        for (int sig : {-1, 1}) {
            const int tg = T + sig * del;
            const auto found = answer.find(tg);
            if (found != answer.end()) {
                std::cout << found->second << " = " << tg << '\n';
                return 1;
            }
        }
        ++del;
    }

    return 2;
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 arith_expr_target.cpp -o arith_expr_target.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address arith_expr_target.cpp -o arith_expr_target

Output:

((75 + 18) * 13 - (911 - 9)) = 307

*/
