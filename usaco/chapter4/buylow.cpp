/*
ID: marcin.8
LANG: C++
TASK: buylow
PROBLEM STATEMENT: https://train.usaco.org/usacoprob2?a=Dk29JqmgAzp&S=buylow
*/

#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <numeric>
#include <unordered_map>
#include <set>
#include <vector>

std::ifstream task_in("buylow.in");
std::ofstream task_out("buylow.out");

struct bignum_t {
    bignum_t(int64_t n = 0) {
        if (n == 0)
            _bignum.push_back(0);
        while (n) {
            _bignum.push_back(n % 10);
            n /= 10;
        }
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
        for (const auto d : _bignum)
            ret.push_back(d + '0');
        std::reverse(ret.begin(), ret.end());
        return ret;
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

  private:
    std::string _bignum;
};

static void working_suboptimal()
{
    int size; task_in >> size;
    std::vector<int> input(size);
    for (auto& n : input)
        task_in >> n;

    auto order = input;
    std::sort(order.begin(), order.end());
    order.erase(std::unique(order.begin(), order.end()), order.end());
    int distinct = int(order.size());
    std::unordered_map<int, int> rev_order;
    for (int i = 0; i < distinct; ++i)
        rev_order[order[i]] = i;
    order.clear();

    // dp[i][j] holds number of decreasing sequences of length i ending with order[j]
    std::vector<std::vector<bignum_t>> dp(2, std::vector<bignum_t>(distinct));
    std::vector<int> tails, locations(1), prev(size, -1);
    tails.push_back(input.front());
    dp[1][rev_order[tails.back()]] = 1;
    for (int i = 1; i < size; ++i) {
        const auto val = input[i];
        const auto rev = rev_order[val];
        dp[1][rev] = 1;
        if (val < tails.back()) {
            prev[i] = locations.back();
            tails.push_back(val);
            locations.push_back(i);
            dp.emplace_back(distinct);
        } else {
            const auto it = std::lower_bound(tails.begin(), tails.end(), val, [](const int e, const int v){
                return v < e;
            });
            *it = val;
            const int ind = it - tails.begin();
            locations[ind] = i;
            prev[i] = (ind > 0) ? locations[ind - 1] : -1;
        }

        for (int len = 2; len <= int(tails.size()); ++len) {
            const auto& shrt = dp[len - 1];
            auto& cur = dp[len][rev]; cur = 0;
            for (int i = rev + 1; i < distinct; ++i) {
                cur += shrt[i];
                if (bignum_t{} < dp[len][i]) {
                    cur += dp[len][i];
                    break;
                }
            }
        }
    }

    const int length = locations.size();
    bignum_t sum;
    for (const auto& c : dp[length])
        sum += c;
    task_out << length << ' ' << sum.to_string() << std::endl;
}

int main(int, char**)
{
    int size; task_in >> size;
    std::vector<int> input(size);
    for (auto& n : input)
        task_in >> n;

    /**
     * @brief for each number, calculate the maximum length that 
     * a valid sequence ending there can have
     */
    std::vector<int> len(size, 1);
    for (int r = 1; r < size; ++r) {
        auto& max = len[r];
        for (int l = 0; l < r; ++l)
            if (input[l] > input[r])
                max = std::max(max, len[l] + 1);
    }

    /**
     * @brief calculate the number of unique sequences ending at each location
     * that are the maximum length that a sequence ending there can be
     */
    std::vector<bignum_t> count(size);
    for (int r = 0; r < size; ++r)
        if (len[r] == 1)
            count[r] = 1;
        else {
            auto& cnt = count[r];
            int last = -1, need{len[r] - 1};
            for (int l = r - 1; l >= 0; --l) {
                if (input[l] > input[r] && len[l] == need && input[l] != last) {
                    cnt += count[l];
                    last = input[l];
                }
            }
        }

    int last = -1;
    bignum_t sum{};
    const auto max_len = *std::max_element(len.begin(), len.end());
    for (int r = size - 1; r >= 0; --r)
        if (len[r] == max_len && input[r] != last) {
            sum += count[r];
            last = input[r];
        }

    task_out << max_len << ' ' << sum.to_string() << std::endl;
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 buylow.cpp -o buylow.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address buylow.cpp -o buylow

Input:

20 
1 16 17 18 20 10 22 22 8 17 26 14 3 24 8 1 2 21 2 17 

Output:

5 20

*/
