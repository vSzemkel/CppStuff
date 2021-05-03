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
