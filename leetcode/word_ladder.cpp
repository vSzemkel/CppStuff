
#include <algorithm>
#include <cassert>
#include <deque>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// Word Ladder
// https://leetcode.com/problems/word-ladder/description


int ladderLength(const std::string& beginWord, const std::string& endWord, std::vector<std::string>& wordList) { // O(min(M*N2, 26*M*N))
    const int ALFABETH_SIZE = 26;
    int startNode, endNode;
    const auto foundEnd = std::find(wordList.begin(), wordList.end(), endWord);
    if (foundEnd == wordList.end()) {
        return 0; // The endWord is not in wordList, so can't be reached
    } else
        endNode = int(std::distance(wordList.begin(), foundEnd));
    const auto foundBegin = std::find(wordList.begin(), wordList.end(), beginWord);
    if (foundBegin == wordList.end()) {
        startNode = int(wordList.size());
        wordList.push_back(beginWord);
    } else
        startNode = int(std::distance(wordList.begin(), foundBegin));

    std::vector<int> neighbors(20);
    const auto N = int(wordList.size());
    const auto M = int(wordList.back().size());
    using strategy_t = std::function<std::vector<int>(int)>;
    std::unordered_map<std::string, int> index; // { word -> nodeIndex }
    const strategy_t few_words_strategy = [&](int nodeIndex) {
        const auto is_similar = [M](const std::string& a, const std::string& b) -> bool {
            assert(a.size() == b.size());
            int differences{};
            for (int i = 0; i < M; ++i) {
                if (a[i] != b[i])
                    ++differences;
                if (differences > 1)
                    return false;
            }

            return differences == 1;
        };

        neighbors.clear();
        for (int t = 0; t < N; ++t)
            if (is_similar(wordList[nodeIndex], wordList[t]))
                neighbors.push_back(t);

        return neighbors;
    };
    const strategy_t many_words_strategy = [&](int nodeIndex) {
        neighbors.clear();
        auto& word = wordList[nodeIndex];
        for (int i = 0; i < M; ++i) {
            const auto letter = word[i];
            for (char c = 'a'; c <= 'z'; ++c)
                if (c != letter) {
                    word[i] = c;
                    const auto found = index.find(word);
                    if (found != index.end())
                        neighbors.push_back(found->second);
                    word[i] = letter;
                }
        }

        return neighbors;
    };

    auto strategy = few_words_strategy;
    if (N > ALFABETH_SIZE) { // strategy pattern
        strategy = many_words_strategy;
        for (int f = 0; f < N; ++f)
            index[wordList[f]] = f;
    }

    std::vector<bool> seen(N);
    std::deque<std::pair<int, int>> que; // {node, length}
    que.emplace_back(startNode, 0);
    seen[startNode] = true;
    while (!que.empty()) {
        const auto& [node, length] = que.front();
        if (node == endNode)
            return length + 1;
        for (const auto next : strategy(node)) // here we could neigbours
            if (!seen[next]) {
                seen[next] = true;
                que.emplace_back(next, length + 1);
            }
        que.pop_front();
    }

    return 0;
}

static void io_handler() {
    int N;
    std::string beginWord, endWord;
    std::cin >> beginWord >> endWord >> N;
    std::vector<std::string> wordList(N);
    for (auto& w : wordList)
        std::cin >> w;
    std::cout << ladderLength(beginWord, endWord, wordList);
}

int main(int, char**)
{
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": "; io_handler(); std::cout << '\n';
    }
}
/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 word_ladder.cpp -o word_ladder.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address word_ladder.cpp -o word_ladder

Run:
word_ladder.exe < word_ladder.in

Input:

1
hit cog 6
hot dot dog lot log cog

Output:

5

*/
