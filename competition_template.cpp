
#include <algorithm>
#include <array>
#include <assert.h>
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
#include <queue>
#include <random>
#include <set>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// $TASKNAME$
// https://

const auto big64 = std::numeric_limits<int64_t>::max();
template <typename T> using vec = std::vector<T>;
template <typename T> using vec2 = std::vector<std::vector<T>>;
vec<int64_t> g_input, g_partial;
vec<bool> g_marked;
size_t g_size;

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



int64_t solve() {
    return rand_in_range((int)big64);
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": " << std::setprecision(15) << solve() << "\n";
        //std::cout << "Case #" << g << ": "; solve(); std::cout << "\n";
    }
}

/*
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 $TASKNAME$.cpp -o $TASKNAME$.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 $TASKNAME$.cpp -o $TASKNAME$.o
$TASKNAME$.exe < $TASKNAME$.in

Input:


Output:


*/