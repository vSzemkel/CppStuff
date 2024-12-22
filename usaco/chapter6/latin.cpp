/*
ID: marcin.8
LANG: C++
TASK: latin
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=q5IyqPflpNO&S=latin
*/

#include <algorithm>
#include <cassert>
#include <fstream>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::ifstream task_in("latin.in");
std::ofstream task_out("latin.out");

template<int N>
class positional_number_t
{
  public:
    positional_number_t(const int value = 0) : _value(value) {
        syncn2d();
    }

    positional_number_t(const std::vector<int>& digits) {
        assert(digits.size() <= N);
        int i = int(digits.size());
        for (const int d : digits)
            _digits[--i] = d;
        sync();
    }

    positional_number_t& operator=(const int value) {
        _value = value;
        syncn2d();
        return *this;
    }

    operator int() const {
        return _value;
    }

    int operator[](const int index) const {
        return _digits[index];
    }

    int& operator[](const int index) {
        return _digits[index];
    }

    int push_back(const int digit) {
        assert(0 <= digit && digit < 10);
        _value = 10 * _value + digit;
        for (int i = N - 1; i > 0; --i)
            _digits[i] = _digits[i - 1];
        _digits[0] = digit;
        return _value;
    }

    int pop_back(int count) {
        assert(0 <= count && count <= N);
        for (int i = count; i < N; ++i)
            _digits[i - count] = _digits[i];
        for (int i = N - 1; count; --i, --count) {
            _digits[i] = 0;
            _value /= 10;
        }
        return _value;
    }

    void sync() {
        _value = 0;
        for (int i = N - 1; ~i; --i)
            _value = 10 * _value + _digits[i];
    }

  private:
    void syncn2d() {
        auto tmp = _value;
        for (int& d : _digits) {
            d = tmp % 10;
            tmp /= 10;
        }
    }

    int _value{};
    int _digits[N] = {};
};

int N, required_rounds;
int64_t count_21star, ans{};
std::unordered_set<int> prefixes;
using num_t = positional_number_t<7>;
std::vector<num_t> numbers, rows, cols;
std::unordered_map<uint64_t, int64_t> memo;
static const int factorial[] = {1, 1, 2, 6, 24, 120, 720};

int generate() {
    std::vector<int> digits(N);
    std::iota(digits.begin(), digits.end(), 1);
    do {
        num_t num;
        int d = numbers.emplace_back(digits);
        while (d) {
            prefixes.insert(d);
            d /= 10;
        }
    } while (std::next_permutation(digits.begin(), digits.end()));

    return numbers.size();
}

auto prefix_range(const num_t& prefix)
{
    assert(prefix > 0);
    auto low = prefix;
    while (low[N - 1] == 0)
        low.push_back(0);
    auto high = prefix;
    while (high[N - 1] == 0)
        high.push_back(9);

    const auto beg = std::lower_bound(numbers.begin(), numbers.end(), low);
    return std::make_pair(beg, std::lower_bound(beg, numbers.end(), high));
}

bool try_set_cache(uint64_t& cache, const int digit, const int column)
{
    assert(0 < digit && digit < 8 && 0 <= column && column < 8);
    auto ptr = reinterpret_cast<uint8_t*>(&cache) + column;
    const int mask = 1 << (digit - 1);
    if (*ptr & mask)
        return false;
    *ptr |= mask;
    return true;
}

std::vector<int> valid_prefixes(const uint64_t cache, const int length)
{
    assert(1 < length && length < 8);
    auto ptr = reinterpret_cast<const uint8_t*>(&cache) + 1;
    std::vector<int> ret(1, length);

    auto no_such_digit = [](int number, int digit){
        for (; number; number /= 10)
            if (number % 10 == digit)
                return false;
        return true;
    };
    for (int pos = 1; pos < length; ++pos, ++ptr) {
        std::vector<int> tmp;
        for (const auto p : ret)
            for (int d = 1, m = 1; d <= N; ++d, m <<= 1)
                if ((*ptr & m) == 0 && no_such_digit(p, d))
                    tmp.push_back(10 * p + d);
        std::swap(ret, tmp);
    }

    return ret;
}

/**
 * Observation1: if we find acceptable latin square with sorted rows, we can have (N - 1)! variants of it by permuting all rows but the first one.
 * Observation2: Acceptable, row-sorted squares have left column fixed and equal to top row
 * Observation3: Correctly filled N-1 square can be extended to full N size in only one way
 * Observation4: Computation for second row can by optimized, see coment inline below
 */
void inner_solve(const int round, uint64_t cache)
{
    if (round == required_rounds) {
        ++ans;
        return;
    }

    const int cur = round / 2;
    if (round & 1) { // fill column
        const auto old_col = cols[cur];
        const auto [low, high] = prefix_range(old_col);
        for (auto it = low; it != high; ++it) {
            bool valid{true};
            for (int r = cur + 1; r < N; ++r)
                if (!prefixes.count(rows[r] * 10 + (*it)[N - 1 - r]))
                    valid = false;

            if (valid) {
                cols[cur] = *it;
                for (int r = cur + 1; r < N; ++r)
                    rows[r].push_back((*it)[N - 1 - r]);
                inner_solve(round + 1, cache);
                for (int r = cur + 1; r < N; ++r)
                    rows[r].pop_back(1);
            }
        }

        cols[cur] = old_col;
    } else { // fill row
        const auto old_row = rows[cur];
        const auto [low, high] = prefix_range(old_row);
        for (auto it = low; it != high; ++it) {
            if (round == 2) {
                // Observation4 : optimization for second row
                // 21* is fewer then 23* because for 21* digit 3 can not be anywhere in *, for 23* digit 1 can be anywhere
                // 23* and 24* are symmetric in any mean, the same hold true for 25* .. 2N*
                // Answer = count(21*) + count(23*) * (N - 2)
                if (count_21star == 0 && (*it)[N - 2] == 3)
                    count_21star = ans;
                else if ((*it)[N - 2] == 4) {
                    ans = count_21star + (ans - count_21star) * (N - 2);
                    break;
                }
            }
            bool valid{true};
            auto ncache = cache;
            for (int c = 0; c < N; ++c)
                valid &= try_set_cache(ncache, (*it)[N - 1 - c], c);

            if (valid) {
                const auto found = memo.find(ncache);
                //if (false)
                if (found != memo.end())
                    ans += found->second;
                else {
                    rows[cur] = *it;
                    for (int c = cur; c < N; ++c)
                        cols[c].push_back((*it)[N - 1 - c]);
                    const auto saved_ans = ans;
                    inner_solve(round + 2, ncache);
                    memo[ncache] = ans - saved_ans;
                    for (int c = cur; c < N; ++c)
                        cols[c].pop_back(1);
                }
            }
        }

        rows[cur] = old_row;
    }
}

int main(int, char**)
{
    task_in >> N;

    generate();
    rows.resize(N);
    cols.resize(N);
    required_rounds = 2 * (N - 1);

    const num_t seed = numbers[0];
    rows[0] = cols[0] = seed;
    for (int c = 1; c < N; ++c)
        rows[c] = cols[c] = seed[N - 1 - c];

    uint64_t cache{};
    for (int d = 0; d < N; ++d)
        try_set_cache(cache, d + 1, d);

    inner_solve(2, cache);

    task_out << ans * factorial[N - 1] << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++20 latin.cpp -o latin.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address latin.cpp -o latin

Run:
latin.exe && type latin.out

Input:

7

Output:

12198297600

*/
