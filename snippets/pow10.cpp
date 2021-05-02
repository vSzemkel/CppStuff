std::vector<int64_t> pow10(19);

    int64_t p{1};
    for (auto& pow : pow10) {
        pow = p;
        p *= 10;
    }

