/*
ID: marcin.8
LANG: C++
TASK: window
PROBLEM STATEMENT: 
*/

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

std::ifstream task_in("window.in");
std::ofstream task_out("window.out");

struct window_t 
{
    char id;
    int x, y, X, Y;
};

std::vector<window_t> screen;

int main(int, char**)
{
    std::string line;
    while (std::getline(task_in, line)) {
        char action, id;
        int x, y, X, Y;
        assert(2 == sscanf_s(line.data(), "%c(%c", &action, 1, &id, 1));
        const auto mid = std::find_if(screen.begin(), screen.end(), [id](const auto& w){ return w.id == id; });
        assert(action == 'w' || mid != screen.end());

        switch (action)
        {
        case 'w':
            assert(4 == sscanf_s(line.data() + 4, "%d,%d,%d,%d)", &x, &y, &X, &Y));
            screen.push_back(window_t{id, std::min(x, X), std::min(y, Y), std::max(x, X), std::max(y, Y)});
            break;
        case 't':
            assert(mid != screen.end());
            std::rotate(mid, mid + 1, screen.end());
            break;
        case 'b':
            assert(mid != screen.end());
            std::rotate(screen.begin(), mid, mid + 1);
            break;
        case 'd':
            screen.erase(mid);
            break;
        default:
            break;
        }
    }
    task_out << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 window.cpp -o window.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address window.cpp -o window

Input:


Output:


*/
