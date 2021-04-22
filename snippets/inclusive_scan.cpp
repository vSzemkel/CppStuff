
std::partial_sum(data.begin(), data.end(), data.begin())

namespace fuck_google_cxx17 {
    template<class InputIt, class OutputIt>
    void inclusive_scan(InputIt first, InputIt last, OutputIt d_first) {
        auto acc = *first;
        *d_first++ = acc;
        for (auto it = first + 1; it != last; ++it) {
            acc += *it;
            *d_first++ = acc;
        }
    }
}