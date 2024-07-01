
#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

// Warnings Counter - identifies unique warnings from build log file
// Reports in order from less frequent ones

struct position_t
{
    std::string path;
    int line;

    auto operator<=>(const position_t&) const = default;
};

template <>
struct std::hash<position_t>
{
    std::size_t operator()(const position_t& f) const noexcept
    {
        const auto h1 = std::hash<std::string>{}(f.path);
        const auto h2 = std::hash<int>{}(f.line);
        return h1 ^ (h2 << 1);
    }
};

std::unordered_map<std::string, std::set<position_t>> warnings; // {warning -> {unique occurences}}

static const std::regex build_server_pattern{R"(##\[warning\](.+)\.([^\.]+)\((\d+),\d+\): Warning (C[^:]+):)"};
static const std::regex local_pattern{R"((.+)\.([^\.]+)\((\d+)\): warning (C[^:]+):)"};

static void parse_log_line(const std::string& logLine)
{
    std::smatch matches;
    if (std::regex_search(logLine, matches, local_pattern)) {
        std::string filenameWithPath = matches[1].str();
        std::string extension = matches[2].str();
        std::string line = matches[3].str();
        std::string warningType = matches[4].str();

        const auto fullPath = std::format("{}.{}", filenameWithPath, extension);
        const auto position = position_t{fullPath, std::atoi(line.c_str())};
        warnings[warningType].insert(std::move(position));
    }
}

static void report()
{
    // sort warnings from less frequent
    std::vector<std::pair<size_t, std::string>> priorities;
    priorities.reserve(warnings.size());
    std::transform(warnings.begin(), warnings.end(), std::back_inserter(priorities), [](const auto& w){
        return std::pair{w.second.size(), w.first};
    });
    std::ranges::sort(priorities);

    int total{};
    for (const auto& [occurences, tag] : priorities) {
        std::cout << std::format("{} ({}):\n", tag, occurences);
        for (const auto& o : warnings[tag])
            std::cout << std::format("\t{} ({})\n", o.path, o.line);
        total += occurences;
    }

    std::cout << std::format("\nTotal warnings: {}\n", total);
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <build log file>\n";
        return 1;
    }

    std::ifstream src{argv[1]};
    if (!src.good()) {
        std::cout << "Can't open the log file: " << argv[1] << '\n';
        return 1;
    }

    std::string line;
    while (std::getline(src, line))
        parse_log_line(line);

    src.close();

    report();
}

/*

Format:
clang-format -i warnings_count.cpp

Compile:
cls && clang++.exe -Wall -Wextra -g -O0 -std=c++20 warnings_count.cpp -o warnings_count.exe
g++ -Wall -Wextra -g3 -Og -std=c++20 -fsanitize=address warnings_count.cpp -o warnings_count

*/
