#include <cassert>
#include <charconv>
#include <iostream>
#include <filesystem>
#include <format>
#include <fstream>
#include <string>
#include <string_view>
#include <unordered_set>

static constexpr int EMPTY = -1;
using set_t = std::unordered_set<int>;

bool case_insensitive_equal(std::string_view left, std::string_view right)
{
    if (left.size() != right.size())
        return false;

    for (std::size_t index = 0; index < left.size(); ++index) {
        const auto lhs = static_cast<unsigned char>(left[index]);
        const auto rhs = static_cast<unsigned char>(right[index]);

        if (std::tolower(lhs) != std::tolower(rhs))
            return false;
    }

    return true;
}

std::string generat_next_impl(const set_t& existingIds, const std::string& name, int counter, bool appendToFirst)
{
    if (!appendToFirst) {
        if (!existingIds.contains(EMPTY))
            return name;

        ++counter;
    }

    while (existingIds.contains(counter))
        ++counter;

    return std::format("{}{}", name, counter);
}

std::string generate_next_file_name(
    const std::filesystem::path& destPath,
    const std::string& filePrefix,
    int counter = 0,
    bool appendToFirst = false)
{
    const auto prefixLength = filePrefix.size();

    set_t existingIds;
    for (const auto& entry : std::filesystem::directory_iterator(destPath)) {
        if (!entry.is_regular_file())
            continue;

        auto existingName = entry.path().filename().string();
        if (existingName.size() < prefixLength || !case_insensitive_equal(
                                                      std::string_view(existingName.data(), prefixLength), filePrefix))
            continue;

        const auto dotPosition = existingName.find('.', prefixLength);
        if (dotPosition != std::filesystem::path::string_type::npos)
            existingName.resize(dotPosition);

        int value = EMPTY;
        std::from_chars(existingName.data() + prefixLength, existingName.data() + existingName.size(), value);
        existingIds.insert(value);
    }

    return (destPath / generat_next_impl(existingIds, filePrefix, counter, appendToFirst)).string();
}

std::string generate_next_dir_name(
    const std::filesystem::path& destPath,
    const std::string& dirPrefix,
    int counter = 0,
    bool appendToFirst = false)
{
    const auto prefixLength = dirPrefix.size();

    set_t existingIds;
    for (const auto& entry : std::filesystem::directory_iterator(destPath))
    {
        if (entry.is_directory())
        {
            auto existingName = entry.path().filename().string();
            if (existingName.size() >= prefixLength && case_insensitive_equal(
                std::string_view(existingName.data(), prefixLength), dirPrefix))
            {
                int value = EMPTY;
                std::from_chars(existingName.data() + prefixLength, existingName.data() + existingName.size(), value);
                existingIds.insert(value);
            }
        }
    }

    return (destPath / generat_next_impl(existingIds, dirPrefix, counter, appendToFirst)).string();
}

int main(int, char**)
{
    const std::string test_dir_name = "generate_next_file_name";
    const auto tmp_dir = std::filesystem::temp_directory_path() / test_dir_name;
    std::filesystem::create_directory(tmp_dir);
    std::ofstream(tmp_dir / "file.zip.001");
    std::ofstream(tmp_dir / "file6.pdf");

    const auto nextfile = generate_next_file_name(tmp_dir, "file", 5);
    assert(nextfile == tmp_dir / "file7");

    const auto nextdir = generate_next_dir_name(std::filesystem::temp_directory_path(), test_dir_name);
    assert(nextdir == std::filesystem::temp_directory_path() / (test_dir_name + "1"));

    std::filesystem::remove_all(tmp_dir);
    std::cout << "PASSED\n";
}

/*

Format:
clang-format -i generate_next_file_name.cpp

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++20 generate_next_file_name.cpp

*/