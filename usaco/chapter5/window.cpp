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
#include <iomanip>
#include <string>
#include <vector>

std::ifstream task_in("window.in");
std::ofstream task_out("window.out");

struct window_t 
{
    char id;
    int x, y, X, Y;
    float cache;
    int area() const {
        return (X - x) * (Y - y);
    }
};

std::vector<window_t> screen;

void print(const std::vector<window_t>::iterator mid)
{
    //if (mid->cache < 0) {
        int covered{}, total = mid->area();
        for (int i = mid->x; i < mid->X; ++i)
            for (int j = mid->y; j < mid->Y; ++j)
                if (std::any_of(mid + 1, screen.end(), [i, j](const auto& f){
                    return f.x <= i && i < f.X && f.y <= j && j < f.Y;
                }))
                    ++covered;

        mid->cache = 100.0 * (total - covered) / total;
    //}

    task_out << std::fixed << std::setprecision(3) << mid->cache << '\n';
}

int main(int, char**)
{
    std::string line;
    while (std::getline(task_in, line)) {
        char action, id;
        int x, y, X, Y;
        //assert(2 == sscanf(line.data(), "%c(%c", &action, &id));
        assert(2 == sscanf_s(line.data(), "%c(%c", &action, 1, &id, 1));
        const auto mid = std::find_if(screen.begin(), screen.end(), [id](const auto& w){ return w.id == id; });
        assert(action == 'w' || mid != screen.end());

        switch (action)
        {
        case 'w':
            //assert(4 == sscanf(line.data() + 4, "%d,%d,%d,%d)", &x, &y, &X, &Y));
            assert(4 == sscanf_s(line.data() + 4, "%d,%d,%d,%d)", &x, &y, &X, &Y));
            screen.push_back(window_t{id, std::min(x, X), std::min(y, Y), std::max(x, X), std::max(y, Y), -1.});
            std::for_each(screen.begin(), screen.end(), [](auto& w){ w.cache = -1.; });
            break;
        case 't':
            std::rotate(mid, mid + 1, screen.end());
            std::for_each(mid, screen.end(), [](auto& w){ w.cache = -1.; });
            break;
        case 'b':
            std::rotate(screen.begin(), mid, mid + 1);
            std::for_each(screen.begin(), mid, [](auto& w){ w.cache = -1.; });
            break;
        case 'd':
            screen.erase(mid);
            break;
        case 's': {
            print(mid);
            break;
        }
        default:
            assert(true);
        }
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 window.cpp -o window.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address window.cpp -o window

Input:

w(s,26368,31524,7661,22131) 
s(s) 
w(E,23902,16575,17367,18133) 
w(0,2548,19609,32409,3058) 
s(E) 
s(s) 
s(s) 
w(Z,16613,8720,8446,2823) 
s(Z) 
b(Z) 
s(Z) 
t(0) 
s(E)

Output:

100.000
0.000
100.000
100.000
100.000
3.985
0.000

*/
