#include <numeric>
#include <stdio.h>
#include <vector>

// sort sequence of objects by a key from a finite set
// simplification: object == key == decimal digit

// case definition - 301st Fibonacci number
constexpr char g_pattern[] = 
"5357543035931336604742125245300009052807024058527"
"6680372187519418517552556246806124659918940784792"
"9063797336458776573412593572642846157021799228878"
"7349287401967283887412115492710537302531185570938"
"9770910765232374917909706336993837795827719730385"
"3145728559823884327108383021491582631219341860283"
"4034688";

// globals
std::vector<uint16_t> g_stats(11, 0);

int main(int argc, char* argv[])
{
    // scan number of occurences
    for (int i = 0; i < sizeof(g_pattern) - 1; ++i)
        g_stats[g_pattern[i] - '0' + 1]++;

    // count initial index for each symbol
    std::inclusive_scan(g_stats.begin(), g_stats.end(), g_stats.begin());

    // output stable, sorted collection
    std::vector<char> output(sizeof(g_pattern) - 1);
    for (int i = 0; i < output.size(); ++i)
        output[g_stats[g_pattern[i] - '0']++] = g_pattern[i];

    // present result
    printf("\nOryginal sequence:\n");
    for (int i = 0; i < sizeof(g_pattern) - 1; ++i)
        printf("%c", g_pattern[i]);
    printf("\n\nSorted digits:\n");
    for (const auto d : output)
        printf("%c", d);

    return 0;
}