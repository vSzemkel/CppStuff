
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

// $TASKNAME$
// 

using vi_t = std::vector<int>;
using vl_t = std::vector<int64_t>;
using vii_t = std::vector<std::vector<int>>;
using vll_t = std::vector<std::vector<int64_t>>;
using min_pq_t = std::priority_queue<int, std::vector<int>, std::greater<>>;

vl_t g_input, g_partial;
std::vector<bool> g_marked;
int64_t g_rows, g_cols, g_size;
std::ofstream g_debug("$TASKNAME$.log");
constexpr const int INF = 1e09, EPS = 1e-09;

const auto col = [](const int64_t pos) noexcept { return pos % g_cols; };
const auto row = [](const int64_t pos) noexcept { return pos / g_cols; };
const auto rc  = [](const int64_t row, const int64_t col) noexcept { return row * g_cols + col; };
const auto bits = [](const int n){ return n == 0 ? 0 : 31 - __builtin_clz(n); };
const auto dig_cnt = [](const int64_t n){return int(std::log10(n)) + 1;};
const auto bin_coeff = [](const int n, const int k){return (int)(1/((n+1)*std::beta(n-k+1,k+1)));};
const auto rand_in_range = [](const int64_t ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int64_t> dist(0,ubound-1);return dist(gen);};
size_t next_max(const int off) {return std::lower_bound(g_input.begin()+off+1,g_input.end(),0,[](const auto& n,auto){const auto pred=&n-1;return *pred<=n;})-g_input.begin()-1;}
size_t next_min(const int off) {return std::lower_bound(g_input.begin()+off+1,g_input.end(),0,[](const auto& n,auto){const auto pred=&n-1;return *pred>=n;})-g_input.begin()-1;}
void partial_init(const vl_t& src){const int size=src.size();g_partial.assign(size,0);for(int i=0;i<size;++i)for(int a=i;a<size;a|=a+1)g_partial[a]+=src[i];}
void partial_update(vl_t& src, const size_t pos, const int64_t val){const auto size=src.size();for(auto i=pos;i<size;i|=i+1)g_partial[i]+=val-src[pos];src[pos]=val;}
int64_t partial_sum(const int i, const int j){int64_t r{0};for(int k=j+1;k>0;k&=k-1)r+=g_partial[k-1];for(int k=i;k>0;k&=k-1)r-=g_partial[k-1];return r;}
int64_t pow(int64_t a, int64_t n) {int64_t r{1};while(n>0){if(n&1)r*=a;a*=a;n>>=1;}return r;}


static void solve() {
    int64_t ret{0};
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
        //std::cout << "Case #" << g << ": " << std::fixed << std::setprecision(9); solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 $TASKNAME$.cpp -o $TASKNAME$.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address $TASKNAME$.cpp -o $TASKNAME$

Run:
py.exe interactive_runner.py py.exe $TASKNAME$_testing_tool.py 1 -- $TASKNAME$.exe
$TASKNAME$.exe < $TASKNAME$.in

Input:


Output:


*/