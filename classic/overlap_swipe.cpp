
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
    if (!window.empty() && start < finish) {
        std::cout << "    [" << start << ", " << finish - 1 << "] ";
        for (const auto& s : window) std::cout << s << ' ';
        std::cout << std::endl;
    }
}

static void solution() // 2N*log(2N)
{
    using edition_t = std::tuple<int, bool, std::string>; // { pos, inserting, editor_name }
    int N; std::cin >> N; // ignore text size
    std::cin >> N;
    const int size = 2 * N;
    std::vector<edition_t> editors(size);
    for (int i = 0; i < N; ++i) {
        int start, stop;
        std::string name;
        // format: editor_name, start_position, one_after_last_position
        std::cin >> name >> start >> stop;
        ++stop;
        editors[2 * i] = {start, true, name};
        editors[2 * i + 1] = {stop, false, name};
    }

    std::sort(editors.begin(), editors.end());

    int prev{0};
    editors_t cur;
    for (const auto& [pos, inserting, name] : editors) {
        print_window(prev, pos, cur);
        if (inserting)
            cur.insert(name);
        else 
            cur.erase(name);
        prev = pos;
    }
}

void solution_overcomplicated() // O(M*logM) not depending on text size
{
    using edition_t = std::tuple<int, int, std::string>; // {start, stop, editor_name }
    constexpr auto ed_comp = [](const edition_t& ed1, const edition_t& ed2) {
        return std::get<1>(ed1) > std::get<1>(ed2);
    };

    int size; std::cin >> size;
    int N; std::cin >> N;
    std::vector<edition_t> editors(N); // [start, stop)
    for (auto& ed : editors) {
        // format: editor_name, start_position, one_after_last_position
        std::cin >> std::get<2>(ed) >> std::get<0>(ed) >> std::get<1>(ed);
        ++std::get<1>(ed);
    }

    editors_t window;
    std::priority_queue<edition_t, std::vector<edition_t>, decltype(ed_comp)> pq(ed_comp);
    std::sort(editors.begin(), editors.end());

    int cur{0};
    for (const edition_t& ed : editors) {
        while (!pq.empty() && std::get<1>(pq.top()) <= std::get<0>(ed)) {
            const auto& fin = pq.top();
            if (std::get<1>(pq.top()) != std::get<0>(ed) || (!g_coalesce && window.count(std::get<2>(fin)) == 1)) {
                print_window(cur, std::get<1>(fin), window);
                cur = std::get<1>(fin);
            }
            window.extract(std::get<2>(fin));
            pq.pop();
        }

        if (!window.empty()) {
            print_window(cur, std::get<0>(ed), window);
            cur = std::get<0>(ed);
        }

        window.insert(std::get<2>(ed));
        pq.push(ed);
    }

    while (!pq.empty()) {
        const auto& fin = pq.top();
        print_window(cur, std::get<1>(fin), window);
        cur = std::get<1>(fin);
        window.extract(std::get<2>(fin));
        pq.pop();
    }
}

void solution_mle() // O(N*logM) with O(N) memory
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

int main(int, char**)
{
    solution();

    return 0;
}

/* 
clang++.exe -Wall -g -std=c++17 overlap_swipe.cpp -o overlap_swipe.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address overlap_swipe.cpp -o overlap_swipe

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
Kabacki 24 30

Output:

[0, 1] Dabacki, 
[2, 3] Dabacki, Ibacki, 
[4, 21] Babacki, Dabacki, Ibacki, 
[22, 22] Dabacki, Ibacki, 
[23, 23] Dabacki, 
[24, 30] Dabacki, Gabacki, Kabacki, 
[31, 31] Dabacki, Gabacki, 
[32, 44] Dabacki, Gabacki, Jabacki, 
[45, 49] Dabacki, Fabacki, Gabacki, Jabacki, 
[50, 52] Cabacki, Dabacki, Fabacki, Jabacki, 
[53, 54] Cabacki, Dabacki, Fabacki, Habacki, Jabacki, 
[55, 64] Cabacki, Dabacki, Habacki, Jabacki, 
[65, 65] Cabacki, Dabacki, Habacki, 
[66, 67] Cabacki, Dabacki, Ebacki, 
[68, 70] Cabacki, Dabacki, 
[71, 76] Cabacki, 
[77, 80] Abacki, Cabacki, 
[81, 100] Abacki, 

*/
