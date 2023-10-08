
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

// Binary Tree Maximum Path Sum
// https://leetcode.com/problems/binary-tree-maximum-path-sum

// Data structure with raw pointer is a part of this task specification
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

int maxPathSum(TreeNode* root) {
    int ans = -1e09;
    std::function<int(TreeNode*)> visit = [&](TreeNode* node) {
        if (!node)
            return 0;

        const int ret = node->val;
        const int left = visit(node->left);
        const int right = visit(node->right);

        ans = std::max(ans, ret + left + right);

        return std::max(0, ret + std::max(left, right));
    };

    visit(root);
    return ans;
}

static void io_handler() {
    int N;
    std::cin >> N;
    std::vector<int> weights(N);
    std::vector<TreeNode*> nodes(N);
    for (auto& w : weights)
        std::cin >> w;
    while (--N) {
        int p, c;
        std::cin >> p >> c;
        if (!nodes[p]) nodes[p] = new TreeNode(weights[p]);
        if (!nodes[c]) nodes[c] = new TreeNode(weights[c]);
        if (nodes[p]->left)
            nodes[p]->right = nodes[c];
        else
            nodes[p]->left = nodes[c];
    }

    std::cout << maxPathSum(nodes[0]);
    for (auto n : nodes)
        delete n;
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
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 binary_tree_maximum_path_sum.cpp -o binary_tree_maximum_path_sum.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address binary_tree_maximum_path_sum.cpp -o binary_tree_maximum_path_sum

Run:
binary_tree_maximum_path_sum.exe < binary_tree_maximum_path_sum.in

Input:


Output:


*/
