/*
ID: marcin.8
LANG: C++
TASK: tour
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=TIPYHbbD5Id&S=tour
*/

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

std::ifstream task_in("tour.in");
std::ofstream task_out("tour.out");

struct city_t {
    std::vector<int> east;
    std::vector<int> west;
};

int start, turn;
size_t ans{1};
std::vector<int> track;
std::vector<bool> seen;
std::vector<city_t> flights;
std::unordered_map<std::string, int> names;

void dfs(int where, bool to_east = true)
{
    if (to_east)
        for (const int e : flights[where].east) {
            seen[e] = true;
            track.push_back(e);
            dfs(e, e != turn);
        }
    else
        for (const int w : flights[where].west) {
            if (w == start && ans < track.size()) {
                ans = track.size();
                // here copy track to solution path if needed
            } else if (!seen[w]) {
                seen[w] = true;
                track.push_back(w);
                dfs(w, false);
            }
        }

    track.pop_back();
    seen[where] = false;
}

int main(int, char**)
{
    int C, F;
    task_in >> C >> F;

    flights.resize(C);
    track.reserve(C);
    seen.resize(C);
    for (int c = 0; c < C; ++c) {
        std::string n;
        task_in >> n;
        names[n] = c;
    }
    for (int f = 0; f < F; ++f) {
        std::string n, n2;
        task_in >> n >> n2;
        // assert(names.contains(n) && names.contains(n2));
        const int i = names[n];
        const int i2 = names[n2];
        if (i < i2) {
            flights[i].east.push_back(i2);
            flights[i2].west.push_back(i);
        } else {
            flights[i].west.push_back(i2);
            flights[i2].east.push_back(i);
        }
    }

    start = 0;
    turn = C - 1;
    track.push_back(start);
    seen[start] = true;
    dfs(start);

    task_out << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 tour.cpp -o tour.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address tour.cpp -o tour

Run:
tour.exe && type tour.out

Input:


Output:


*/
