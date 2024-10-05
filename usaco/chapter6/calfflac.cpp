/*
ID: marcin.8
LANG: C++
TASK: calfflac
PROBLEM STATEMENT: https://usaco.training/usacoprob2?a=BZ0afcyNAFw&S=calfflac
*/

#include <algorithm>
#include <fstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

std::ifstream task_in("calfflac.in");
std::ofstream task_out("calfflac.out");

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
        _expanded.assign(_exp_size, '#');
        _expanded.front() = '$';
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

int main(int, char**)
{
    std::string chunk, text;
    while (task_in.good()) {
        std::getline(task_in, chunk);
        text += chunk;
        text += '\n';
    }
    text.pop_back();

    int pos{};
    std::vector<int> fillers;
    std::string clean_text;
    for (const char c : text) {
        if (::isalpha(c))
            clean_text.push_back(::tolower(c));
        else
            fillers.push_back(pos);
        ++pos;
    }

    manacher_t m{clean_text};
    auto [start, size] = m.longest();

    int expanded_size{size};
    bool update_start{true};
    for (const int off : fillers)
        if (update_start) {
            if (off <= start)
                ++start;
            else {
                update_start = false;
                goto here;
            }
        } else {
here:
            if (off < start + expanded_size)
                ++expanded_size;
            else
                break;
        }

    task_out << size << '\n' << std::string_view(text.data() + start, expanded_size) << '\n';
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 calfflac.cpp -o calfflac.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 -fsanitize=address calfflac.cpp -o calfflac

Run:
calfflac.exe && type calfflac.out

Input:

Confucius say: Madam, I'm Adam.

Output:

11
Madam, I'm Adam

*/
