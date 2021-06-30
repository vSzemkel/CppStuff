// see: ..\kickstart\2020\locked_doors.cpp

std::vector<std::vector<int>> ancestors;

    // initialize ancestors
    int lg2size{0}, counter{N};
    const int root = ...;
    while (counter > 0) { counter >>= 1; ++lg2size; }
    ancestors.assign(N, std::vector<int>(lg2size, root));
    // fill parents
    for (int i = 0; i < N; ++i)
        p[i][0] = ...// parent of node i
    // fill ancestors
    for (int j = 1; j < lg2size; ++j)
        for (int i = 0; i < N - 1; ++i)
            ancestors[i][j] = ancestors[ancestors[i][j - 1]][j - 1];

// USAGE:

                int query_root = leaf_to_check, len = lg2size - 1;
                while (true) {
                    const int candidate = ancestors[query_root][len];
                    if (check(candidate))
                        query_root = candidate;
                    if (len == 0)
                        break;
                    --len;
                }
                query_root = ancestors[query_root][0];

// source: https://www.geeksforgeeks.org/lca-in-a-tree-using-binary-lifting-technique/
// source: https://cp-algorithms.com/graph/lca_binary_lifting.html
