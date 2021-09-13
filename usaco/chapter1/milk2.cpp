/*
ID: marcin.8
LANG: C++
TASK: milk2
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=VTuK8BtpCVk&S=milk2
*/

#include <algorithm>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>

std::ifstream task_in("milk2.in");
std::ofstream task_out("milk2.out");

enum class event_t {
    start = -1,
    finish = 1
};

using sweep_t = std::pair<int, event_t>; // <TIME, TYPE>

int main(int, char**) // O(NlogN) CPU + O(N) MEMORY, better for small N << max
{
    int N; task_in >> N;
    std::vector<sweep_t> events(2 * N);
    for (int i = 0; i < 2 * N; i += 2) {
        int s, f; task_in >> s >> f;
        events[i] = {s, event_t::start};
        events[i + 1] = {f, event_t::finish};
    }

    std::sort(events.begin(), events.end());
    int milking{0}, max_milking{0}, max_relax{0}, change{events[0].first};
    for (const sweep_t& ev : events) {
        if (milking == 0 && ev.second == event_t::start) {
            max_relax = std::max(max_relax, ev.first - change);
            change = ev.first;
        } else if (milking == 1 && ev.second == event_t::finish) {
            max_milking = std::max(max_milking, ev.first - change);
            change = ev.first;
        }

        milking -= (int)ev.second;
    }

    task_out << max_milking << " " << max_relax << "\n";
}

static void for_huge_N() // O(N + max) CPU + O(max) MEMORY
{
    constexpr int max = 1000000;
    int begining{max}, ending{0}, N; task_in >> N;
    std::vector<int> start(max), finish(max);
    for (int i = 0; i < N; ++i) {
        int s, f; task_in >> s >> f;
        begining = std::min(begining, s);
        ending = std::max(ending, f);
        ++start[s];
        ++finish[f];
    }

    int milking{0}, change{begining}, max_milking{0}, max_relax{0};
    for (int i = begining; i <= ending; ++i) {
        if (milking == 0 && start[i] > 0) {
            max_relax = std::max(max_relax, i - change);
            change = i;
        } else if (milking > 0 && finish[i] == milking && start[i] == 0) {
            max_milking = std::max(max_milking, i - change);
            change = i;
        }
        milking = milking + start[i] - finish[i];
    }

    task_out << max_milking << " " << max_relax << "\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 milk2.cpp -o milk2.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 milk2.cpp -o milk2.o

Input:

3
300 1000
700 1200
1500 2100

Output:

900 300

*/
