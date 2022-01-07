
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

static auto modulo_inverse_list(const int last, const int M)
{
    assert(std::gcd(last, M) == 1); // coprimes
    std::vector<int> ret(last + 1);
    ret[1] = 1;
    for (int i = 2; i <= last; ++i)
        ret[i] = M - M / i * ret[M % i] % M;
    return ret;
}

static auto modulo_range_length(const int first, const int last, const int M) { // [first..last]
    auto len = last - first + 1;
    if (len <= 0)
        len = (M + 1 - first + last) % M;
    return len;
}
