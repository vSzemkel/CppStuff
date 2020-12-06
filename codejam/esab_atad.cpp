
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

// ESAb ATAd
// https://codingcompetitions.withgoogle.com/codejam/round/000000000019fd27/0000000000209a9e

constexpr int g_chunk_size = 10;
constexpr int g_init_question_count = 150;

bool g_val;
int g_size;
int g_pos;
int g_chunks_needed;
int g_question_count;
std::ofstream g_debug("esab_atad.info");
std::vector<std::pair<std::vector<bool>, std::vector<bool>>> g_flags; // {left, right}

std::vector<bool> flip(const std::vector<bool>& flag)
{
    const int size = flag.size();
    std::vector<bool> ret(size);
    for (int i = 0; i < size; ++i)
        ret[i] = !flag[i];
    return ret;
}

std::vector<bool> reverse(const std::vector<bool>& flag)
{
    auto ret = flag;
    std::reverse(ret.begin(), ret.end());
    return ret;
}

std::string to_string(const std::vector<bool>& flag)
{
    const int size = flag.size();
    std::string ret(size, '0');
    for (int i = 0; i < size; ++i)
        if (flag[i]) ret[size - i - 1] = '1';
    return ret;
}

std::vector<bool> to_vecbool(const std::string& flag)
{
    const int size = flag.size();
    std::vector<bool> ret(size);
    for (int i = 0; i < size; ++i)
        if (flag[i] != '0') ret[size - i - 1] = true;
    return ret;
}

bool is_palindrome(const std::vector<bool>& flag)
{
    const int size = flag.size();
    for (int i = 0; i < size; ++i)
        if (flag[i] != flag[size - i - 1]) return false;
g_debug << "found palindrome " << " " << to_string(flag) << "\n";
    return true;
}

std::vector<bool> read_chunk()
{
    std::vector<bool> ret(g_chunk_size);
    for (int i = g_pos; i < g_pos + g_chunk_size; ++i) {
        std::cout << g_size - i << std::endl;
        std::cin >> g_val;
        g_debug << i << " -> " << g_val << std::endl;
        ret[i] = g_val;
    }
    g_question_count -= g_chunk_size;
    return ret;
}

std::vector<bool> read_associate(const std::vector<bool>& flag)
{
    const int half_size = g_chunk_size / 2;
    const auto flag_flipped = flip(flag);
    std::string pattern_low = to_string(flag).substr(half_size);
    std::string pattern_low_flipped = to_string(flag_flipped).substr(half_size);
    std::string pattern_high = to_string(flag).substr(0, half_size);
    std::string pattern_high_flipped = to_string(flag_flipped).substr(0, half_size);
    std::string result_low, result_high;

    std::vector<bool> candidate_low1(half_size);
    for (int i = g_pos; i < g_pos + half_size; ++i) {
        std::cout << g_size - i << std::endl;
        std::cin >> g_val;
        g_debug << i << " ll => " << g_val << std::endl;
        candidate_low1[i] = g_val;
    }
    g_debug << "candidate_low1: " << to_string(candidate_low1) << std::endl;

    std::vector<bool> candidate_low2(half_size);
    for (int i = g_pos; i < g_pos + half_size; ++i) {
        std::cout << i + 1 << std::endl;
        std::cin >> g_val;
        g_debug << g_size - i - 1 << " lh => " << g_val << std::endl;
        candidate_low2[i - g_pos] = g_val;
    }
    g_debug << "candidate_low2: " << to_string(candidate_low2) << std::endl;

    auto lo = to_string(candidate_low1);
    if (lo == pattern_low)
        result_low = to_string(candidate_low2);
    else if (lo == pattern_low_flipped)
        result_low = to_string(flip(candidate_low2));
    else {
        auto lo = to_string(candidate_low2);
        if (lo == pattern_low)
            result_low = to_string(candidate_low1);
        else if (lo == pattern_low_flipped)
            result_low = to_string(flip(candidate_low1));
    }
    g_debug << "result_low: " << result_low << std::endl;

    std::vector<bool> candidate_high1(half_size);
    for (int i = g_pos + half_size; i < g_pos + g_chunk_size; ++i) {
        std::cout << g_size - i << std::endl;
        std::cin >> g_val;
        g_debug << i << " hl => " << g_val << std::endl;
        candidate_high1[i - g_pos - half_size] = g_val;
    }
    g_debug << "candidate_high1: " << to_string(candidate_high1) << std::endl;

    std::vector<bool> candidate_high2(half_size);
    for (int i = g_pos + half_size; i < g_pos + g_chunk_size; ++i) {
        std::cout << i + 1 << std::endl;
        std::cin >> g_val;
        g_debug << g_size - i - 1 << " hh => " << g_val << std::endl;
        candidate_high2[i - g_pos - half_size] = g_val;
    }
    g_debug << "candidate_high2: " << to_string(candidate_high2) << std::endl;

    auto hi = to_string(candidate_high1);
    if (hi == pattern_high)
        result_high = to_string(candidate_high2);
    else if (hi == pattern_high_flipped)
        result_high = to_string(flip(candidate_high2));
    else {
        hi = to_string(candidate_high2);
        if (hi == pattern_high)
            result_high = to_string(candidate_high1);
        else if (hi == pattern_high_flipped)
            result_high = to_string(flip(candidate_high1));
    }
    g_debug << "result_high: " << result_high << std::endl;

    return to_vecbool(result_high + result_low);
}

void read_chunk_pair(int& chunks_needed)
{
    const std::vector<bool> right = read_chunk();
    if ((chunks_needed % 2) == 1) {
        g_flags.emplace_back(right, right);
    } else {
        const auto left = read_associate(right); 
        g_debug << "read_chunk_pair: " << to_string(left) << std::endl;
        g_flags.emplace_back(left, right);
        --chunks_needed;
    }

    --chunks_needed;
}

std::vector<std::vector<bool>> multiplex()
{
    int chunks_needed = g_chunks_needed;
    std::vector<std::vector<bool>> ret(1);
    if (chunks_needed % 2 == 1) {
        const std::vector<bool>& orig = g_flags.back().first;
        auto chunk = orig;
        ret[0] = chunk;
        const auto flpd = flip(chunk);
        ret.push_back(flpd);
        if (!is_palindrome(chunk)) {
            chunk = reverse(chunk);
            if (chunk != flpd)
                ret.push_back(chunk);
            chunk = flip(chunk);
            if (chunk != flpd && chunk != orig)
            ret.push_back(chunk);
        }
        --chunks_needed;
        g_flags.pop_back();
    }
    std::unordered_set<std::vector<bool>> tmp;
    while (chunks_needed > 0) {
        auto& [left, right] = g_flags.back();
        const auto leftflip = flip(left);
        const auto rightflip = flip(right);
        const std::string sleft = to_string(left);
        const std::string sleftflip = to_string(leftflip);
        const std::string sright = to_string(right);
        const std::string srightflip = to_string(rightflip);
        for (const auto& c : ret) {
            const std::string smid = to_string(c);
            tmp.insert(to_vecbool(sleft + smid + sright));
            tmp.insert(to_vecbool(sright + smid + sleft));
            tmp.insert(to_vecbool(sleftflip + smid + sright));
            tmp.insert(to_vecbool(sright + smid + sleftflip));
            tmp.insert(to_vecbool(sleft + smid + srightflip));
            tmp.insert(to_vecbool(srightflip + smid + sleft));
            tmp.insert(to_vecbool(sleftflip + smid + srightflip));
            tmp.insert(to_vecbool(srightflip + smid + sleftflip));
            if (!is_palindrome(left)) {
                const std::string sleftreve = to_string(reverse(left));
                const std::string sleftflipreve = to_string(reverse(leftflip));
                tmp.insert(to_vecbool(sleftreve + smid + sright));
                tmp.insert(to_vecbool(sleftflipreve + smid + sright));
                tmp.insert(to_vecbool(sleftreve + smid + srightflip));
                tmp.insert(to_vecbool(sleftflipreve + smid + srightflip));
            }
            if (!is_palindrome(right)) {
                const std::string srightreve = to_string(reverse(right));
                const std::string srightflipreve = to_string(reverse(rightflip));
                tmp.insert(to_vecbool(srightreve + smid + sleft));
                tmp.insert(to_vecbool(srightreve + smid + sleftflip));
                tmp.insert(to_vecbool(srightflipreve + smid + sleft));
                tmp.insert(to_vecbool(srightflipreve + smid + sleftflip));
            }
        }
        chunks_needed -= 2;
        g_flags.pop_back();
        ret.assign(tmp.begin(), tmp.end());
    }

    return ret;
}

void print(const char* msg)
{
    g_debug << msg << std::endl;
    for (auto& f : g_flags)
        g_debug << to_string(f.first) << " " << to_string(f.second) << std::endl;
}

void print_candidates(const char* msg, const std::vector<std::vector<bool>>& cand)
{
    g_debug << msg << std::endl;
    if (cand.empty())
        g_debug << "empty" << std::endl;
    std::vector<std::string> vecstr;
    std::transform(cand.begin(), cand.end(), std::back_inserter(vecstr), [](const auto& vb){return to_string(vb);});
    std::sort(vecstr.begin(), vecstr.end());
    for (auto& s : vecstr)
        g_debug << s << std::endl;
}

void fake_answer()
{
    char fake;
    std::cout << std::string(g_size, '0') << std::endl;
    std::cin >> fake;
    g_debug << "answer is " << fake << std::endl << std::endl;
    exit(0);
}

void solve() {
    g_pos = 0;
    g_flags.clear();
    g_question_count = g_init_question_count;

    int chunks_needed = g_chunks_needed;
    while (chunks_needed > 0) {
        read_chunk_pair(chunks_needed);
        print("After read_chunk_pair");
        g_pos += g_chunk_size;
    }

    auto candidates = multiplex();
    print_candidates("After multiplex", candidates);

    int shift{0};
    while (candidates.size() > 1) {
        for (g_pos = shift; g_pos < g_size; g_pos += g_chunk_size) {
            if (g_question_count-- == 0) {
                g_debug << "no questions left" << std::endl;
                goto give_answer;
            }
            std::cout << g_size - g_pos << std::endl;
            std::cin >> g_val;
            const auto new_end = std::remove_if(candidates.begin(), candidates.end(), [](const auto& flag){ return flag[g_pos] != g_val; });
            candidates.erase(new_end, candidates.end());
            g_debug << "after preserving " << g_val << " on position " << g_pos << " remains " << candidates.size() << std::endl;
            print_candidates("After elimination", candidates);
        }
        shift = (shift + 1) % g_chunk_size;
    }

    if (candidates.empty())
        fake_answer();

give_answer:
    char answer;
    std::cout << to_string(candidates[0]) << std::endl;
    std::cin >> answer;
    g_debug << "answer for " << to_string(candidates[0]) << " is " << answer << std::endl << std::endl;
    if (answer != 'Y')
        exit(1);
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases >> g_size;
    g_chunks_needed = g_size / g_chunk_size;
    for (int g = 1; g <= no_of_cases; ++g) {
        g_debug << "\nCASE: " << g << std::endl;
        solve();
    }
}

/*
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 esab_atad.cpp -o esab_atad.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 esab_atad.cpp -o esab_atad.o
py.exe interactive_runner.py py.exe esab_atad_testing_tool.py 1 -- esab_atad.exe


Interactive tool: esab_atad_testing_tool.py


*/
