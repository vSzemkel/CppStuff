
#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
#include <utility>
#include <vector>

// ATM Queue
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff48/00000000003f4ed8
// Round in which a person can withdraw money is equal to (g_max_amount + amount - 1) / g_max_amount

template <typename T> using vec = std::vector<T>;
template <typename T> void fill(T& cont, const int size){ cont.resize(size), std::copy_n(std::istream_iterator<typename T::value_type>{std::cin}, size, cont.begin()); };

vec<uint64_t> g_input;
int g_size, g_max_amount;

void naive() {
    int served{0};
    std::vector<bool> marked(g_size, false);
    while (served < g_size)
        for (int i = 0; i < g_size; ++i) {
            if (marked[i]) continue;
            if (g_input[i] <= g_max_amount) {
                ++served;
                marked[i] = true;
                std::cout << i + 1 << " ";
            } else
                g_input[i] -= g_max_amount;
        }
}

void solution()
{
    using pair_t = std::pair<uint64_t, int>;
    using cont_t = std::vector<pair_t>;

    cont_t que;
    for (int i = 1; i <= g_size; ++i)
        que.emplace_back((g_max_amount + g_input[i - 1] - 1) / g_max_amount, i);

    std::priority_queue<pair_t, cont_t, std::greater<pair_t>> pq{std::greater<pair_t>{}, que};
    while (!pq.empty()) {
        std::cout << pq.top().second << " ";
        pq.pop();
    }
}

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    // parse console input
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cin >> g_size >> g_max_amount;
        fill(g_input, g_size);
        // Set 2
        std::cout << "Case #" << g << ": ";
        solution();
        std::cout << "\n";
        // Set 1
        //std::cout << "Case #" << g << ": ";
        //naive();
        //std::cout << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 atm_queue.cpp -o atm_queue.exe
atm_queue.exe < atm_queue.in

Input:

3
3 3
2 7 4
5 6
9 10 4 7 2
10 6
9 10 31 1635 4 12 7 2 1 74

Output:

Case #1: 1 3 2 
Case #2: 3 5 1 2 4
Case #3: 5 8 9 1 2 6 7 3 10 4

*/
