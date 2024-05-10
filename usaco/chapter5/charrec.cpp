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
int N;

int dp(const int line, const int text_position)
{
    if (N <= line) return 0;
    if (~memo[line][text_position]) return memo[line][text_position];
    int cur_ans{INF}, width{-1}, letter{-1};

    int case_width = 19;
    if (line + case_width <= N) {
        const int continuation = dp(line + case_width, text_position + 1);
        if (continuation < INF) {
            int best_diff{INF}, best_letter{-1};
            for (int letter = 0; letter < CHARS; ++letter)
                for (int skip = 0; skip < 20; ++skip) {
                    int letter_diff{};
                    for (int row = 0; row < 20; ++row)
                        if (row == skip)
                            continue;
                        else {
                            const auto text_line = data[line + row - (row < skip ? 0 : 1)];
                            const auto font_line = font[letter * SIZE + row];
                            letter_diff += (text_line ^ font_line).count();
                        }
                    if (letter_diff < best_diff) {
                        best_diff = letter_diff;
                        best_letter = letter;
                    }
                }

            if (best_diff + continuation < cur_ans) {
                cur_ans = best_diff + continuation;
                letter = best_letter;
                width = case_width;
            }
        }
    }

    case_width = 20;
    if (line + case_width <= N) {
        const int continuation = dp(line + case_width, text_position + 1);
        if (continuation < INF) {
            int best_diff{INF}, best_letter{-1};
            for (int letter = 0; letter < CHARS; ++letter) {
                int letter_diff{};
                for (int row = 0; row < 20; ++row) {
                    const auto text_line = data[line + row];
                    const auto font_line = font[letter * SIZE + row];
                    letter_diff += (text_line ^ font_line).count();
                }
                if (letter_diff < best_diff) {
                    best_diff = letter_diff;
                    best_letter = letter;
                }
            }

            if (best_diff + continuation < cur_ans) {
                cur_ans = best_diff + continuation;
                letter = best_letter;
                width = case_width;
            }
        }
    }

    case_width = 21; 
    if (line + case_width <= N) {
        const int continuation = dp(line + case_width, text_position + 1);
        if (continuation < INF) {
            int best_diff{INF}, best_letter{-1};
            for (int letter = 0; letter < CHARS; ++letter)
                for (int dub = 0; dub < 20; ++dub) {
                    int letter_diff{};
                    for (int row = 0; row < 20; ++row) {
                        const auto text_line = data[line + row + (dub < row ? 0 : 1)];
                        const auto font_line = font[letter * SIZE + row];
                        letter_diff += (text_line ^ font_line).count();
                    }
                    if (letter_diff < best_diff) {
                        best_diff = letter_diff;
                        best_letter = letter;
                    }
                }

            if (best_diff + continuation < cur_ans) {
                cur_ans = best_diff + continuation;
                letter = best_letter;
                width = case_width;
            }
        }
    }

    if (width == -1)
        return memo[line][text_position] = INF;

    ans[line][text_position] = {letter, width};
    return memo[line][text_position] = cur_ans;
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
    task_in >> N >> std::ws;
    data.resize(N);
    for (auto& bl : data) {
        std::getline(task_in, line);
        bl = line_t{line};
    }

    const int max_length = (N + 18) / 19;
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
