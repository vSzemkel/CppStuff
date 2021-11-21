// see: ..\kickstart\2020\locked_doors.cpp
// walking up to node, see: ..\kickstart\2021\dependent_events.cpp

constexpr const int DEPTH = desctree_t::max_depth(200000);
    std::vector<std::array<int, DEPTH>> anc(N);
    anc[0].fill(-1); // root has no ancestors
    for (int i = 1; i < N; ++i) {
        anc[i][0] = ..;
        for (int k = 1; k < DEPTH; ++k) {
            anc[i][k] = ~anc[i][k - 1] ? anc[anc[i][k - 1]][k - 1] : -1;
            if (~anc[i][k]) {
                // do stuff here for init tree node's additional values

        for (int k = DEPTH - 1; k >= 0; --k) { // traverse from node p up to node la with 2**k-jumps
            while (~anc[p][k] && anc[p][k] >= la && p != la) // in-tree && parent-not-above-target && below-target
                do_stuff(p, k);

////////////////////////////////////////////////////////////////////

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
