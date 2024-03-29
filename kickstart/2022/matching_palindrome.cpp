
#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

// Matching Palindrome
// https://codingcompetitions.withgoogle.com/kickstart/round/00000000008cb0f5/0000000000ba82c5

class manacher_t {
  public:
    manacher_t(const std::string& s) {
        expand(s);
        manacher();
    }

    bool is_palindrome(const int begin, const int end) const { // [begin, end)
        return _radius[begin + end + 1] >= end - begin;
    }

    std::pair<int, int> longest() { // {offset, length}
        const auto it = std::max_element(_radius.begin(), _radius.end());
        auto pos = (it - _radius.begin() - *it - 1) / 2;
        auto len = *it;
        return {pos, len};
    }

  private:
    void expand(const std::string& s) {
        const int size = int(s.size());
        _exp_size = 2 * size + 3;
        _expanded.resize(_exp_size, '#');
        _expanded[0] = '$';
        _expanded.back() = '&';
        for (int i = 0; i < size; ++i)
            _expanded[2 * (i + 1)] = s[i];
    }

    void manacher() {
        int c{}, r{};
        _radius.resize(_exp_size, 0);
        // compute palindrome substrings map
        for (int i = 1; i < _exp_size - 1; ++i) {
            if (i < r) {
                const auto mirror = c - (i - c);
                _radius[i] = std::min(_radius[mirror], r - i);
            }

            while (_expanded[i - _radius[i] - 1] == _expanded[i + _radius[i] + 1])
                ++_radius[i];

            if (i + _radius[i] > r) {
                r = i + _radius[i];
                c = i;
            }
        }
    }

    int _exp_size;
    std::string _expanded;
    std::vector<int> _radius;
};

static void solve() { // O(N)
    int N;
    std::string S;
    std::cin >> N >> S;

    manacher_t m{S};
    for (int i = 1; i <= N; ++i)
        if (m.is_palindrome(0, i) && m.is_palindrome(i, N)) {
            std::cout << std::string_view(S.data(), i);
            return;
        }
}

/**
 * @brief Observation: if S is composed of N equal chunks, then adding one chunk is a solution
 * As chunks[0] == chunks[N-1] == reverse(chunks[N-1]) we know that chunk is a palindrome
 */
static void solve_chunks() { // O(NsqrtN)
    int N;
    std::string S;
    std::cin >> N >> S;

    for (int i = 1; i <= N; ++i) {
        if (N % i)
            continue;

        bool ok{true};
        const int z = N / i - 1;
        const auto chunk = std::string_view(S.data(), i);
        for (int j = 1; j <= z; ++j)
            if (std::string_view(S.data() + i * j, i) != chunk) {
                ok = false;
                break;
            }

        if (ok) {
            std::cout << chunk;
            return;
        }
    }
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
clang++.exe -Wall -Wextra -g -O0 -std=c++17 matching_palindrome.cpp -o matching_palindrome.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address matching_palindrome.cpp -o matching_palindrome

Run:
matching_palindrome.exe < matching_palindrome.in

Input:

3
4
abba
4
cccc
6
cdccdc

Output:

Case #1: abba
Case #2: c
Case #3: cdc

*/
