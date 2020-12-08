
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
constexpr int g_check_cycle = (g_chunk_size - 2) >> 1;

bool g_val, g_reversed, g_flipped;
int g_size, g_pos, g_eq_pair, g_neq_pair, g_chunks_needed, g_question_asked;
std::ofstream g_debug("esab_atad.log");
std::vector<bool> g_answer;
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
    return true;
}

void print_associates(const char* msg)
{
    g_debug << msg << std::endl;
    for (auto& f : g_flags)
        g_debug << to_string(f.first) << " " << to_string(f.second) << std::endl;
}

void print_candidates(const char* msg, const std::vector<std::vector<bool>>& cand)
{
    int max_lines = 10;
    g_debug << msg << std::endl;
    if (cand.empty())
        g_debug << "empty" << std::endl;
    std::vector<std::string> vecstr;
    std::transform(cand.begin(), cand.end(), std::back_inserter(vecstr), [](const auto& vb){return to_string(vb);});
    std::sort(vecstr.begin(), vecstr.end());
    for (auto& s : vecstr) {
        g_debug << s << std::endl;
        if (--max_lines == 0) break;
    }
    if (max_lines == 0)
        g_debug << "..." << std::endl;
}

bool read_bit(const int pos)
{
    std::cout << g_size - pos << std::endl;
    ++g_question_asked;
    std::cin >> g_val;
    return g_val;
}

std::vector<bool> read(const int size = g_chunk_size, const bool reversed = false, const int offset = 0)
{
    std::vector<bool> ret(size);
    for (int i = g_pos + offset; i < g_pos + offset + size; ++i)
        ret[i - g_pos - offset] = read_bit(reversed ? g_size - i - 1 : i);
    return ret;
}

std::vector<bool> read_associate(const std::vector<bool>& flag)
{
    const int half_size = g_chunk_size >> 1;
    const auto flag_flipped = flip(flag);
    std::string pattern_low = to_string(flag).substr(half_size);
    std::string pattern_low_flipped = to_string(flag_flipped).substr(half_size);
    std::string pattern_high = to_string(flag).substr(0, half_size);
    std::string pattern_high_flipped = to_string(flag_flipped).substr(0, half_size);
    std::string result_low, result_high;

    std::vector<bool> candidate_low1 = read(half_size, false);
    g_debug << "candidate_low1: " << to_string(candidate_low1) << std::endl;
    auto lo = to_string(candidate_low1);
    if (lo != pattern_low && lo != pattern_low_flipped) { // can read optimally
        result_high = to_string(read(half_size, false, half_size));
        return to_vecbool(result_high + lo);
    }

    std::vector<bool> candidate_low2 = read(half_size, true);
    g_debug << "candidate_low2: " << to_string(candidate_low2) << std::endl;
    if (lo == pattern_low)
        result_low = to_string(candidate_low2);
    else if (lo == pattern_low_flipped)
        result_low = to_string(flip(candidate_low2));
    g_debug << "result_low: " << result_low << std::endl;

    std::vector<bool> candidate_high1 = read(half_size, false, half_size);
    g_debug << "candidate_high1: " << to_string(candidate_high1) << std::endl;
    std::vector<bool> candidate_high2 = read(half_size, true, half_size);
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
    const std::vector<bool> right = read();
    if ((chunks_needed % 2) == 1) {
        g_flags.emplace_back(right, right);
    } else {
        const auto left = read_associate(right); 
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
        tmp.clear();
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
        print_candidates("After multiplex", ret);
    }

    return ret;
}

bool reduce(std::vector<std::vector<bool>>& candidates)
{
    int shift{0};
    while (candidates.size() > 1) {
        for (g_pos = shift; g_pos < g_size; g_pos += g_chunk_size) {
            if (g_question_asked == g_init_question_count)
                return false;
            read_bit(g_pos);
            const auto new_end = std::remove_if(candidates.begin(), candidates.end(), [](const auto& flag){ return flag[g_pos] != g_val; });
            candidates.erase(new_end, candidates.end());
            g_debug << "after preserving " << g_val << " on position " << g_pos << " remains " << candidates.size() << std::endl;
            print_candidates("After elimination", candidates);
        }
        shift = (shift + 1) % g_chunk_size;
    }

    return true;
}

void fake_answer()
{
    char fake;
    std::cout << std::string(g_size, '0') << std::endl;
    std::cin >> fake;
    g_debug << "After " << g_question_asked << " questions fake answer is " << fake << std::endl << std::endl;
    exit(0);
}

void solve() {
    g_pos = 0;
    g_flags.clear();

    int chunks_needed = g_chunks_needed;
    while (chunks_needed > 0) {
        read_chunk_pair(chunks_needed);
        print_associates("After read_chunk_pair");
        g_pos += g_chunk_size;
    }

    auto candidates = multiplex();
    if (!reduce(candidates) || candidates.empty())
        fake_answer();

    char answer;
    std::cout << to_string(candidates[0]) << std::endl;
    std::cin >> answer;
    g_debug << "After " << g_question_asked << " questions, answer for " << to_string(candidates[0]) << " is " << answer << std::endl << std::endl;
    if (answer != 'Y')
        exit(1);
}

void read_pair()
{
    bool right = read_bit(g_pos);
    bool left = read_bit(g_size - g_pos - 1);
    if (g_reversed)
        std::swap(left, right);
    if (g_flipped) {
        left ^= true;
        right ^= true;
    }
    if (left == right)
        g_eq_pair = g_pos;
    else
        g_neq_pair = g_pos;
    g_answer[g_size - g_pos - 1] = left;
    g_answer[g_pos] = right;
}

void check_fluctuation()
{
    if (g_eq_pair == -1 && g_neq_pair == -1)
        read_pair();
    else if (g_eq_pair == -1 || g_neq_pair == -1) {
        auto known_pos = std::max(g_eq_pair, g_neq_pair);
        read_bit(0); // align fluctuation
        g_flipped  = g_answer[known_pos] != read_bit(known_pos);
        g_reversed= false;
    } else {
        g_flipped = g_answer[g_eq_pair] != read_bit(g_eq_pair);
        g_reversed = g_flipped ^ (g_answer[g_neq_pair] != read_bit(g_neq_pair));
    }
}

void smart()
{
    g_answer.resize(g_size);
    g_eq_pair = g_neq_pair = -1;
    g_reversed = g_flipped = false;
    for (g_pos = 0; g_pos < g_size / 2; ++g_pos) {
        if (g_pos % g_check_cycle == 0) check_fluctuation();
        read_pair();
    }

    if (g_reversed)
        g_answer = reverse(g_answer);
    if (g_flipped)
        g_answer = flip(g_answer);

    char answer;
    std::cout << to_string(g_answer) << std::endl;
    std::cin >> answer;
    g_debug << "After " << g_question_asked << " questions, answer for " << to_string(g_answer) << " is " << answer << std::endl << std::endl;
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
        g_question_asked = 0;
        //solve();
        smart();
    }
}

/*
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 esab_atad.cpp -o esab_atad.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 esab_atad.cpp -o esab_atad.o
py.exe interactive_runner.py py.exe esab_atad_testing_tool.py 1 -- esab_atad.exe


Interactive tool: esab_atad_testing_tool.py


*/
