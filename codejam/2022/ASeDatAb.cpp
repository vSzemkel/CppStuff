
#include <algorithm>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

// ASeDatAb
// https://codingcompetitions.withgoogle.com/codejam/round/000000000087711b/0000000000acd29b

const auto rand_in_range = [](const int64_t ubound){std::random_device seed;std::mt19937 gen{seed()};std::uniform_int_distribution<int64_t> dist(0,ubound-1);return dist(gen);};

constexpr const int L = 8;
constexpr const int SZ = (1 << L);
static const auto all0 = std::string(L, '0'), all1 = std::string(L, '1');

// rotate N bits
static int rot(const int v, const int r) { // rotl iff r>0
    const auto s = ((r % L) + L) % L;
    if (!s) return v;
    return ((v << s) | (v >> (L - s))) & ((1 << L) - 1);
}

template <typename T = int>
static std::string binstr(const T val) { // 5 -> "00000101", when L == 8
    std::string ret(L, '0');
    for (T p = 0, m = 1 << (L - 1); m; m >>= 1, ++p)
        if (val & m) ret[p] = '1';
    return ret;
}

static std::string rndstr() {
    std::string ret(L, '0');
    for (int p = 0; p < L; ++p)
        ret[p] = (rand_in_range(2) & 1) + '0';
    return ret;
}

/**
 * PHASE 1 seed 11111111
 * 11111111 00000000 
 * PHASE 2 seed 01010101
 * 01010101 00000000 10101010 
 * PHASE 3 seed 00110011
 * 00110011 00000000 01100110 01010101 10011001 11001100 
 * PHASE 4 seed 00010001
 * 00010001 00000000 00100010 01000100 01110111 01100110 10001000 10111011 10101010 11011101 11001100 11101110 
 * PHASE 5 seed 00001111
 * 00001111 00000000 00011110 00010001 00101101 00100010 00111100 00110011 01001011 01000100 01011010 01010101 01101001 01100110 01111000 01110111 10000111 10010110 10100101 10110100 11000011 11010010 11100001 11110000 
 * PHASE 6 seed 00000101
 * 00000101 00000000 00001010 00010100 00010001 00011011 00100111 00100010 00101000 00110110 00110011 00111001 01000001 01001110 01001011 01010000 01011111 01011010 01100011 01101100 01101001 01110010 01111101 01111000 10000010 10001101 10001000 10010011 10011100 10011001 10100000 10101111 10101010 10110001 10111110 10111011 11000110 11000011 11001001 11010111 11010010 11011000 11100100 11100001 11101011 11110101 11110000 11111010 
 * PHASE 7 seed 00000011
 * 00000011 00000000 00000110 00000101 00001001 00001100 00010010 00010001 00010111 00010100 00011000 00011101 00100001 00100100 00101011 00101000 00101110 00101101 00110000 00110101 00111010 00111001 00111111 00111100 01000010 01000001 01000111 01000100 01001000 01001101 01010011 01010000 01010110 01010101 01011001 01011100 01100000 01100101 01101010 01101001 01101111 01101100 01110001 01110100 01111011 01111000 01111110 01111101 10000001 10000100 10001011 10001000 10001110 10001101 10010000 10010101 10011010 10011001 10011111 10011100 10100011 10100000 10100110 10100101 10101001 10101100 10110010 10110001 10110111 10110100 10111000 10111101 11000000 11000101 11001010 11001001 11001111 11001100 11010001 11010100 11011011 11011000 11011110 11011101 11100010 11100001 11100111 11100100 11101000 11101101 11110011 11110000 11110110 11110101 11111001 11111100 
 * PHASE 8 seed 00000001
 * 00000001 00000000 00000010 00000100 00000111 00000110 00001000 00001011 00001010 00001101 00001100 00001110 00010000 00010011 00010010 00010101 00010100 00010110 00011001 00011000 00011010 00011100 00011111 00011110 00100000 00100011 00100010 00100101 00100100 00100110 00101001 00101000 00101010 00101100 00101111 00101110 00110001 00110000 00110010 00110100 00110111 00110110 00111000 00111011 00111010 00111101 00111100 00111110 01000000 01000011 01000010 01000101 01000100 01000110 01001001 01001000 01001010 01001100 01001111 01001110 01010001 01010000 01010010 01010100 01010111 01010110 01011000 01011011 01011010 01011101 01011100 01011110 01100001 01100000 01100010 01100100 01100111 01100110 01101000 01101011 01101010 01101101 01101100 01101110 01110000 01110011 01110010 01110101 01110100 01110110 01111001 01111000 01111010 01111100 01111111 01111110 10000000 10000011 10000010 10000101 10000100 10000110 10001001 10001000 10001010 10001100 10001111 10001110 10010001 10010000 10010010 10010100 10010111 10010110 10011000 10011011 10011010 10011101 10011100 10011110 10100001 10100000 10100010 10100100 10100111 10100110 10101000 10101011 10101010 10101101 10101100 10101110 10110000 10110011 10110010 10110101 10110100 10110110 10111001 10111000 10111010 10111100 10111111 10111110 11000001 11000000 11000010 11000100 11000111 11000110 11001000 11001011 11001010 11001101 11001100 11001110 11010000 11010011 11010010 11010101 11010100 11010110 11011001 11011000 11011010 11011100 11011111 11011110 11100000 11100011 11100010 11100101 11100100 11100110 11101001 11101000 11101010 11101100 11101111 11101110 11110001 11110000 11110010 11110100 11110111 11110110 11111000 11111011 11111010 11111101 11111100 11111110 
 */
static auto rotate_xor_monoid_seeds() {
    std::vector<std::string> ret;
    std::vector<bool> seen(SZ);
    seen[0] = true;
    while (ret.size() < L)
        for (int i = 1; i < SZ; ++i) {
            if (seen[i]) continue;
            bool unique = true;
            for (int r = 0; r < L; ++r)
                if (!seen[i ^ rot(i, r)]) {
                    unique = false;
                    break;
                }
            if (unique) {
                seen[i] = true;
                ret.push_back(binstr(i));
                for (int j = 0; j < SZ; ++j)
                    if (seen[j])
                        seen[j ^ i] = true;
                break;
            }
        }

    return ret;
}

/**
 * @brief Why this works? Magick factors are equal to rotate_xor_monoid_seeds
 * This solution is equivalent to solve_seeds
 */
static void solve_magick()
{
    int value;
    for (int i = 0; i < SZ; ++i) {
        if (i == 0) {
            value = 0x00;
        } else if (i % 2 == 1) {
            value = 0xff; // 0b11111111
        } else if (i % 4 == 2) {
            value = 0x55; // 0b01010101
        } else if (i % 8 == 4) {
            value = 0x33; // 0b00110011
        } else if (i % 16 == 8) {
            value = 0x11; // 0b00010001
        } else if (i % 32 == 16) {
            value = 0x0f; // 0b00001111
        } else if (i % 64 == 32) {
            value = 0x05; // 0b00000101
        } else if (i % 128 == 64) {
            value = 0x03; // 0b00000011
        } else {
            value = 0x01; // 0b00000001
        }

        std::cout << binstr(value) << std::endl;
        int N; std::cin >> N;
        if (!N) break;
    }
}

static void solve_seeds() {
    auto seeds = rotate_xor_monoid_seeds();
    for (int z = SZ - 1; z; --z) {
        std::cout << seeds[__builtin_ctz(z)] << std::endl;
        int N; std::cin >> N;
        if (!N) return;
    }
}

/**
 * @brief Constructive approach. Build a sequence that solves task for L == (1 << x)
 * If the input is in the form of XX, it has a form of YY after the rotation
 */
static void solve()
{
    std::vector<std::string> seq(1, "1");
    for (int tour = 1; (1 << tour) <= L; ++tour) {
        auto copy_zero = seq;
        for (auto& s : copy_zero)
            s += all0.substr(0, s.size());
        auto copy_dub = seq;
        for (auto& s : copy_dub)
            s += s;
        auto next = copy_dub;
        for (const auto& s : copy_zero) {
            next.push_back(s);
            next.insert(next.end(), copy_dub.begin(), copy_dub.end());
        }
        seq = std::move(next);
    }

    for (const auto& probe : seq) {
        std::cout << probe << std::endl;
        int N; std::cin >> N;
        if (!N) break;
    }
}

static void solve_rnd() // defeat randomness with randomness, solves Set1
{
    int N;

    do {
        std::cout << rndstr() << std::endl;
        std::cin >> N;
    } while (2 < N && N < L - 1);

    if (N > 4) {
        std::cout << all1 << std::endl;
        std::cin >> N;
    }

    if (N) {
        std::string s{all0};
        for (int z = N; z; --z)
            s[L - z] = '1';
        std::vector<std::string> cand;
        do {
            cand.push_back(s);
        } while (std::next_permutation(s.begin(), s.end()));

        while (true) {
            auto probe = cand[rand_in_range(cand.size())];
            std::cout << probe << std::endl;
            std::cin >> N;
            if (!N) break;

            std::unordered_set<std::string> next;
            for (int r = 0; r < L; ++r) {
                std::rotate(probe.begin(), probe.begin() + 1, probe.end());
                for (const auto& s : cand) {
                    int cnt{0};
                    auto n = s;
                    for (int i = 0; i < L; ++i)
                        if (s[i] == probe[i])
                            n[i] = '0';
                        else {
                            ++cnt;
                            n[i] = '1';
                        }
                    if (cnt == N)
                        next.insert(n);
                }
            }

            cand = std::vector<std::string>{next.begin(), next.end()};
        }
    }
}

int main(int, char**)
{
    rotate_xor_monoid_seeds();
    return 0;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g)
        solve_magick();
}

/*

Compile:
clang++.exe -Wall -Wextra -g3 -O0 -std=c++17 ASeDatAb.cpp -o ASeDatAb.exe
g++ -Wall -Wextra -g3 -Og -std=c++17 -fsanitize=address ASeDatAb.cpp -o ASeDatAb

Run:
py.exe interactive_runner.py py.exe ASeDatAb_testing_tool.py 0 -- ASeDatAb.exe

*/
