
#include <iostream>
#include <vector>

// Elementary tree operations for predefined size trees

constexpr int N = 10;
int g_segtree_index{0};
std::vector<std::vector<int>> g_tree(N);
std::vector<int> g_size(N), g_paths(N), g_subtree_paths(N), g_parent(N), g_depth(N), g_heavy(N, -1), g_head(N), g_segtree_pos(N);

void init()
{
    g_tree[0].push_back(1);
    g_tree[0].push_back(2);
    g_tree[1].push_back(3);
    g_tree[3].push_back(6);
    g_tree[3].push_back(7);
    g_tree[7].push_back(9);
    g_tree[2].push_back(4);
    g_tree[2].push_back(5);
    g_tree[5].push_back(8);

    g_parent[0] = -1;
}

int dfs(const int node)
{
    int size{1}, max_size{0};
    for (const int child : g_tree[node]) {
        g_parent[child] = node;
        g_depth[child] = g_depth[node] + 1;
        size += dfs(child);
        g_paths[node] += g_paths[child] + g_size[child];
        g_subtree_paths[node] += g_subtree_paths[child];
        if (max_size < g_size[child]) {
            max_size = g_size[child];
            g_heavy[node] = child;
        }
    }
    g_subtree_paths[node] += g_paths[node];
    g_size[node] = size;
    return size;
}

void decompose(const int node, const int head)
{
    g_head[node] = head;
    g_segtree_pos[node] = g_segtree_index++;

    const int heavy = g_heavy[node];
    if (heavy != -1)
        decompose(heavy, head);
    for (const int child : g_tree[node])
        if (child != heavy)
            decompose(child, child);
}

int count_paths_to(const int node) 
{
    const int parent = g_parent[node];
    if (parent == -1)
        return g_paths[0];
    return count_paths_to(parent) + g_size[0] - 2 * g_size[node];
}

int find_lca(int node1, int node2)
{
    while (g_head[node1] != g_head[node2]) {
        if (g_depth[g_head[node1]] > g_depth[g_head[node2]])
            std::swap(node1, node2);
        node2 = g_parent[g_head[node2]];
    }

    return g_depth[node1] < g_depth[node2] ? node1 : node2;
}

int distance(const int node1, const int node2)
{
    const int lca = find_lca(node1, node2);
    return g_depth[node1] + g_depth[node2] - 2 * g_depth[lca];
}

std::vector<int> to_segtree()
{
    std::vector<int> ret(N);
    for (int i = 0; i < N; ++i)
        ret[g_segtree_pos[i]] = i;
    return ret;
}

int segment_tree_query(const int node1, const int node2)
{
    return 0; // TODO: implement really interesting segment tree, see ./segment_tree.cpp
}

int query(int node1, int node2)
{
    int ret{0};
    while (g_head[node1] != g_head[node2]) {
        if (g_depth[g_head[node1]] > g_depth[g_head[node2]])
            std::swap(node1, node2);
        const int ca = segment_tree_query(g_segtree_pos[g_head[node2]], g_segtree_pos[node2]);
        ret = std::max(ret, ca);
        node2 = g_parent[g_head[node2]];
    }

    if (g_depth[node1] > g_depth[node2])
        std::swap(node1, node2);
    return std::max(ret, segment_tree_query(g_segtree_pos[node1], g_segtree_pos[node2]));
}

int main(int argc, char* argv[])
{
    init();
    dfs(0);
    decompose(0, 0);
    to_segtree();

    std::cout << "Nodes count: " << g_size[0] << '\n';
    std::cout << "Root paths sum: " << g_paths[0] << '\n';
    std::cout << "Subtrees paths sum: " << g_subtree_paths[0] << '\n';
    std::cout << "Sum of paths to 3: " << count_paths_to(2) << '\n';
    std::cout << "Lowest common ancestor of 6 and 9: " << find_lca(6, 9) << '\n';
    std::cout << "Distance between 5 and 7: " << distance(5, 7) << '\n';
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 tree_no_ptr.cpp -o tree_no_ptr.exe

Output:

Nodes count: 10
Root paths sum: 21
Subtrees paths sum: 39
Sum of paths to 3: 23
Lowest common ancestor of 6 and 9: 3
Distance between 5 and 7: 5

*/
