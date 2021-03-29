
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

// Median Sort
// https://codingcompetitions.withgoogle.com/codejam/round/000000000043580a/00000000006d1284

std::ofstream g_debug("median_sort.log");
int N, Q;

static void print(const std::vector<int>& v)
{
    for (const int val : v)
        std::cout << val << " ";
    std::cout << std::endl;
}

static void print_debug(const std::vector<int>& v)
{
    for (const int val : v)
        g_debug << val << " ";
    g_debug << std::endl;
}

static void solve() {
    std::vector<int> data = {1, 2};
    data.reserve(N);

    int answer;
    for (int i = 3; i <= N; ++i) {
        int lpos, rpos, lo{0}, hi{i - 1};
        while (lo + 1 < hi) {
            const int width = hi - lo;
            lpos = std::max(0, lo + width / 3);
            rpos = lo + 2 * width / 3;
            if (lpos == rpos) ++rpos;
            std::cout << i << " " << data[lpos] << " " << data[rpos] << std::endl;
            std::cin >> answer;
            print_debug(data);
            g_debug << "Q: " << "[" << i << "] " << data[lpos] << " " << data[rpos] << std::endl;
            g_debug << "lpos == " << lpos << " rpos == " << rpos << " answer: " << answer << std::endl;
            if (answer == -1)
                exit(0);
            if (answer == i) {
                lo = lpos;
                hi = rpos;
                g_debug << "V0 lpos,rpos,lo,hi " << lpos << " " << rpos << " " << lo << " " << hi << std::endl;
            } else if (answer == data[lpos]) {
                hi = lpos;
                if (lo == 0 && hi == 1) --lo;
                g_debug << "V1 lpos,rpos,lo,hi " << lpos << " " << rpos << " " << lo << " " << hi << std::endl;
            } else {
                lo = rpos;
                g_debug << "V2 lpos,rpos,lo,hi " << lpos << " " << rpos << " " << lo << " " << hi << std::endl;
            }
        }

        data.insert(data.begin() + hi, i);
    }

    print(data);
    g_debug << "Answer: "; print_debug(data);
    std::cin >> answer;
    if (answer == -1) exit(0);
}

static void solve_set2() {
    std::vector<int> data = {1, 2, 3};
    data.reserve(N);
    std::cout << "1 2 3" << std::endl;
    int answer; std::cin >> answer;
    std::rotate(data.begin(), data.begin() + (answer + 1) % 3, data.end());

    for (int i = 4; i <= N; ++i) {
        int lo{0}, hi{i - 1}, pos = (i - 1) / 2;
        g_debug << "For i == " << i << " initial pos == " << pos << std::endl;
        while (0 <= pos && pos < i - 2) {
            std::cout << i << " " << data[pos] << " " << data[pos + 1] << std::endl;
            std::cin >> answer;
            print_debug(data);
            g_debug << "Q: " << i << " " << data[pos] << " " << data[pos + 1] << std::endl;
            g_debug << "pos == " << pos << " answer: " << answer << std::endl;
            if (answer == -1)
                exit(0);
            if (answer == i) {
                data.insert(data.begin() + pos + 1, i);
                break;
            }
            if (answer == data[pos]) {
                g_debug << "V1 pos,lo,hi " << pos << " " << lo << " " << hi << " => ";
                pos -= std::max(1, (pos - lo) / 2);
                g_debug << pos << std::endl;
                hi = pos;
            } else {
                g_debug << "V2 pos,lo,hi " << pos << " " << lo << " " << hi << " => ";
                pos += std::max(1, (hi - pos) / 2);
                lo = pos;
                g_debug << pos << std::endl;
            }
        }
        if ((int)data.size() < i) {
            g_debug << "Final pos, lo, hi " << pos << " " << lo << " " << hi << std::endl;
            if (pos < 0)
                data.insert(data.begin(), i);
            else
                data.push_back(i);
        }
    }

    print(data);
    g_debug << "Answer: "; print_debug(data);
    std::cin >> answer;
    if (answer == -1) exit(0);
}

static void solve_set1() {
    std::vector<int> data = {1, 2, 3};
    data.reserve(N);
    std::cout << "1 2 3" << std::endl;
    int answer; std::cin >> answer;
    std::rotate(data.begin(), data.begin() + (answer + 1) % 3, data.end());
    print_debug(data);

    for (int i = 4; i <= N; ++i) {
        for (int pos = 0; pos < i - 2; ++pos) {
            std::cout << i << " " << data[pos] << " " << data[pos + 1] << std::endl;
            std::cin >> answer;
            if (answer == -1)
                exit(0);
            if (answer == data[pos]) {
                data.insert(data.begin() + pos, i);
                break;
            }
            if (answer == i) {
                data.insert(data.begin() + pos + 1, i);
                break;
            }
        }
        if ((int)data.size() < i)
            data.push_back(i);
    }

    print(data);
    std::cin >> answer;
    if (answer == -1) exit(0);
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases >> N >> Q;
    g_debug << N << " numbers, " << Q << " queries" << std::endl;
    for (int g = 1; g <= no_of_cases; ++g) {
        solve();
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 median_sort.cpp -o median_sort.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address median_sort.cpp -o median_sort.o

Run:
py.exe interactive_runner.py py.exe median_sort_testing_tool.py 1 -- median_sort.exe

*/
