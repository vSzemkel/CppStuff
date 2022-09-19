// see: /kickstart/2022/scheduling_a_meeting.cpp

    std::vector<int> meetings(N, 0); // number of meetings a manager has in current window
    std::vector<std::vector<int>> increase_events(D + 1), decrease_events(D + 1);
    for (int z = M; z; --z) {
        int m, b, e;
        std::cin >> m >> b >> e; // [b, e)
        --m;
        if (b < X)
            ++meetings[m];
        else
            increase_events[b - X + 1].push_back(m);

        decrease_events[e].push_back(m);
    }

    for (int i = 0; i <= D - X; ++i) {
        for (const int j : increase_events[i])
            ++meetings[j];
        for (const int j : decrease_events[i])
            --meetings[j];

        // DO STUFF
    }
