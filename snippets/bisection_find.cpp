// use std::lower_bound when range is allocated

int64_t lb{0}, ub{};
while (lb != ub) {
    const auto mid = (lb + ub) / 2;
    if (check(mid))
        ub = mid;
    else
        lb = mid + 1;
}
check(lb);

// by Benq
template <typename T, typename U>
T firstTrue(T lo, T hi, U f) {
    hi++;
    assert(lo <= hi); // assuming f is increasing
    while (lo < hi) { // find first index such that f is true
        T mid = lo + (hi - lo) / 2;
        f(mid) ? hi = mid : lo = mid + 1;
    }
    return lo;
}

template <typename T, typename U>
T lastTrue(T lo, T hi, U f) {
    lo--;
    assert(lo <= hi); // assuming f is decreasing
    while (lo < hi) { // find last index such that f is true
        T mid = lo + (hi - lo + 1) / 2;
        f(mid) ? lo = mid : hi = mid - 1;
    }
    return lo;
}
