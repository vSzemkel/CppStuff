
#include <iostream>
#include <iterator>
#include <queue>
#include <vector>

// Robot path
// Robot is sitting on the upper left corner of grid. The robot can only move
// in two directions, right and down, but certain cells are "off limits".
// Find a path to the bottom right.


template <typename T> std::vector<T> fill(const size_t size){ std::vector<T> cont(size); std::copy_n(std::istream_iterator<T>{std::cin}, size, cont.begin()); return cont; };
template <typename T> void fill2(std::vector<std::vector<T>>& cont, int& rows, int& cols){ std::cin >> rows >> cols; cont.resize(rows); for (auto& r : cont) r = fill<T>(cols); };

void solve() {
    int rows, cols;
    std::queue<int> bfs;
    std::vector<std::vector<int>> board;

    fill2<int>(board, rows, cols);
    if (board[0][0] == 0) {
        bfs.push(0);
        while (!bfs.empty()) {
            auto cell = bfs.front();
            bfs.pop();

            const auto row = cell / rows;
            const auto col = cell % rows;
            if (cell + 1 == rows * cols) { // found
                std::vector<int> path;
                while (cell != 0) {
                    path.push_back(cell);
                    cell = board[cell / rows][cell % cols];
                }

                std::cout << " [0, 0]";
                for (auto it = path.rbegin(); it != path.rend(); ++it)
                    std::cout << " -> [" << *it / rows << ", " << *it % cols << "]";
                return;
            }

            const auto right = (cell % cols) + 1;
            if (right < cols && board[row][right] == 0) {
                board[row][right] = cell;
                bfs.push(cell + 1);
            }
            const auto down = row + 1;
            if (down < rows && board[down][col] == 0) {
                board[down][col] = cell;
                bfs.push(cell + cols);
            }
        }
    }

    std::cout << " IMPOSSIBLE";
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
        std::cout << "Case #" << g << ":"; solve(); std::cout << "\n";
    }
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 robot_path.cpp -o robot_path.exe
g++ -Wall -ggdb3 -O0 -std=c++17 robot_path.cpp -o robot_path.o
robot_path.exe < robot_path.in

Input:

2
5 5
0 1 0 0 0
0 1 0 1 0
0 0 0 1 0
0 1 0 0 0
0 0 1 0 0
2 3
0 1 0
1 0 0

Output:

Case #1: [0, 0] -> [1, 0] -> [2, 0] -> [2, 1] -> [2, 2] -> [3, 2] -> [3, 3] -> [3, 4] -> [4, 4]
Case #2: IMPOSSIBLE

*/
