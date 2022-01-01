/*
ID: marcin.8
LANG: C++
TASK: contact
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=k9pbbjb8UkI&S=contact
*/

#include <algorithm>
#include <array>
#include <fstream>
#include <string>
#include <vector>

std::ifstream task_in("contact.in");
std::ofstream task_out("contact.out");

constexpr const int MAX_PER_LINE = 6;

int main(int, char**)
{
    int A, B, N; task_in >> A >> B >> N;
    std::string data, chunk;
    while (std::getline(task_in, chunk))
        data.append(chunk);

    int first_chunk{0};
    const int width = B - A + 1;           // observe this may lengths
    const int max = (1 << (B + 1));        // maximal observed number
    const int size = int(data.size());
    const int max_len = std::min(B, size);
    std::vector<std::vector<int>> stats(width, std::vector<int>(max));
    std::vector<std::vector<int>> log(width, std::vector<int>(max));
    for (int len = A; len <= max_len; ++len) {
        if (first_chunk == 0)
            for (int i = 0; i < len; ++i) {
                first_chunk <<= 1;
                first_chunk += data[i] == '0' ? 0 : 1;
            }
        else {
            first_chunk <<= 1;
            first_chunk += data[len - 1] == '0' ? 0 : 1;
        }

        int cur = first_chunk;
        const int mask = ~(1 << len);
        for (int end = len; end < size; ++end) {
            ++stats[len - A][cur];
            log[len - A][cur] = end - len;
            cur <<= 1;
            cur += data[end] == '0' ? 0 : 1;
            cur &= mask;
        }
        ++stats[len - A][cur];
        log[len - A][cur] = size - len;
    }

    std::vector<std::array<int, 3>> result; // {count, len - A, number}
    for (int len = 0; len < width; ++len)
        for (int n = 0; n < max; ++n)
            if (stats[len][n] > 0)
                result.push_back({stats[len][n], len, n});

    std::sort(result.begin(), result.end(), [](const auto& r1, const auto& r2) {
        return r1[0] > r2[0]
           || (r1[0] == r2[0] && r1[1] < r2[1])
           || (r1[0] == r2[0] && r1[1] == r2[1] && r1[2] < r2[2]);
    });

    const int rsz = int(result.size());
    for (int z = N, pos = 0; z && pos < rsz; --z) {
        const int freq = result[pos][0];
        task_out << freq << std::endl;
        int lc{0};
        while (pos < rsz && result[pos][0] == freq) {
            if ((lc % MAX_PER_LINE) > 0)
                task_out << ' ';
            const int l = result[pos][1];
            for (int i = log[l][result[pos][2]], z = A + l; z; --z, ++i)
                task_out << data[i];
            if ((++lc % MAX_PER_LINE) == 0)
                task_out << std::endl;
            ++pos;
        }
        if ((lc % MAX_PER_LINE) != 0)
            task_out << std::endl;
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 contact.cpp -o contact.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address contact.cpp -o contact

Input:

2 4 10
01010010010001000111101100001010011001111000010010011110010000000

Output:


*/
