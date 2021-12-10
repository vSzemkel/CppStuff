/*
ID: marcin.8
LANG: C++
TASK: zerosum
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=YhUGeW3rpbq&S=zerosum
*/

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::ifstream task_in("zerosum.in");
std::ofstream task_out("zerosum.out");

int N;

const char* symbols = "+- ";

enum class op {
    plus = 0,
    minus = 1,
    concat = 2
};

static int do_op(const int first, const op oper, const int second) {
    switch (oper) {
        case op::plus:
            return first + second;
            break;
        case op::minus:
            return first - second;
            break;
        case op::concat:
            return first * 10 + second;
    }
}

static std::string decode_opseq(int os) {
    std::string ret = "1";
    for (int d = 2; d <= N; ++d) {
        const auto next_op = op(os % 3);
        os /= 3;
        ret.push_back(symbols[int(next_op)]);
        ret.push_back('0' + d);
    }

    return ret;
}

int main(int, char**)
{
    task_in >> N;

    int counter{1};
    for (int z = N - 1; z; --z)
        counter *= 3;

    std::vector<std::string> result;
    for (int i = 0; i < counter; ++i) {
        op operation{op::plus};
        int first{0}, second{1}, opseq{i};
        for (int d = 2; d <= N; ++d) {
            const auto next_op = op(opseq % 3);
            opseq /= 3;
            if (next_op == op::concat)
                second = do_op(second, next_op, d);
            else {
                first = do_op(first, operation, second);
                operation = next_op;
                second = d;
            }
        }

        if (do_op(first, operation, second) == 0)
            result.push_back(decode_opseq(i));
    }

    std::sort(result.begin(), result.end());
    for (const auto& r : result)
        task_out << r << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 zerosum.cpp -o zerosum.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address zerosum.cpp -o zerosum

Input:

6

Output:

1_2+3-4-5-6

*/
