
#include <iostream>
#include <vector>

// Reverse Nodes in K Groups
// https://leetcode.com/problems/reverse-nodes-in-k-group

// Data structure with raw pointer is a part of this task specification
struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

void printList(ListNode* head) {
    while (head) {
        std::cout << head->val << ' ';
        head = head->next;
    }
}

ListNode* reverseList(ListNode* head) {
    decltype(head) rev_head{};
    while (head) {
        const auto cur = head;
        head = cur->next;
        cur->next = rev_head;
        rev_head = cur;
    }

    return rev_head;
}

ListNode* reverseKGroup(ListNode* head, int k) {
    std::vector<ListNode*> stack(k);
    decltype(head) result{};
    while (true) {
        int found{};
        while (head && found < k) {
            stack[found] = head;
            head = head->next;
            ++found;
        }

        if (found == 0)
            break;
        if (found < k)
            for (int i = 0; i < found; ++i) {
                const auto cur = stack[i];
                cur->next = result;
                result = cur;
            }
        else
            for (int i = found - 1; ~i; --i) {
                const auto cur = stack[i];
                cur->next = result;
                result = cur;
            }
    }

    return reverseList(result);
}

static void io_handler() {
    int N, K;
    std::cin >> N >> K;
    ListNode* head{};
    for (int z = N; z; --z) {
        int v;
        std::cin >> v;
        head = new ListNode(v, head);
    }

    // reverse list to preserve left to right intuitive order
    const auto rev_head = reverseList(head);
    auto result = reverseKGroup(rev_head, K);
    printList(result);

    while (result) {
        auto to_del = result;
        result = result->next;
        delete to_del;
    }
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
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 reverse_nodes_in_kgroups.cpp -o reverse_nodes_in_kgroups.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address reverse_nodes_in_kgroups.cpp -o reverse_nodes_in_kgroups

Run:
reverse_nodes_in_kgroups.exe < reverse_nodes_in_kgroups.in

Input:

3
1 1
12345678
5 2
1 2 3 4 5
13 5
1 2 3 4 5 6 7 8 9 10 11 12 13

Output:

Case #1: 12345678 
Case #2: 2 1 4 3 5 
Case #3: 5 4 3 2 1 10 9 8 7 6 11 12 13

*/
