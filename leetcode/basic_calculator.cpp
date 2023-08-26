
#include <cassert>
#include <iomanip>
#include <string>
#include <vector>

// Basic Calculator
// https://leetcode.com/problems/basic-calculator/

static int solve(const std::string& S) {
    int64_t ans{}, sign{1};
    const auto sz = int(S.size());
    std::vector<int> stack;
    for (int i = 0; i < sz; ++i) {
        const char d = S[i];
        if (::isdigit(d)) {
            int num = S[i++] - '0';
            while (i < sz && ::isdigit(S[i]))
                num = 10 * num - '0' + S[i++];
            --i;
            ans = ans + sign * num;
            sign = 1;
        } else if (d == '-')
            sign = -sign;
        else if (d == '(') {
            stack.push_back(ans);
            stack.push_back(sign);
            ans = 0;
            sign = 1;
        } else if (d == ')') {
            const int prevSign = stack.back();
            stack.pop_back();
            const int prevAns = stack.back();
            stack.pop_back();

            ans = prevAns + prevSign * ans;
        }
    }

    assert(stack.empty());
    return ans;
}

static void io_handler() {
    std::string S;
    std::getline(std::cin, S);
    assert(!S.empty());

    std::cout << solve(S);
}

int main(int, char**)
{
    int no_of_cases;
    std::cin >> no_of_cases;
    std::string _;
    std::getline(std::cin, _); // eat up end of number of cases line
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; io_handler(); std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 basic_calculator.cpp -o basic_calculator.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address basic_calculator.cpp -o basic_calculator

Run:
basic_calculator.exe < basic_calculator.in

Input:

4
2 + 3
-2 + 3
-2 + (-3)
2 + (-3)

Output:

Case #1: 5
Case #2: 1
Case #3: -5
Case #4: -1

*/
