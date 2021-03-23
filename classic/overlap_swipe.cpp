
#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <tuple>
#include <vector>

// Overlapped swipe
// Find all uniformly edited fragments

constexpr bool g_coalesce{true};
using editors_t = std::set<std::string>;

void print_window(const int start, const int finish, const editors_t& window)
{
    std::cout << "    [" << start << ", " << finish << "] ";
    for (const auto& s : window) std::cout << s << ", ";
    std::cout << "\n";
}

void solution1() // O(N*logM)
{
    std::vector<editors_t> g_start;
    std::vector<editors_t> g_finish;
    int g_text_size; std::cin >> g_text_size;
    int no_of_editors; std::cin >> no_of_editors;

    ++g_text_size;
    g_start.resize(g_text_size);
    g_finish.resize(g_text_size);
    
    for (int i = 0; i < no_of_editors; ++i) {
        // format: editor_name, start_position, one_after_last_position
        int start, finish;
        std::string editor;
        std::cin >> editor >> start >> finish;
        g_start[start].insert(editor);
        g_finish[finish].insert(editor);
    }

    int last{0};
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
}

void solution2() // O(M*logM) not depending on text size
{
    using edition_t = std::tuple<int, int, std::string>;
    constexpr auto ed_comp = [](const edition_t& ed1, const edition_t& ed2) {
        return std::get<1>(ed1) > std::get<1>(ed2);
    };

    int size; std::cin >> size;
    int N; std::cin >> N;
    std::vector<edition_t> editors(N); // [start, stop)
    for (auto& ed : editors)
        // format: editor_name, start_position, one_after_last_position
        std::cin >> std::get<2>(ed) >> std::get<0>(ed) >> std::get<1>(ed);

    editors_t window;
    edition_t result;
    std::priority_queue<edition_t, std::vector<edition_t>, decltype(ed_comp)> pq(ed_comp);
    std::sort(editors.begin(), editors.end());

    for (const edition_t& ed : editors) {
        while (!pq.empty() && std::get<1>(pq.top()) <= std::get<1>(ed)) {
            const auto& fin = pq.top();
            if (std::get<1>(pq.top()) != std::get<1>(ed) || !g_coalesce)
                print_window(std::get<0>(fin), std::get<1>(fin), window);
            window.extract(std::get<2>(fin));
            pq.pop();
        }

        window.insert(std::get<2>(ed));
        pq.push(ed);
    }

    while (!pq.empty()) {
        const auto fin = pq.top();
        print_window(std::get<0>(fin), std::get<1>(fin), window);
        window.extract(std::get<2>(fin));
        pq.pop();
    }
}

int main(int, char**)
{
    //solution1();
    solution2();

    return 0;
}

/* 
clang++.exe -Wall -g -std=c++17 overlap_swipe.cpp -o overlap_swipe.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address overlap_swipe.cpp -o overlap_swipe.o

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
