
constexpr int64_t MAX = 999999999999999;

int64_t add(int64_t a, int64_t b)
{
    a += b;
    while (a >= MAX)
        a -= MAX;
    return a;
}

int64_t modmul(int64_t a, int64_t b)
{
    int64_t r{0};
    while (b) {
        if (b & 1) r = add(r, a);
        b >>= 1;
        a = add(a, a);
    }
    return r;
}
