/*
ID: marcin.8
LANG: C++
TASK: charrec
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=lctPANaw7rt&S=charrec
*/

#include <array>
#include <bitset>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

std::ifstream font_in("font.in");
std::ifstream task_in("charrec.in");
std::ofstream task_out("charrec.out");

static constexpr int INF = 1e09;
static constexpr int CHARS = 27;
static constexpr int SIZE = 20;
const char LETTERS[] = " abcdefghijklmnopqrstuvwxyz";

using line_t = std::bitset<SIZE>;
std::array<line_t, CHARS * SIZE> font;
std::vector<line_t> data;

std::vector<std::vector<std::pair<int, int>>> ans; // [line][text_position] = {letter, size}
std::vector<std::vector<int>> memo; // [line][text_position] = bit mismatches

int dp(const int line, const int text_position)
{
    return line+text_position-19;
}

int main(int, char**)
{
    // Read font
    std::string line;
    std::getline(font_in, line); // skip 540
    for (auto& bl : font) {
        std::getline(font_in, line);
        bl = line_t{line};
    }

    // Read data
    int N;
    task_in >> N >> std::ws;
    data.resize(N);
    for (auto& bl : data) {
        std::getline(task_in, line);
        bl = line_t{line};
    }

    const int max_length = N + 18 / 19;
    ans.assign(N, std::vector<std::pair<int, int>>(max_length, {-1, -1})); // -1 is not seen yet
    memo.assign(N, std::vector<int>(max_length, -1)); // -1 is not seen yet

    dp(0, 0);

    std::string found;
    int row{}, text_position{};
    while (row < N) {
        const auto& answer = ans[row][text_position];
        found += LETTERS[answer.first];
        row += answer.second;
        ++text_position;
    }

    task_out << found << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 charrec.cpp -o charrec.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address charrec.cpp -o charrec

Run:
charrec.exe && type charrec.out

Input:


Output:


*/
