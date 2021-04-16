
#include <cassert>
#include <iostream>
#include <string>

// Robot Path Decoding
// https://codingcompetitions.withgoogle.com/kickstart/round/000000000019ffc8/00000000002d83dc

constexpr int64_t INF = 1000000000;
int64_t R, C;

static void applay(const char d) {
    if (d == 'N') {
        R = (R == 0) ? INF - 1 : R - 1;
    } else if (d == 'S') {
        R = (R == INF - 1) ? 0 : R + 1;
    } else if (d == 'E') {
        C = (C == INF - 1) ? 0 : C + 1;
    } else if (d == 'W') {
        C = (C == 0) ? INF - 1 : C - 1;
    }
}

static void decode_path(const std::string& path) {
    const int size = (int)path.size();
    for (int i = 0; i < size; ++i) {
        int rep{0};
        while (i < size && isdigit(path[i])) {
            rep = 10 * rep + path[i] - '0';
            ++i;
        }
        if (rep > 0) {
            assert(path[i] == '(');
            int level{0};
            const int beg = ++i;
            while (i < size && (level > 0 || path[i] != ')')) {
                if (path[i] == '(')
                    ++level;
                if (path[i] == ')')
                    --level;
                ++i;
            }
            assert(path[i] == ')');
            std::string tmp(path.begin() + beg, path.begin() + i);

            const auto old_C{C};
            const auto old_R{R};
            decode_path(tmp);
            const auto dR = R - old_R;
            const auto dC = C - old_C;
            while (--rep > 0) {
                R = (INF + R + dR) % INF;
                C = (INF + C + dC) % INF;
            }
        } else
            applay(path[i]);
    }
}

static void solve() {
    R = C = 0;
    std::string path; std::cin >> path;

    decode_path(path);
    std::cout << C + 1 << " " << R + 1;
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 robot_path_decoding.cpp -o robot_path_decoding.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address robot_path_decoding.cpp -o robot_path_decoding.o

Run:
robot_path_decoding.exe < robot_path_decoding.in

Input:

4
SSSEEE
N
N3(S)N2(E)N
2(3(NW)2(W2(EE)W))

Output:


*/
