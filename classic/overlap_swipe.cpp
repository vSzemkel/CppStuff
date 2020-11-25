
#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>

// Overlapped swipe
// Find all uniformly edited fragments O(2n*logm)

constexpr bool g_coalesce{true};
using editors_t = std::set<std::string>;

// globals
int g_text_size;
std::vector<editors_t> g_start;
std::vector<editors_t> g_finish;

void read_input() {
    std::string editor;
    int no_of_editors, start, finish;
    std::cin >> g_text_size;
    ++g_text_size;
    g_start.resize(g_text_size);
    g_finish.resize(g_text_size);
    std::cin >> no_of_editors;
    for (int i = 0; i < no_of_editors; ++i) {
        // format: editor_name, start_position, one_after_last_position
        std::cin >> editor >> start >> finish;
        g_start[start].insert(editor);
        g_finish[finish].insert(editor);
    }
}

void print_window(const int start, const int finish, const editors_t& window)
{
    std::cout << "    [" << start << ", " << finish << "] ";
    for (const auto& s : window) std::cout << s << ", ";
    std::cout << "\n";
}

int main(int argc, char* argv[])
{
    read_input();

    int last = 0;
    editors_t window{};
    for (int i = 0; i < g_text_size; ++i) {
        auto& start = g_start[i];
        auto& finish = g_finish[i];
        if (g_coalesce) {
            std::vector<std::string> commons;
            std::set_intersection(start.begin(), start.end(), finish.begin(), finish.end(), std::back_inserter(commons));
            for (const auto& editor : commons) {
                start.extract(editor);
                finish.extract(editor);
            }
        }
        if (!start.empty() || !finish.empty()) {
            // window changes - print out the last one if not empty
            if (!window.empty())
                print_window(last, i, window);
            // remove finished
            for (const auto& s : g_finish[i])
                window.extract(s);
            // add starting
            window.merge(g_start[i]);
            // start of a new window
            last = i;
        }
    }

    return 0;
}

/* 
clang++.exe -Wall -g -std=c++17 overlap_swipe.cpp -o overlap_swipe.exe
overlap_swipe.exe < overlap_swipe.in

Input:

100
10
Abacki 77 100
Babacki 4 21
Cabacki 50 80
Dabacki 0 70
Ebacki 66 67
Fabacki 45 54
Gabacki 24 49
Habacki 53 65
Ibacki 2 22
Jabacki 32 64

Output:

    [0, 2] Dabacki, 
    [2, 4] Dabacki, Ibacki,
    [4, 21] Babacki, Dabacki, Ibacki,
    [21, 22] Dabacki, Ibacki,
    [22, 24] Dabacki,
    [24, 32] Dabacki, Gabacki, 
    [32, 45] Dabacki, Gabacki, Jabacki,
    [45, 49] Dabacki, Fabacki, Gabacki, Jabacki,
    [49, 50] Dabacki, Fabacki, Jabacki,
    [50, 53] Cabacki, Dabacki, Fabacki, Jabacki, 
    [53, 54] Cabacki, Dabacki, Fabacki, Habacki, Jabacki,
    [54, 64] Cabacki, Dabacki, Habacki, Jabacki,
    [64, 65] Cabacki, Dabacki, Habacki,
    [65, 66] Cabacki, Dabacki, 
    [66, 67] Cabacki, Dabacki, Ebacki,
    [67, 70] Cabacki, Dabacki,
    [70, 77] Cabacki,
    [77, 80] Abacki, Cabacki,
    [80, 100] Abacki,

*/
