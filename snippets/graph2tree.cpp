class T {
    int id;
    int peer_id;
};

static std::vector<T> graph2tree(const std::vector<std::vector<T>>& roads, const int root) {
    const int size = (int)roads.size();
    assert(0 <= root && root < size);
    std::vector<bool> visited(size);
    std::vector<id_t> stack, parents(size);
    parents[root] = stack.emplace_back(T{root, -1});
    visited[root] = true;
    while (!stack.empty()) {
        const auto curr = stack.back();
        stack.pop_back();
        for (auto child : roads[curr.id]) {
            if (child.peer_id != curr.id)
                std::swap(child.id, child.peer_id);
            if (visited[child.id]) continue;
            visited[child.id] = true;
            parents[child.id] = child;
            stack.push_back(child);
        }
    }

    return parents;
}
