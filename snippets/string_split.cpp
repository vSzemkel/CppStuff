#include <algorithm>
#include <string>
#include <vector>

constexpr std::vector<std::string> split(std::string_view strv, std::string_view delims = " ") {
    std::vector<std::string> ret;
    int pos{0}, sz = int(strv.size());

    while (pos < sz) {
        const auto end = strv.find_first_of(delims, pos);

        if (pos != end)
            ret.emplace_back(strv.substr(pos, end - pos));

        if (end == std::string_view::npos)
            break;

        pos = end + 1;
    }

    return ret;
}