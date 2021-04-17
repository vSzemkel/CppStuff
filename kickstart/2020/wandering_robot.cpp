
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

// Wandering Robot
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ffc8/00000000002d8565

const auto big64 = std::numeric_limits<int64_t>::max();
template <typename T> using vec = std::vector<T>;
template <typename T> using vec2 = std::vector<std::vector<T>>;
std::ofstream g_debug("wandering_robot.log");
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
const auto rand_in_range = [](const int ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int> dist(0,ubound-1);return dist(gen);};
size_t next_max(const int off) {return std::lower_bound(g_input.begin()+off+1,g_input.end(),0,[](const auto& n,auto){const auto pred=&n-1;return *pred<=n;})-g_input.begin()-1;}
size_t next_min(const int off) {return std::lower_bound(g_input.begin()+off+1,g_input.end(),0,[](const auto& n,auto){const auto pred=&n-1;return *pred>=n;})-g_input.begin()-1;}
void partial_init(const vec<int64_t>& src){const int size=src.size();g_partial.assign(size,0);for(int i=0;i<size;++i)for(int a=i;a<size;a|=a+1)g_partial[a]+=src[i];}
void partial_update(vec<int64_t>& src, const size_t pos, const int64_t val){const auto size=src.size();for(auto i=pos;i<size;i|=i+1)g_partial[i]+=val-src[pos];src[pos]=val;}
int64_t partial_sum(const int i, const int j){int64_t ret{0};for(int k=j+1;k>0;k&=k-1)ret+=g_partial[k-1];for(int k=i;k>0;k&=k-1)ret-=g_partial[k-1];return ret;}



static void solve() {
    int64_t CC, RR, L, T, R, B; std::cin >> CC >> RR >> L >> T >> R >> B;

    int64_t all{0}, good{0};
    std::string path(CC - 1, 'E');
    path.append(std::string(RR - 1, 'S'));
    do {
        int r{1}, c{1};
        for (const char d : path) {
            if (L <= c && c <= R && T <= r && r <= B)
                break;
            if (d == 'E') ++c;
            else ++r;
        }
        if (r == RR && c == CC)
            ++good;
        ++all;
    } while(std::next_permutation(path.begin(), path.end()));

    std::cout << (double)good / all;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << std::setprecision(15); solve(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 wandering_robot.cpp -o wandering_robot.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address wandering_robot.cpp -o wandering_robot.o

Run:
py.exe interactive_runner.py py.exe wandering_robot_testing_tool.py 1 -- wandering_robot.exe
wandering_robot.exe < wandering_robot.in

Input:

4
3 3 2 2 2 2
5 3 1 2 4 2
1 10 1 3 1 5
6 4 1 3 3 4

Output:


*/
