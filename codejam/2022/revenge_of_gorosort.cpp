
#include <cmath>
#include <iostream>
#include <vector>

// Revenge of GoroSort
// https://codingcompetitions.withgoogle.com/codejam/round/00000000008779b4/0000000000b45189


int N, K;
constexpr const int LONG_CYCLE = 7;

static void solve() {
    std::vector<int> balls(N), ans(N);

    for (int z = K; z; --z) {
        for (auto& b : balls) {
            std::cin >> b; --b;
        }
        // identify sort disturbance cycles
        int c{0}, res;
        std::vector<bool> seen(N);
        for (int i = 0; i < N; ++i)
            if (!seen[i]) {
                int start{i}, cur{i}, len{1};
                ans[cur] = ++c;
                seen[start] = true;
                cur = balls[start];
                while (cur != start) {
                    ++len;
                    ans[cur] = c;
                    seen[cur] = true;
                    cur = balls[cur];
                }

                if (len >= LONG_CYCLE) { // chopp long cycles
                    const int total = len;
                    const int size = int(std::sqrt(total)); // bit of magick
                    len = 1;
                    cur = balls[start];
                    while (cur != start) {
                        ans[cur] = c;
                        if ((++len % size) == 0 && len + size <= total)
                            ++c;
                        cur = balls[cur];
                    }
                }
            }

        for (const int a : ans)
            std::cout << a << ' ';
        std::cout << std::endl;

        std::cin >> res;
        if (res == -1)
            break;
        if (res == 1) // solved
            return;
    }

    exit(0); // to many trials
}

int main(int, char**)
{
    int no_of_cases;
    //attach_trap();
    std::cin >> no_of_cases >> N >> K;
    for (int g = 1; g <= no_of_cases; ++g)
        solve();
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 revenge_of_gorosort.cpp -o revenge_of_gorosort.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address revenge_of_gorosort.cpp -o revenge_of_gorosort

Run:
py.exe interactive_runner.py py.exe revenge_of_gorosort_testing_tool.py 1 -- revenge_of_gorosort.exe

*/
