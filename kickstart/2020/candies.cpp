
#include <iostream>
#include <vector>

// Candies
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ff43/0000000000337b4d#problem

void partial_init(const std::vector<int64_t>& src, std::vector<int64_t>& dst){const int size=src.size();dst.assign(size,0);for(int i=0;i<size;++i)for(int a=i;a<size;a|=a+1)dst[a]+=src[i];}
void partial_update(std::vector<int64_t>& src, std::vector<int64_t>& dst, const size_t pos, const int64_t val){const auto size=src.size();for(auto i=pos;i<size;i|=i+1)dst[i]+=val-src[pos];src[pos]=val;}
int64_t partial_sum(const std::vector<int64_t>& src, const int i, const int j){int64_t ret{0};for(int k=j+1;k>0;k&=k-1)ret+=src[k-1];for(int k=i;k>0;k&=k-1)ret-=src[k-1];return ret;}

static void solve() {
    int parity{0}, N, Q; std::cin >> N >> Q;
    std::vector<int64_t> candies(N), mcandies(N), prefsum, mprefsum;
    for (auto& c : candies) {
        std::cin >> c;
        if (parity++ & 1)
            c *= -1;
        mcandies[parity - 1] = c * parity;
    }

    partial_init(candies, prefsum);
    partial_init(mcandies, mprefsum);

    int64_t ret{0};
    for (int i = 0; i < Q; ++i) {
        char op; int ind, val;
        std::cin >> op >> ind >> val;
        const auto factor = (ind & 1) == 0 ? -1 : 1;
        if (op == 'U') {
            const auto mval = val * ind--;
            partial_update(candies, prefsum, ind, val * factor);
            partial_update(mcandies, mprefsum, ind, mval * factor);
        } else {
            --ind; --val;
            ret += factor * (partial_sum(mprefsum, ind, val) - ind * partial_sum(prefsum, ind, val));
        }
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 candies.cpp -o candies.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address candies.cpp -o candies

Run:
candies.exe < candies.in

Input:

2
5 4
1 3 9 8 2
Q 2 4
Q 5 5
U 2 10
Q 1 2
3 3
4 5 5
U 1 2
U 1 7
Q 1 2

Output:

Case #1: -8
Case #2: -3

*/
