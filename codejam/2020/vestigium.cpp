
#include <algorithm>
#include <iostream>
#include <iterator>
#include <unordered_set>
#include <vector>

// Vestigium
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019fd27/000000000020993c

template <typename T> using vec = std::vector<T>;
template <typename T> using vec2 = std::vector<std::vector<T>>;
template <typename T> std::vector<T> fill(const size_t size){std::vector<T> cont(size);std::copy_n(std::istream_iterator<T>{std::cin},size,cont.begin());return cont;};
template <typename T> void fill2sq(std::vector<std::vector<T>>& cont, int& size){std::cin>>size;cont.resize(size);for(auto& r:cont) r=fill<T>(size);};

void solve() {
    int N, T{0}, RR{0}, CC{0};
    vec2<int> matrix;
    fill2sq(matrix, N);
    for (int i = 0; i < N; ++i)
        T += matrix[i][i];
    std::unordered_set<int> sink;
    for (int r = 0; r < N; ++r) {
        sink.clear();
        for (int c = 0; c < N; ++c)
            sink.insert(matrix[r][c]);
        if ((int)sink.size() < N)
            ++RR;
    }
    for (int c = 0; c < N; ++c) {
        sink.clear();
        for (int r = 0; r < N; ++r)
            sink.insert(matrix[r][c]);
        if ((int)sink.size() < N)
            ++CC;
    }
    std::cout << T << " " << RR << " " << CC;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << "\n";
    }
}

/*
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 vestigium.cpp -o vestigium.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 vestigium.cpp -o vestigium.o
vestigium.exe < vestigium.in

Input:

3
4
1 2 3 4
2 1 4 3
3 4 1 2
4 3 2 1
4
2 2 2 2
2 3 2 3
2 2 2 3
2 2 2 2
3
2 1 3
1 3 2
1 2 3

Output:

Case #1: 4 0 0
Case #2: 9 4 4
Case #3: 8 0 2

*/
