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

int main(int, char**)
{
    int C, F;
    task_in >> C >> F;

    std::unordered_map<std::string, int> names;
    std::vector<std::vector<bool>> flights(C, std::vector<bool>(C));
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
        flights[i][i2] = true;
        flights[i2][i] = true;
    }

    const int INF = 1e09;
    std::vector<std::vector<int>> cost(C, std::vector<int>(C, -INF));
    cost[C - 1][C - 1] = 1;
    for (int i = C - 2; ~i; --i)
        for (int j = C - 1; i <= j; --j)
            if (i != j || i == 0)
                for (int k = i + 1; k < C; ++k)
                    if (flights[i][k] && cost[i][j] < 1 + cost[k][j])
                        cost[i][j] = cost[j][i] = 1 + cost[k][j];

    auto ans = cost[0][0] - 1;
    if (ans < 0)
        ans = 1;

    task_out << ans << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 tour.cpp -o tour.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address tour.cpp -o tour

Run:
tour.exe && type tour.out

Input:

8 13
A
B
C
D
E
F
G
H
A B
A C
C B
D C
D B
D E
D F
E F
E G
D H
H F
H G
A G

Output:

8

*/
