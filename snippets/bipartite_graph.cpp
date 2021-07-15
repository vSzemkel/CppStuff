
using bipartite_t = std::vector<std::vector<bool>>;

    int M{0}, G{0}, flips{0};
    std::vector<std::vector<int>> order(R, std::vector<int>(C));
    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            if (check()) 
                order[r][c] = M++;
            else
                order[r][c] = G++;
    bipartite_t bg(M, std::vector<bool>(G)); // M -> G

    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c) {
            if (check1()) {
                if (check2())
                    bg[order[r][c]][order[r][c + 1]] = true;
                else
                    bg[order[r][c + 1]][order[r][c]] = true;
            }



