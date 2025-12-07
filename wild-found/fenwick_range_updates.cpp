struct Fenwick { // A_Le_K
    //vector <vector<int>> t;
    int t[MX][MX];
    //int n, m;
    const int n=MX, m=MX;

    void reset() {
        FOR(i, MX) FOR(j, MX) t[i][j]=0;
    }

    Fenwick()
    {
        reset();
    }

    inline int sum(int x, int y)
    {
        int result = 0;
        for (int i = x; i >= 0; i = (i & (i+1)) - 1)
            for (int j = y; j >= 0; j = (j & (j+1)) - 1)
                result += t[i][j];
        return result;
    }

    inline void inc (int x, int y, int delta)
    {
        for (int i = x; i < n; i = (i | (i+1)))
            for (int j = y; j < m; j = (j | (j+1)))
                t[i][j] += delta;
    }

};
