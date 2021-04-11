
#include <cmath>
#include <iostream>
#include <vector>

// Append Sort
// https://codingcompetitions.withgoogle.com/codejam/round/000000000043585d/00000000007549e5


static std::string add_one(const std::string& num)
{
    auto ret = num;
    for (int d = (int)ret.size() - 1; d >= 0; --d)
        if (ret[d] < '9') {
            ++ret[d];
            return ret;
        } else 
            ret[d] = '0';

    ret.insert(ret.begin(), '1');
    return ret;
}

static void solve() {
    int N; std::cin >> N;
    std::vector<std::string> numbers(N);
    for (int i = 0; i < N; ++i)
        std::cin >> numbers[i];

    int ret{0};
    for (int i = 1; i < N; ++i) {
        auto& num = numbers[i];
        const auto& prev = numbers[i - 1];
        const auto rdigits = (int)num.size();
        const auto ldigits = (int)prev.size();
        if (ldigits < rdigits || (ldigits == rdigits && prev < num))
            continue;
        const auto digdiff = ldigits - rdigits;
        const auto bound = num + std::string(digdiff, '9');
        num.append(std::string(digdiff, '0'));
        if (prev >= num) {
            if (bound > prev)
                num = add_one(prev);
            else
                num.append("0");
        }
        ret += num.size() - rdigits;
    }

    std::cout << ret;
}

static void overflowing() {
    int N; std::cin >> N;
    std::vector<int64_t> numbers(N);
    for (int i = 0; i < N; ++i)
        std::cin >> numbers[i];

    int ret{0};
    for (int i = 1; i < N; ++i) {
        if (numbers[i - 1] < numbers[i])
            continue;
        const auto ldigits = (int)std::floor(log10(numbers[i - 1])) + 1;
        const auto rdigits = (int)std::floor(log10(numbers[i])) + 1;
        const auto digdiff = ldigits - rdigits;
        const auto bound = (numbers[i] + 1) * (int64_t)pow(10, digdiff) - 1;
        numbers[i] *= (int64_t)pow(10, digdiff);
        if (numbers[i - 1] >= numbers[i]) {
            if (bound > numbers[i - 1])
                numbers[i] = numbers[i - 1] + 1;
            else
                numbers[i] *= 10;
        }
        ret += (int)std::floor(log10(numbers[i])) + 1 - rdigits;
    }

    std::cout << ret;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 append_sort.cpp -o append_sort.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address append_sort.cpp -o append_sort.o

Run:
py.exe interactive_runner.py py.exe append_sort_testing_tool.py 1 -- append_sort.exe
append_sort.exe < append_sort.in

Input:

4
3
100 7 10
2
10 10
3
4 19 1
3
1 2 3

Output:

Case #1: 4
Case #2: 1
Case #3: 2
Case #4: 0

*/
