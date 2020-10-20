
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Kick_Start
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000001a0069/0000000000414bfb

int kickstart(const std::string& txt)
{
    int ret{0};
    const auto first_kick = txt.find("KICK");
    if (first_kick != std::string::npos) {
        std::vector<int> start;
        auto pos = txt.find("START", first_kick + 4);
        while (pos != std::string::npos) {
            start.push_back(pos);
            pos = txt.find("START", pos + 5);
        };

        pos = first_kick;
        while (pos != std::string::npos) {
            ret += start.end() - std::lower_bound(start.begin(), start.end(), pos);
            pos = txt.find("KICK", pos + 3);
        };
    }

    return ret;
}

int main(int argc, char* argv[])
{
    // parse console input
    int no_of_cases;
    std::string input;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cin >> input;
        std::cout << "Case #" << g << ": " << kickstart(input) << "\n";
    }
}

/* clang++.exe -Wall -ggdb3 -O0 -std=c++17 kick_start.cpp -o kick_start.exe
kick_start.exe < kick_start.in

Input:

6
STARTKICK
AKICKSTARTPROBLEMNAMEDKICKSTART
STARTUNLUCKYKICK
KICKXKICKXSTARTXKICKXSTART
STARTJEBACPISKICKICKSTART

Output:

Case #1: 0
Case #2: 3
Case #3: 0
Case #4: 5
Case #5: 2
Case #6: 2

*/