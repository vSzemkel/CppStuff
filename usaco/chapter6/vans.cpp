/*
ID: marcin.8
LANG: C++
TASK: vans
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=0nOb9wTUsiE&S=vans
*/

#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::ifstream task_in("vans.in");
std::ofstream task_out("vans.out");

struct state_t {
    int16_t row;
    int16_t column;
    int8_t avoid;
    bool operator==(const state_t& other) const {
        return row == other.row && column == other.column && avoid == other.avoid;
    }
} ;

struct bignum_t {
    bignum_t(int64_t n = 0) {
        if (n == 0)
            _bignum.push_back(0);
        while (n) {
            _bignum.push_back(n % 10);
            n /= 10;
        }
    }

    bignum_t(const std::string& s) {
        assert(!s.empty());
        _bignum.resize(s.size());
        std::transform(s.crbegin(), s.crend(), _bignum.begin(), [](const char d){ return d - '0'; });
    }

    bool operator==(const bignum_t& other) const {
        return _bignum == other._bignum;
    }

    bool operator<(const bignum_t& other) const {
        const auto sz = _bignum.size(), osz = other._bignum.size();
        if (sz < osz)
            return true;
        if (sz > osz)
            return false;
        if (_bignum == other._bignum)
            return false;
        // not equal but equal size
        int pos = sz;
        while (_bignum[pos] == other._bignum[pos])
            --pos;
        return _bignum[pos] < other._bignum[pos];
    }

    auto to_string() const {
        if (_bignum.empty())
            return std::string{"0"};
        std::string ret;
        ret.reserve(_bignum.size());
        std::transform(_bignum.crbegin(), _bignum.crend(), std::back_inserter(ret), [](const char d){ return d + '0'; });
        return ret;
    }

    auto operator+(const bignum_t& other) const {
        const auto sz = int(std::min(_bignum.size(), other._bignum.size()));
        std::string ret;
        ret.reserve(sz);
        int carry{0};
        for (int i = 0; i < sz; ++i) {
            ret.push_back(_bignum[i] + other._bignum[i] + carry);
            auto& last = ret.back();
            carry = last / 10;
            last %= 10;
        }

        const auto& tail = (_bignum.size() < other._bignum.size()) ? other._bignum : _bignum;
        const auto tsz = int(tail.size());
        for (int i = sz; i < tsz; ++i) {
            ret.push_back(tail[i] + carry);
            auto& last = ret.back();
            carry = last / 10;
            last %= 10;
        }

        if (carry)
            ret.push_back(carry);

        bignum_t wrapper;
        wrapper._bignum = std::move(ret);
        return wrapper;
    }

    void operator+=(const bignum_t& other) {
        int carry{0};
        const auto sz = int(std::min(_bignum.size(), other._bignum.size()));
        for (int i = 0; i < sz; ++i) {
            auto& cur = _bignum[i];
            const auto val = cur + other._bignum[i] + carry;
            cur = val % 10;
            carry = val / 10;
        }

        const auto& tail = (_bignum.size() < other._bignum.size()) ? other._bignum : _bignum;
        const auto tsz = int(tail.size());
        if (sz < tsz)
            _bignum.resize(tsz);
        for (int i = sz; i < tsz; ++i) {
            auto& cur = _bignum[i];
            const auto val = tail[i] + carry;
            cur = val % 10;
            carry = val / 10;
        }

        if (carry)
            _bignum.push_back(carry);
    }

    auto operator-(const bignum_t& other) const {
        if (_bignum == other._bignum)
            return bignum_t{};
        assert(other < *this);

        const auto sz = int(std::min(_bignum.size(), other._bignum.size()));
        std::string ret;
        ret.reserve(sz);
        int borrow{0};
        for (int i = 0; i < sz; ++i) {
            ret.push_back(_bignum[i] - other._bignum[i] - borrow);
            auto& last = ret.back();
            if (last < 0) {
                last += 10;
                borrow = 1;
            } else
                borrow = 0;
        }

        const auto& tail = (_bignum.size() < other._bignum.size()) ? other._bignum : _bignum;
        const auto tsz = int(tail.size());
        for (int i = sz; i < tsz; ++i) {
            ret.push_back(tail[i] - borrow);
            auto& last = ret.back();
            if (last < 0) {
                last += 10;
                borrow = 1;
            } else
                borrow = 0;
        }
        assert(borrow == 0);

        bignum_t wrapper;
        wrapper._bignum = std::move(ret);
        return wrapper;
    }

    bignum_t operator*(const bignum_t& other) const {
        const auto sz = _bignum.size(), osz = other._bignum.size();
        if (3 * sz < 2 * osz)
            return other * (*this);

        bignum_t ret{};
        ret._bignum.reserve(sz + osz);
        for (int i = 0; i < int(osz); ++i)
            ret = ret + digmul(other._bignum[i], i);

        return ret;
    }

    bignum_t operator^(bignum_t other) const {
        bignum_t base{*this}, ret{1}, zero{};
        while (zero < other) {
            if (other._bignum[0] & 1)
                ret = ret * base;
            base = base * base;
            other = other.digdiv(2);
        }

        return ret;
    }

  private:
    std::string _bignum;

    /**
     * @brief Multiply this by d and add p padding of least significant zeros to the result
     */
    bignum_t digmul(const char d, const int p) const {
        const auto sz = int(_bignum.size());
        std::string ret(p, 0);
        ret.reserve(sz);
        int carry{0};
        for (int i = 0; i < sz; ++i) {
            const auto v = _bignum[i] * d + carry;
            ret.push_back(v % 10);
            carry = v / 10;
        }
        if (carry > 0)
            ret.push_back(carry);

        bignum_t wrapper;
        wrapper._bignum = std::move(ret);
        return wrapper;
    }

    /**
     * @brief Divide this by d and discard the reminder
     */
    bignum_t digdiv(const char d) const {
        if (_bignum.empty())
            return bignum_t{};
        const auto sz = int(_bignum.size());
        std::string ret;
        ret.reserve(sz);
        int carry{0}, pos = sz - 1, msd = _bignum.back();
        if (msd < d) {
            carry = msd;
            --pos;
        }
        for (int i = pos; i >= 0; --i) {
            const auto cd = _bignum[i] + 10 * carry;
            ret.push_back(cd / d);
            carry = cd % d;
        }

        bignum_t wrapper;
        wrapper._bignum = std::move(ret);
        return wrapper; // reminder in carry
    }
};

bignum_t ans{};
bool before_right{true};
int SZ, ALL, count{1};
std::vector<bool> visited;
std::vector<int8_t> coll_fill_factor;
constexpr const int N{0}, E{1}, S{2}, W{3};
state_t final{ 0, 0, S };
constexpr const int8_t DR[] = {-1, 0, +1, 0, -1, 0, +1, 0}; // cycled: N, E, S, W, N, E, S, W
constexpr const int8_t DC[] = {0, +1, 0, -1, 0, +1, 0, -1};

void ride(const state_t s)
{
    if (count == ALL && s == final) {
        ans += 1;
        return;
    }

    ++count;
    ++coll_fill_factor[s.column];
    for (int z = 3, dir = (s.avoid + 1) % 4; z; --z, dir = (dir + 1) % 4) {
        const int8_t nr = s.row + DR[dir];
        const int16_t nc = s.column + DC[dir];
        const auto vertex = nr * SZ + nc;
        if (0 <= nr && nr < 4 && 0 <= nc && nc < SZ && !visited[vertex]) {
            if (before_right) {
                if (dir == E) {
                    if (nc == SZ - 1)
                        before_right = false;
                    if (nr == 1 && coll_fill_factor[s.column] == 1)
                        continue;
                } else if (dir == W && (coll_fill_factor[s.column] > 2 || nc == 0))
                    continue;
                else if ((dir == N || dir == S) && coll_fill_factor[s.column] == 4)
                    continue;
            } else if (coll_fill_factor[s.column] == 4) {
                if (dir == E)
                    continue;
                if (nc < SZ - 1 && (dir == N || dir == S) && coll_fill_factor[nc + 1] < 4)
                    continue;
            }

            visited[vertex] = true;
            ride(state_t{nr, nc, static_cast<int8_t>(dir + 2) });
            visited[vertex] = false;
        }
    }
    --coll_fill_factor[s.column];
    --count;
}

int main(int, char**)
{
    task_in >> SZ;
    if (SZ > 1) {
        if (SZ < 4) {
            ALL = 4 * SZ;
            visited.resize(ALL);
            coll_fill_factor.resize(SZ);
            visited[1] = true;
            if (SZ == 2)
                before_right = false;
            ride(state_t{ 0, 1, W });
            ans = ans * 2;
        } else { // kudos: wrwwctb
            std::vector<bignum_t> memo(SZ + 1);
            memo[1] = 2;
            memo[2] = 2;
            memo[3] = 8;
            ans = 4;
            for (int i = 4; i <= SZ; ++i) {
                memo[i] = ans * 2 + memo[i - 1] + memo[i - 2] - memo[i - 3];
                ans = ans + memo[i - 1];
            }
        }
    }

    task_out << ans.to_string() << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 vans.cpp -o vans.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address vans.cpp -o vans

Run:
vans.exe && type vans.out

Input:

14

Output:

125906

*/
