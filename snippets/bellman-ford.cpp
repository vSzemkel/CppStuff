    // see: /kickstart/2021/star_trappers.cpp

    for (int i = 0; i < letters ; ++i)
        costs[i][i] = 0;

    for (int k = 0; k < letters; ++k)
        for (int i = 0; i < letters - 1; ++i)
            for (int j = i + 1; j < letters; ++j)
                costs[i][j] = costs[j][i] = std::min(costs[i][j], costs[i][k] + costs[k][j]);
