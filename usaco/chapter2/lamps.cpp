/*
ID: marcin.8
LANG: C++
TASK: lamps
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=dJ0oxLYmaoh&S=lamps
*/

#include <fstream>
#include <set>
#include <unordered_set>
#include <vector>

std::ifstream task_in("lamps.in");
std::ofstream task_out("lamps.out");

int N, C;
constexpr const int MAXLAMP = 6; // LCM of buttons periods
auto bt1 = [](const std::vector<bool>& in){ std::vector<bool> out = in; for (int i = 0; i < N; ++i) out[i] = !in[i]; return out; };
auto bt2 = [](const std::vector<bool>& in){ std::vector<bool> out = in; for (int i = 0; i < N; i += 2) out[i] = !in[i]; return out; };
auto bt3 = [](const std::vector<bool>& in){ std::vector<bool> out = in; for (int i = 1; i < N; i += 2) out[i] = !in[i]; return out; };
auto bt4 = [](const std::vector<bool>& in){ std::vector<bool> out = in; for (int i = 0; i < N; i += 3) out[i] = !in[i]; return out; };

int main(int, char**)
{
    task_in >> N >> C;
    std::set<std::vector<bool>> state[2];
    state[C & 1].insert(std::vector<bool>(N, true));
    while (C) {
        const auto& in = state[C & 1];
        --C;
        auto& out = state[C & 1];
        out.clear();
        for (const auto& lamps : in) {
            out.insert(bt1(lamps));
            out.insert(bt2(lamps));
            out.insert(bt3(lamps));
            out.insert(bt4(lamps));
        }
    }

    std::unordered_set<int> on_set, off_set;
    while (true) {
        int ind; task_in >> ind;
        if (ind < 0) break;
        on_set.insert((ind - 1) % MAXLAMP);
    }
    while (true) {
        int ind; task_in >> ind;
        if (ind < 0) break;
        off_set.insert((ind - 1) % MAXLAMP);
    }

   bool any_config{false};
    for (const auto& lamps : state[0]) {
        bool ok{true};
        for (const int i : on_set)
            if (!lamps[i]) {
                ok = false;
                break;
            }
        if (!ok)
            continue;
        for (const int i : off_set)
            if (lamps[i]) {
                ok = false;
                break;
            }
        if (ok) {
            if (!lamps.empty())
                any_config = true;
            for (const auto l : lamps)
                task_out << l;
            task_out << '\n';
        }
    }
    if (!any_config)
        task_out << "IMPOSSIBLE\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 lamps.cpp -o lamps.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address lamps.cpp -o lamps

Input:


Output:


*/
