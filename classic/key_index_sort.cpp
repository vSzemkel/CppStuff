
#include <numeric>
#include <stdio.h>
#include <vector>

// sort sequence of objects by a key from a finite set
// simplification: object == key == decimal digit

// case definition - 301st Fibonacci number, a digits sequence
constexpr int g_distinct_keys = 10;
constexpr char g_pattern[] = 
"5357543035931336604742125245300009052807024058527"
"6680372187519418517552556246806124659918940784792"
"9063797336458776573412593572642846157021799228878"
"7349287401967283887412115492710537302531185570938"
"9770910765232374917909706336993837795827719730385"
"3145728559823884327108383021491582631219341860283"
"4034688";
constexpr int g_pattern_size = sizeof(g_pattern) - 1;

// globals
std::vector<uint16_t> g_stats(g_distinct_keys + 1, 0);

int main(int, char**)
{
    // scan number of occurences
    for (int i = 0; i < g_pattern_size; ++i)
        g_stats[g_pattern[i] - '0' + 1]++;

    // count initial index for each symbol
    std::inclusive_scan(g_stats.begin(), g_stats.end(), g_stats.begin());

    // output stable, sorted collection, +1 to null terminate
    std::vector<char> output(g_pattern_size + 1);
    for (int i = 0; i < g_pattern_size; ++i)
        output[g_stats[g_pattern[i] - '0']++] = g_pattern[i];

    // present result
    printf("Original sequence:\n%s\n\n", g_pattern);
    printf("Sorted digits:\n%s\n", output.data());

    return 0;
}

/*
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 key_index_sort.cpp -o key_index_sort.exe

Output:

Original sequence:
5357543035931336604742125245300009052807024058527668037218751941851755255624680612465991894078479290637973364587765734125935726428461570217992288787349287401967283887412115492710537302531185570938977091076523237491790970633699383779582771973038531457285598238843271083830214915826312193418602834034688

Sorted digits:
0000000000000000000000000000111111111111111111111111111122222222222222222222222222222222333333333333333333333333333333333333444444444444444444444444455555555555555555555555555555555566666666666666666666777777777777777777777777777777777777778888888888888888888888888888888889999999999999999999999999999

*/