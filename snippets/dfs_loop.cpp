// see: meta/2024/least_common_ancestor.cpp

stats_t dfs(int node_idx) { // stack overflow on Windows
    auto& node = tree[node_idx];
    // root path increase related stuff here

    stats_t ret;
    for (const int c : node._childs) {
        auto child_descendants = dfs(c);
        if (ret.size() < child_descendants.size())
            std::swap(ret, child_descendants);
        // merge state of current node's child into ret
    }

    // root path decrease related stuff here
    // current root path correct - use it here
    // descendants state in ret - use it here
    // update ret with current node;

    return ret;
}

void dfs_loop(int node_idx) {
    struct frame_t {
        int node_id;
        size_t next_child;
        stats_t stats;
    };
    std::vector<frame_t> stack(1, {node_idx, 0, stats_t{}});
    stack.reserve(U);
    while (!stack.empty()) {
        auto& cur = stack.back();
        auto& node = tree[cur.node_id];

        if (cur.next_child == 0)
            ; // root path increase related stuff here
        if (cur.next_child < node._childs.size()) {
            stack.push_back({node._childs[cur.next_child++], 0, {}});
            continue;
        }

        // root path decrease related stuff here
        // current root path correct - use it here
        // descendants state in cur.stats - use it here

        if (0 <= node._parent) {
            auto& parent_stats = stack[stack.size() - 2].stats;
            if (parent_stats.size() < cur.stats.size())
                std::swap(parent_stats, cur.stats);
            // merge state of current node's children into parent node
            // merge state of current node into parent node
        }
        stack.pop_back();
    }
}
