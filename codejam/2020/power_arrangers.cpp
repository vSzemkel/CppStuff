
#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

// Power Arrangers
// https://codingcompetitions.withgoogle.com/codejam/round/00000000000516b9/0000000000134e91

constexpr int g_size = 5;
int g_max_inspections;

/*void print_query(const std::vector<std::vector<int>>& query)
{
    for (int i = 0; i < query.size(); ++i) {
        std::cerr << (char)('A' + i) << " " << query[i].size() << "  [ ";
        for (const auto& pos : query[i])
            std::cerr << pos << " ";

        std::cerr << "]" << std::endl;
    }
}*/

constexpr int factorial(const int n)
{
    int answer = 1;
    for (int i = 2; i <= n; ++i)
        answer *= i;
    return answer;
}

char find_letter(std::vector<int>& consider, const int pos)
{
    char answer;
    std::array<std::vector<int>, g_size> answers;
    for (const int i : consider) {
        const int question = i + 1;
        std::cout << question << std::endl;
        std::cin >> answer;
        answers[answer - 'A'].push_back(question);
    }

    auto it = std::find_if(answers.begin(), answers.end(), [n = factorial(g_size - pos - 1) - 1](const auto& v) { return v.size() == n; });
    consider = std::move(*it);
    return 'A' + (it - answers.begin());
}

bool solve_case()
{
    // initialize
    std::string solution(g_size, '*');
    std::vector<int> consider(factorial(g_size) - 1);
    std::generate(consider.begin(), consider.end(), [n = -g_size]() mutable { n += g_size; return n; });

    // iterate for g_size - 2 times
    for (int i = 0; i < g_size - 2; ++i) {
        solution[i] = find_letter(consider, i);
        std::cerr << solution << std::endl;
    }

    // answer differs from consider, so swap last two chars
    const int question = consider[0] + 1;
    std::cout << question << std::endl;
    std::cin >> solution[g_size - 1];
    std::cerr << solution << std::endl;

    // find missing one
    for (int i = 0; i < g_size; ++i)
        if (solution.find('A' + i) == std::string::npos) {
            solution[g_size - 2] = 'A' + i;
            break;
        }

    // check the solution
    char answer;
    std::cout << solution << std::endl;
    std::cin >> answer;
    std::cerr << "My answer is: " << solution << ", judge answer is: " << answer << std::endl;
    return answer == 'Y';
}

int main(int argc, char* argv[])
{
    // parse console input
    int no_of_cases;
    std::cin >> no_of_cases >> g_max_inspections;
    std::cerr << "received: " << no_of_cases << " " << g_max_inspections << std::endl;
    // analyze cases
    for (int i = 0; i < no_of_cases; ++i) {
        std::cerr << "\nCase: " << i + 1 << std::endl;
        if (!solve_case()) break;
    }

    return 0;
}

/* clang++.exe -Wall -g -O0 -std=c++17 power_arrangers.cpp -o power_arrangers.exe

Test:

py.exe C:\GitHub\GCC-test\interactive_runner.py py.exe C:\GitHub\GCC-test\power_arrangers_testing_tool.py 1 -- C:\GitHub\GCC-test\power_arrangers.exe

Output:

...
  sol:
  sol: Case: 49
  sol: C****
  sol: CE***
  sol: CED**
  sol: CED*B
  sol: My answer is: CEDAB, judge answer is: Y
  sol:
  sol: Case: 50
  sol: C****
  sol: CB***
  sol: CBA**
  sol: CBA*E
  sol: My answer is: CBADE, judge answer is: Y

*/