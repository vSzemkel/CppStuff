
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

// Hacked Exam
// https://codingcompetitions.withgoogle.com/codejam/round/000000000043585d/0000000000754750

struct fraction_t {
    inline bool is_zero() const { return num == 0 || denum == 0; }

    void reduce() {
        if (!is_zero()) {
            const auto gcd = std::gcd(num, denum);
            num /= gcd;
            denum /= gcd;
        }
    }

    void operator+=(const fraction_t& other) {
        if (!other.is_zero()) {
            if (is_zero()) {
                num = other.num;
                denum = other.denum;
            } else {
                num = num * other.denum + other.num * denum;
                denum *= other.denum;
                reduce();
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const fraction_t& f) {
        return os << f.num << '/' << f.denum;
    }

    int64_t num, denum;
};

static void solve_set1() {
    int N, Q; std::cin >> N >> Q;
    std::vector<int> scores(N);
    std::vector<std::string> answers(N);
    for (int i = 0; i < N; ++i)
        std::cin >> answers[i] >> scores[i];

    int big = 1 << Q;
    std::vector<int> candidates;
    for (int i = 0; i < big; ++i) {
        int j = 0;
        for (; j < N; ++j) {
            int score{0};
            for (int k = 0; k < Q; ++k) {
                bool ans = answers[j][k] == 'T';
                bool exp = i & (1 << k);
                if (ans == exp)
                    ++score;
            }
            if (score != scores[j])
                break;
        }
        if (j == N)
            candidates.push_back(i);
    }

    fraction_t p{};
    std::string answer(Q, '-');
    const int size = (int)candidates.size();
    for (int i = 0; i < Q; ++i) {
        int ca{0};
        for (const int j : candidates)
            if (j & (1 << i))
                ++ca;

        answer[i] = (2 * ca < size) ? 'F' : 'T';
        p += fraction_t{std::max(ca, size - ca), size};
    }

    std::cout << answer << " " << p;
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve_set1(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 hacked_exam.cpp -o hacked_exam.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address hacked_exam.cpp -o hacked_exam.o

Run:
hacked_exam.exe < hacked_exam.in

Input:

4
1 3
FFT 3
1 3
FFT 2
2 6
FFTTTF 2
FTFTFT 4
2 2
FF 1
TT 1

Output:

Case #1: FFT 3/1
Case #2: FFT 2/1
Case #3: TTFTFT 4/1
Case #4: TT 1/1

*/
