
#include <algorithm>
#include <array>
#include <assert.h>
#include <bitset>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stdlib.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <utility>
#include <vector>

// Retiling
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000435915/00000000007dc2de

const auto big64 = std::numeric_limits<int64_t>::max();
template <typename T> using vec = std::vector<T>;
template <typename T> using vec2 = std::vector<std::vector<T>>;
std::ofstream g_debug("retiling.log");
vec<int64_t> g_input, g_partial;
int64_t g_rows, g_cols, g_size;
vec<bool> g_marked;

const auto col = [](const int64_t pos) noexcept { return pos % g_cols; };
const auto row = [](const int64_t pos) noexcept { return pos / g_cols; };
const auto rc  = [](const int64_t row, const int64_t col) noexcept { return row * g_cols + col; };
template <typename T> std::vector<T> fill(const size_t size){std::vector<T> cont(size);std::copy_n(std::istream_iterator<T>{std::cin},size,cont.begin());return cont;};
template <typename T> void fill2(std::vector<std::vector<T>>& cont, int& rows, int& cols){std::cin>>rows>>cols;cont.resize(rows);for(auto& r:cont) r=fill<T>(cols);};
template <typename T> void fill2sq(std::vector<std::vector<T>>& cont, int& size){std::cin>>size;cont.resize(size);for(auto& r:cont) r=fill<T>(size);};
template <typename K, typename V> std::map<K, V> fillmap(size_t size){std::map<K, V> cont;for(int i=0;i<size;++i){int k,v;std::cin>>k>>v;cont[k]=v;}return cont;};
template <typename T> T init2(const int rows, const int cols){T cont(rows,typename T::value_type(cols));return cont;};
const auto dig_cnt = [](const int64_t n){return int(std::log10(n)) + 1;};
const auto bin_coeff = [](const int n, const int k){return (int)(1/((n+1)*std::beta(n-k+1,k+1)));};
const auto rand_in_range = [](const int64_t ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int64_t> dist(0,ubound-1);return dist(gen);};
size_t next_max(const int off) {return std::lower_bound(g_input.begin()+off+1,g_input.end(),0,[](const auto& n,auto){const auto pred=&n-1;return *pred<=n;})-g_input.begin()-1;}
size_t next_min(const int off) {return std::lower_bound(g_input.begin()+off+1,g_input.end(),0,[](const auto& n,auto){const auto pred=&n-1;return *pred>=n;})-g_input.begin()-1;}
void partial_init(const vec<int64_t>& src){const int size=src.size();g_partial.assign(size,0);for(int i=0;i<size;++i)for(int a=i;a<size;a|=a+1)g_partial[a]+=src[i];}
void partial_update(vec<int64_t>& src, const size_t pos, const int64_t val){const auto size=src.size();for(auto i=pos;i<size;i|=i+1)g_partial[i]+=val-src[pos];src[pos]=val;}
int64_t partial_sum(const int i, const int j){int64_t ret{0};for(int k=j+1;k>0;k&=k-1)ret+=g_partial[k-1];for(int k=i;k>0;k&=k-1)ret-=g_partial[k-1];return ret;}



static void solve() {
    int R, C, F, S; std::cin >> R >> C >> F >> S;
    std::vector<std::string> start(R), final(R);
    for (auto& r : start)
        std::cin >> r;
    for (auto& r : final)
        std::cin >> r;

    int flips{0}, swapsh{0}, swapsv{0}, swapshv{0};
    std::unordered_set<int> tailsh, tailsv;
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            bool flip{}, swaph{}, swapv{};
            if (start[r][c] != final[r][c])
                flip = true;
            if (flip && c < C - 1 && tailsh.count(r * C + c) == 0 && start[r][c] != start[r][c + 1] && final[r][c] != final[r][c + 1]) {
                tailsh.insert(r * C + c + 1);
                swaph = true;
            }
            if (flip && r < R - 1 && tailsv.count(r * C + c) == 0 && start[r][c] != start[r + 1][c] && final[r][c] != final[r + 1][c]) {
                tailsv.insert((r + 1) * C + c);
                swapv = true;
            }
            if (flip) ++flips;
            if (swaph) ++swapsh;
            if (swapv) ++swapsv;
            if (swaph && swapsv) ++swapshv;
        }

    const int swaps = swapsh + swapsv - swapshv;
    //std::cout << std::min(flips * F, swaps * S + (flips - 2 * swaps) * F);
    std::cout << swaps + (flips - 2 * swaps);
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 retiling.cpp -o retiling.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address retiling.cpp -o retiling

Run:
retiling.exe < retiling.in

Input:

3
2 4 1 1
MGMG
MMMG
GMGM
MMMM
3 3 1 1
MGG
GMG
MMM
MMM
MGM
MMG
1 5 1000 1
MGGGG
GGGMM

Output:


*/
