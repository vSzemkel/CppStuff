
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// Currency Rates
// Given some currency rates which forms connected graph off available currencies
// answer questions about arbitrary 

static void solve() {
    int N, ord{-1};
    std::cin >> N;
    std::unordered_map<std::string, int> index; // currency -> index
    std::vector<std::vector<std::pair<int, double>>> tree(N + 1);
    while (N--) {
        std::string from, to;
        double rate;
        std::cin >> from >> to >> rate;
        assert(rate != 0);
        if (index.contains(from) && index.contains(to))
            continue;
        int fromi, toi;
        if (index.contains(from))
            fromi = index[from];
        else {
            fromi = ++ord;
            index[from] = fromi;
        }
        if (index.contains(to))
            toi = index[to];
        else {
            toi = ++ord;
            index[to] = toi;
        }
        tree[fromi].emplace_back(toi, rate);
        tree[toi].emplace_back(fromi, 1 / rate);
    }

    std::vector<bool> seen(++ord);
    std::vector<double> rates(ord);
    std::vector<int> stack(1);
    seen[0] = true;
    rates[0] = 1;
    while (!stack.empty()) {
        const auto node = stack.back();
        stack.pop_back();
        for (const auto& [to, rate] : tree[node]) {
            if (seen[to])
                continue;
            rates[to] = rates[node] * rate;
            seen[to] = true;
            stack.push_back(to);
        }
    }

    int Q;
    std::cin >> Q;
    while (Q--) {
        std::string from, to;
        std::cin >> from >> to;
        const auto rate = rates[index[to]] / rates[index[from]];
        std::cout << '\n' << from << ' ' << to << ' ' << rate;
    }
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve(); std::cout << '\n';
    }
}

/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 currency_rates.cpp -o currency_rates.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address currency_rates.cpp -o currency_rates

Run:
currency_rates.exe < currency_rates.in

Input:

1
5
PLN EUR 4.63
PLN USD 4.34
DKK EUR 7.45
NIS USD 3.81
PLN SEK 0.39
5
PLN NIS
DKK PLN
USD EUR
EUR PLN
NIS NIS

Output:

Case #1:
PLN NIS 1.13911
DKK PLN 1.60907
USD EUR 1.06682
EUR PLN 0.215983
NIS NIS 1

*/
