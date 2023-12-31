
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

// Merge k Sorted Lists
// https://leetcode.com/problems/merge-k-sorted-lists

struct ListNode
{
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

ListNode* mergeKLists(std::vector<ListNode*>& lists)
{
    std::priority_queue<std::pair<int, ListNode*>, std::vector<std::pair<int, ListNode*>>, std::greater<std::pair<int, ListNode*>>> pq;
    for (const auto list : lists)
        if (list)
            pq.emplace(list->val, list);

    if (pq.empty())
        return nullptr;

    ListNode *ans, *end;
    ans = end = pq.top().second;
    if (end->next)
        pq.emplace(end->next->val, end->next);
    pq.pop();

    while (!pq.empty()) {
        auto list = pq.top().second;
        end->next = list;
        end = end->next;
        pq.pop();

        list = list->next;
        if (list)
            pq.emplace(list->val, list);
    };

    return ans;
}

static void io_handler()
{
    int N;
    std::cin >> N;
    std::vector<ListNode*> lists(N);
    for (auto& list : lists) {
        int len;
        std::cin >> len;
        ListNode* end = list = new ListNode();
        for (--len; len; --len) {
            std::cin >> end->val;
            end->next = new ListNode();
            end = end->next;
        }
        std::cin >> end->val;
    }

    for (auto merged = mergeKLists(lists); merged; merged = merged->next)
        std::cout << merged->val << ' ';
}

int main(int, char**)
{
    int no_of_cases;
    std::cin >> no_of_cases;
    for (int g = 1; g <= no_of_cases; ++g) {
        std::cout << "Case #" << g << ": ";
        io_handler();
        std::cout << '\n';
    }
}
/*

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 merge_k_sorted_lists.cpp -o merge_k_sorted_lists.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address merge_k_sorted_lists.cpp -o n_queens-ii

Run:
merge_k_sorted_lists.exe < merge_k_sorted_lists.in

Input:

1
10
0 1 2 3 4 5 6 7 8 9
5
5 10 15 20 25
3
100 101 111

Output:

Case #1: 0 1 2 3 4 5 5 6 7 8 9 10 15 20 25 100 101 111 

*/
