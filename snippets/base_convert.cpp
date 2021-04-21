
constexpr char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

static std::string base_convert(int n, const int base)
{
    assert(1 < base && base < sizeof(digits) - 1);
    std::string ret;
    while (n > 0) {
        ret.push_back(digits[n % base]);
        n /= base;
    }

    std::reverse(ret.begin(), ret.end());
    return ret;
}