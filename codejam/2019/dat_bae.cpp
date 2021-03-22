
#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

// Dat Bae
// https://codingcompetitions.withgoogle.com/codejam/round/0000000000051705/00000000000881de

std::ofstream g_debug("dat_bae.log");

struct section_t {
    int start;
    int length;
    int broken;
    int before_split_len;
    char start_symbol;
    bool analyzed() const { return 0 == broken || broken == length; }
};

static void print_sections(const std::vector<section_t>& sec) {
    for (const section_t& s : sec)
        g_debug << "[" << s.start << "," << s.length << "," << s.broken << "," << s.start_symbol << "] ";
    g_debug << '\n';
}

static void solve() {
    int Q{0}, N, B, F; std::cin >> N >> B >> F;
    g_debug << N << " " << B << " " << F << std::endl;
    constexpr auto switch_marker = [](const char m){ return (m == '0') ? '1' : '0'; };

    std::vector<int> broken;
    std::vector<section_t> sections;
    std::string answer, query(N, ' ');

    // initialize sections
    {
        char marker{'0'};
        const int chunk = std::min(B + 1, N / 2);
        const int overflow = (chunk - (N % chunk)) % chunk;
        g_debug << "chunk: " << chunk << " overflov: " << overflow << '\n';
        for (int i = 0; i < N; ) {
            const int bound = std::min(i + chunk, N);
            for (int j = i; j < bound; ++j, ++i)
                query[j] = marker;
            marker = switch_marker(marker);
        }
        std::cout << query << std::endl; ++Q;
        std::cin >> answer;
        g_debug << "INIT: " << query << " -> " << answer << std::endl;
        marker = '0';
        section_t* prev = nullptr;
        bool back_operational{false};
        const int ans_size = (int)answer.size();
        for (int pos = 0; pos < ans_size; ) {
            section_t sec{0, chunk, chunk, 0, marker};
            if (prev)
                sec.start = prev->start + prev->length;
            const int start{pos};
            while (pos < ans_size && pos - start < chunk && answer[pos] == marker) {
                ++pos;
                --sec.broken;
            }
            if (back_operational && (sec.broken == 0 || (pos == ans_size && sec.broken == chunk))) {
                prev->length += sec.length;
                prev->broken += sec.broken; // for the last broken trimmed
            } else {
                sections.push_back(sec);
                prev = &sections.back();
            }
            marker = switch_marker(marker);
            back_operational = sec.broken == 0;
        }
        // trim the last one if needed
        if (overflow != 0) {
            prev->length -= overflow;
            prev->broken -= overflow;
        }
        // identify broken sections
        for (const auto& s : sections)
            if (s.broken == s.length)
                for (int i = s.start; i < s.start + s.length; ++i)
                    broken.push_back(i);
        print_sections(sections);
    }

    while ((int)broken.size() < B) {
        char marker{'1'};
        int pos{0}, ind{0};
        // prepare query
        std::vector<std::pair<int, section_t>> new_sections;
        for (auto& s : sections) {
            int head, tail;
            marker = switch_marker(marker);
            if (s.analyzed()) {
                head = s.length;
                tail = 0;
            } else { // split: head <= tail
                head = s.length / 2;
                if (head == 0) ++head;
                tail = s.length - head;
                s.before_split_len = s.length;
            }

            s.start_symbol = marker;
            while (pos < N && head-- > 0) query[pos++] = marker;
            if (tail > 0) {
                s.broken *= -1;
                s.length -= tail;
                section_t tail_sect;
                tail_sect.broken = -1;
                tail_sect.length = tail;
                tail_sect.before_split_len = -1;
                tail_sect.start = s.start + s.length;
                marker = switch_marker(marker);
                tail_sect.start_symbol = marker;
                while (pos < N && tail-- > 0) query[pos++] = marker;
                new_sections.emplace_back(ind + 1, tail_sect);
            }

            ++ind;
        }

        for (auto it = new_sections.rbegin(); it != new_sections.rend(); ++it)
            sections.insert(sections.begin() + it->first, it->second);

        // ask question
        print_sections(sections);
        std::cout << query << std::endl; ++Q;
        std::cin >> answer;
        g_debug << "QA: " << query << " -> " << answer << std::endl;
        if (answer == "-1") exit(0);
        // analyze the answer
        pos = 0;
        int continuation{0}; // =0 first chunk, <0 number of broken, =1 fully functional
        for (auto& s : sections) {
            g_debug << "2nd loop [" << s.start << "," << s.length << "," << s.broken << "," << s.start_symbol << "] " << '\n';
            if (s.broken == 0)
                pos += s.length;
            if (s.analyzed()) continue;

            bool fully_broken{false};
            if (continuation != 0) {
                if (continuation == 1)
                    s.broken = 0;
                else {
                    s.broken = -continuation;
                    fully_broken = s.broken == s.length;
                }
                continuation = 0;
                pos += s.length - s.broken;
            } else {
                int start{pos};
                const int maxlen = std::min(s.length, s.before_split_len + s.broken);
                while (pos < start + maxlen && answer[pos] == s.start_symbol) ++pos;
                g_debug << "headscan: " << pos - start << " scanned of " << s.length << " from " << start << " starting symbol: " << s.start_symbol << " ending pos: " << pos << '\n';
                continuation = s.broken;
                s.broken = s.length - pos + start;
                continuation += s.broken;
                fully_broken = pos == start;
                if (continuation == 0) // next fully functional
                    continuation = 1;
            }

            if (fully_broken)
                for (int i = s.start; i < s.start + s.length; ++i)
                    broken.push_back(i);
        }
    }

    std::sort(broken.begin(), broken.end());
    for (const int b : broken)
        std::cout << b << " ";
    std::cout << std::endl;
    std::cin >> answer;
    g_debug << "Answer == " << answer << " after asking " << Q << " queries" << std::endl;
    if (answer == "-1") exit(0);
}

void ecnerwala()
{
    using namespace std;
    string S[5];
    for (int i = 0; i < 1024; i++) {
        for (int z = 0; z < 5; z++) {
            S[z] += char('0' + ((i >> z) & 1));
        }
    }

    int N, B, F; cin >> N >> B >> F;
    assert(B <= 15);

    string Q[5];
    for (int q = 0; q < 5; q++) {
        cout << S[q].substr(0, N) << endl << flush;
        cin >> Q[q];
        if (Q[q] == "-1") exit(0);
        assert(int(Q[q].size()) == N - B);
        Q[q] += S[q].substr(N);
        assert(int(Q[q].size()) == 1024 - B);
    }

    vector<int> bads;
    int nxt = 0;
    for (int i = 0; i < 1024 - B; i++) {
        int v = 0;
        for (int q = 0; q < 5; q++) {
            v |= int(Q[q][i] - '0') << q;
        }
        while ((nxt & 31) != v) {
            bads.push_back(nxt);
            nxt++;
        }
        nxt++;
    }

    while (nxt != 1024) {
        bads.push_back(nxt);
        nxt++;
    }

    assert(int(bads.size()) == B);
    for (int i = 0; i < B; i++) {
        cout << bads[i] << " \n"[i+1==B];
    }

    int verdict; std::cin >> verdict;
    if (verdict == -1) exit(0);
}

int main(int, char**)
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        solve();
    }
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++17 dat_bae.cpp -o dat_bae.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 dat_bae.cpp -o dat_bae.o

Run:
py.exe interactive_runner.py py.exe dat_bae_testing_tool.py 0 -- dat_bae.exe

*/
