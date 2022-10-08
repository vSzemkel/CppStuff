// Minimal Bounding Rectangle, from r-tree algorithm
    float* writeTo = 
    for (int i = 0; i < featureCount; i += K) {
        auto leafIndex = &leafArray[i];
        auto readFrom = &leafMbrArray[*leafIndex++ * 4];
        std::memcpy(writeTo, readFrom, sizeof(float) * 4);
        // Zero's will occur when the leaf level was not evenly divisible by K
        for (auto z = std::min(K, featureCount - i) - 1; z > 0; --z) {
            auto w = writeTo;
            auto r = &leafMbrArray[*leafIndex++ * 4];

            *w++ = std::min(*w, *r++); // x-min
            *w++ = std::min(*w, *r++); // y-min
            *w++ = std::max(*w, *r++); // x-max
            *w   = std::max(*w, *r);   // y-max
        }

        writeTo += 4;
    }
