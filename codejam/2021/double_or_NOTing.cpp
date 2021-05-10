
#include <algorithm>
#include <assert.h>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <string_view>
#include <vector>

// Double_or_NOTing
// https://codingcompetitions.withgoogle.com/codejam/round/00000000004362d7/00000000007c1139#problem

constexpr int INF = 1'000'000'000;

static std::string dub(const std::string& s) {
    auto ret = s;
    if (!ret.empty() && ret != "0")
        ret.push_back('0');
    return ret;
}

static std::string neg(const std::string& s) {
    if (s.empty())
        return "1";

    int pos{0}, len{int(s.size())};
    while (pos < len && s[pos] == '1') ++pos;
    std::string ret(s.begin() + pos, s.end());
    if (ret.empty())
        return "0";
    else
        for (char& c : ret)
            c = (c == '0') ? '1' : '0';

    return ret;
}

// max length of proper s sufix equal to e prefix
static int common(const std::string& s, const std::string& e) {
    const auto ssize = int(s.size());
    for (int i, len = std::min(ssize, int(e.size())); len >= 0; --len) {
        for (i = 0; i < len; ++i)
            if (s[ssize - len + i] != e[i])
                break;
        if (i == len && (len == 0 || len == ssize || s[ssize - len] != s[ssize - len - 1]))
            return len;
    }

    return 0;
}

// how many consecutive occurences of s.front()
static int head_count(std::string_view s) {
    if (s.empty())
        return 0;

    int ret{0};
    const char c  = s.front();
    while (ret < int(s.size()) && s[ret] == c)
        ++ret;
    return ret;
}

// how many consecutive occurences of s.back()
static int tail_count(std::string_view s) {
    if (s.empty())
        return 0;

    const char c  = s.back();
    int ret{int(s.size()) - 2};
    while (0 <= ret && s[ret] == c)
        --ret;
    return s.size() - ret - 1;
}

// how many different bit groups
static int group_count(std::string_view s) {
    if (s.empty())
        return 0;

    int ret{1};
    char prev{s.front()};
    for (const char c : s)
        if (c != prev) {
            ++ret;
            prev = c;
        }

    return ret;
}

// cost "0" from s
static int to_zero(std::string_view s) {
    return group_count(s);
}

// cost of constructing e from "0"
static int from_zero(std::string_view e) {
    assert(!e.empty());
    if (e == "0") return 0;
    if (e == "1") return 1;

    const int gc = group_count(e);
    if (gc < 3) {
        auto ans = int(e.size());
        if (head_count(e) > 1) // neg + dub * size + neg
            ans += 2;
        return ans;
    }

    return INF;
}

static void solve() {
    std::string S, E; std::cin >> S >> E;

    int dtot{0}, ntot{0};
    int fz = from_zero(E);
    int tz = to_zero(S);
    if (S == "0")
        ntot = fz;
    else if (E == "0")
        ntot = tz;
    else if (S != E) {
        auto prevS = S;
        int cmn = common(S, E);
        int negcmn = common(neg(S), E);
        int egc = group_count(E);
        int max_neg = group_count(S) + 1;
        while (max_neg >= 0) {
            int incd{0}, incn{0};
            std::string winningS;
            for (int m1 = 0; m1 < 2; ++m1)
                for (int m2 = 0; m2 < 2; ++m2)
                    for (int m3 = 0; m3 < 2; ++m3) {
                        int d{0}, n{0};
                        auto SS = prevS;
                        if (m1 & 1) {++d; SS = dub(SS); } else {++n; SS = neg(SS); }
                        if (SS == E) { dtot += d; ntot += n; goto found; }
                        if (m2 & 1) {++d; SS = dub(SS); } else {++n; SS = neg(SS); }
                        if (SS == E) { dtot += d; ntot += n; goto found; }
                        if (m3 & 1) {++d; SS = dub(SS); } else {++n; SS = neg(SS); }
                        if (SS == E) { dtot += d; ntot += n; goto found; }
                        if (group_count(SS) + 1 < egc) continue;
                        const auto cc = common(SS, E);
                        const auto nc = common(neg(SS), E);
                        const auto best = std::max(cmn, negcmn);
                        bool takeit{false};
                        if (cc > best || nc > best) // found more
                            takeit = true;
                        else if ((cc == best || nc == best) && SS.size() < prevS.size()) // or shorter
                            takeit = true;
                        if (takeit) {
                            if (!winningS.empty() && best == int(E.size()) && SS.size() > winningS.size())
                                continue;
                            winningS = SS;
                            incd = d; incn = n;
                            cmn = cc; negcmn = nc;
                        }
                    }

            dtot += incd;
            ntot += incn;
            max_neg -= std::max(incn, 1);
            prevS = winningS;
        }

        std::cout << "IMPOSSIBLE";
        return;
    }
found:
    ntot += dtot;
    ntot = std::min(ntot, tz + fz);
    std::cout << (ntot >= INF ? "IMPOSSIBLE" : std::to_string(ntot));
}

static void solve_bfs() {
    std::string S, E; std::cin >> S >> E;
    assert(S.size() < 16 && E.size() < 16);

    int16_t sb{0}, eb{0};
    for (const char c : S)
        sb = (sb << 1) + (c == '1' ? 1 : 0);
    for (const char c : E)
        eb = (eb << 1) + (c == '1' ? 1 : 0);

    const auto MAX = int16_t(std::numeric_limits<int16_t>::max());
    std::vector<int16_t> dist(MAX, -1);
    dist[sb] = 0;
    std::queue<int16_t> que;
    que.push(sb);
    while (!que.empty()) {
        const auto u = que.front(); que.pop();
        if (u < MAX / 2 && dist[u * 2] == -1) {
            dist[u * 2] = dist[u] + 1;
            que.push(u * 2);
        }
        int mask = 2;
        while (mask <= u)
            mask <<= 1;
        const auto negu = int16_t((mask - 1) ^ u);
        if (dist[negu] == -1) {
            dist[negu] = dist[u] + 1;
            que.push(negu);
        }
    }

    if (dist[eb] == -1)
        std::cout << "IMPOSSIBLE";
    else {
        int a = dist[eb];
        std::cout << a;
    }
}

static void solve_wrong() {
    std::string S, E; std::cin >> S >> E;

    int ans = INF;
    int fz = from_zero(E);
    int tz = to_zero(S);
    if (S == "0") 
        ans = fz;
    else if (E == "0")
        ans = tz;
    else {
        const int e_size = int(E.size());
        const int gcs = group_count(S);
        int d{0}, n{0};
        for (int r = 0; r <= gcs; ++r) {
            int cmn = common(S, E);
            int negcmn = common(neg(S), E);
            std::string_view tail(E.c_str() + cmn, e_size - cmn);
            // try dubs
            if (!tail.empty() && tail.front() == '0' && S != "0" && negcmn < cmn) {
                const int zhc = head_count(tail);
                const auto addition = std::string(zhc, '0');
                d += zhc;
                cmn += zhc;
                S += addition;
                negcmn = common(neg(S), E);
                tail = std::string_view(E.c_str() + cmn, e_size - cmn);
            }
            // completion check
            if (S == E) {
                ans = std::min(ans, r + n + d);
                break;
            } else if (neg(S) == E) {
                ans = std::min(ans, r + n + d + 1);
                break;
            }
            const int headGroups = group_count(std::string_view(S.c_str(), S.size() - cmn));
            const int tailGroups = group_count(tail);
            const int tcs = tail_count(S);
            const int hct = head_count(tail);
            const int start_cmn = int(S.size()) - cmn;
            if (S.back() == '1' && (cmn == 0 || S[start_cmn - 1] != S[start_cmn]) && headGroups == tailGroups) {
                ans = std::min(ans, r + n + d + int(tail.size()) + tailGroups);
                break;
            }
            if (S.back() == '0' && (cmn == 0 || S[start_cmn - 1] != S[start_cmn]) && headGroups == tailGroups + 1) {
                if (tcs < hct)
                    ans = std::min(ans, r + n + d + int(tail.size()) - tcs + tailGroups);
                else
                    ans = std::min(ans, r + n + d + int(tail.size()) + tailGroups + 1);
                break;
            }

            std::string addition;
            if (tcs < hct && cmn < hct) 
                addition.assign(hct - tcs, S.back());
            else if (S.back() == tail.front() && 0 < cmn)
                addition.assign(hct, S.back());
            else if (negcmn <= cmn && cmn < e_size && (0 < cmn || tcs != hct))
                addition.assign(hct, 1 + 2 * '0' - S.back());

            S += addition;
            d += int(addition.size());

            S = neg(S);
        }
        ans = std::min(ans, tz + fz);
    }

    std::cout << (ans == INF ? "IMPOSSIBLE" : std::to_string(ans));
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    // assert(neg("0") == "1");
    // assert(neg("1") == "0");
    // assert(dub("0") == "0");
    // assert(dub("1") == "10");
    // assert(dub("1001") == "10010");
    // assert(neg("1001") == "110");

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; solve_bfs(); std::cout << '\n';
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 double_or_NOTing.cpp -o double_or_NOTing.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address double_or_NOTing.cpp -o double_or_NOTing.o

Run:
py.exe interactive_runner.py py.exe double_or_NOTing_testing_tool.py 1 -- double_or_NOTing.exe
double_or_NOTing.exe < double_or_NOTing.in

Input:

9
111 0
10001 111
1011 111
1010 1011
0 1
0 101
1010111 1010110
1010111 111
1101011 1101011

Output:

Case #1: 1
Case #2: 4
Case #3: 3
Case #4: 2
Case #5: 1
Case #6: IMPOSSIBLE
Case #7: 12
Case #8: 4
Case #9: 0

*/
