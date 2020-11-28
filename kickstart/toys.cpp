
#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <utility>
#include <vector>

// Toys
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff47/00000000003bede9

template <typename T> using vec = std::vector<T>;

std::vector<int64_t> g_partial;
void partial_init(const vec<int64_t>& src){const int size=src.size();g_partial.assign(size,0);for(int i=0;i<size;++i)for(int a=i;a<size;a|=a+1)g_partial[a]+=src[i];}
void partial_update(vec<int64_t>& src, const size_t pos, const int64_t val){const auto size=src.size();for(auto i=pos;i<size;i|=i+1)g_partial[i]+=val-src[pos];src[pos]=val;}
int64_t partial_sum(const int i, const int j){int64_t ret{0};for(int k=j+1;k>0;k&=k-1)ret+=g_partial[k-1];for(int k=i;k>0;k&=k-1)ret-=g_partial[k-1];return ret;}

void solve() {
    size_t size;
    std::cin >> size;
    std::vector<int64_t> enjoyment(size);
    std::vector<std::pair<int64_t,int>> remembrance;
    std::priority_queue<int, std::vector<int>, std::greater<int>> bad_toys;
    for (int i = 0; i < size; ++i) {
        int64_t ri;
        int64_t& ei = enjoyment[i];
        std::cin >> ei >> ri;
        remembrance.emplace_back(ei + ri, i);
    }
    partial_init(enjoyment);
    std::sort(remembrance.begin(), remembrance.end());

    size_t toys{size};
    int64_t first_round_time = std::accumulate(enjoyment.begin(), enjoyment.end(), 0LL);
    int64_t longest_play{first_round_time}, best_remove_count{0};
    while (toys > 1) {
        while (!remembrance.empty() && remembrance.back().first > first_round_time) {
            bad_toys.push(remembrance.back().second);
            remembrance.pop_back();
        }

        if (bad_toys.empty()) {
            std::cout << " " << size - toys << " INDEFINITELY";
            return;
        }

        const auto toy_to_remove = bad_toys.top();
        bad_toys.pop();
        int64_t ca = partial_sum(0, toy_to_remove - 1);
        if (first_round_time + ca > longest_play) {
            longest_play = first_round_time + ca;
            best_remove_count = size - toys;
        }

        first_round_time -= enjoyment[toy_to_remove];
        partial_update(enjoyment, toy_to_remove, 0);
        --toys;
    }

    std::cout << " " << best_remove_count << " " << longest_play;
}

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ":"; solve(); std::cout << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 toys.cpp -o toys.exe
g++ -Wall -ggdb3 -Og -std=c++17 toys.cpp -o toys.o
toys.exe < toys.in

Input:

5
4
10 10
550 1000
10 11
500 1000
1
5 1
2
5 10
10 3
3
30 17
5 10
10 3
3
5 10
5 10
5 11

Output:

Case #1: 0 1080
Case #2: 0 5
Case #3: 0 INDEFINITELY
Case #4: 1 INDEFINITELY
Case #5: 0 25

*/
