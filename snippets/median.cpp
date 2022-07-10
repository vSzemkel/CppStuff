    const auto median = [](const std::vector<int>& data) -> float {
        const auto sz = data.size();
        if (sz & 1)
            return data[sz / 2];
        else
            return float(data[sz / 2 - 1] + data[sz / 2]) / 2;
    };
