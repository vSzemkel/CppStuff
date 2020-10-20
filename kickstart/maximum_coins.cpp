
#include <iostream>
#include <vector>

// Maximum Coins
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000001a0069/0000000000414a23

uint64_t maximum_coins(const std::vector<int>& board, const int size)
{
    uint64_t ret{0};

    for (int r = 0; r < size; ++r)
        for (int c = 0; c < size; ++c) {
            if (r > 0 && c > 0) // non negative cells
                continue;

            uint64_t tmp{0};
            int cc{c}, rr{r};
            while (rr < size && cc < size) {
                tmp += board[rr * size + cc];
                ++rr; 
                ++cc;
            }
            if (tmp > ret)
                ret = tmp;
        }

    return ret;
}

int main(int argc, char* argv[])
{
    // parse console input
    int no_of_cases, case_size;
    std::vector<int> input;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cin >> case_size;
        input.resize(case_size * case_size);
        for (auto& c : input) std::cin >> (int&)c;
        std::cout << "Case #" << g << ": " << maximum_coins(input, case_size) << "\n";
    }
}

/* clang++.exe -Wall -g -O0 -std=c++17 maximum_coins.cpp -o maximum_coins.exe
maximum_coins.exe < maximum_coins.in

Input:

2
3
1 2 5
3 6 1
12 2 7
5
0 0 0 0 0
1 1 1 1 0
2 2 2 8 0
1 1 1 0 0
0 0 0 0 0

Output:

Case #1: 14
Case #2: 9

*/