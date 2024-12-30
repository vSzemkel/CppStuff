
#include <iostream>
#include <filesystem>
#include <format>
#include <fstream>
#include <string>

constinit char g_seedname[] = "competition_template.cpp";
constinit std::string g_replace_pattern{R"($TASKNAME$)"};
const auto g_replace_pattern_size = g_replace_pattern.size();

static bool init_input(const std::string& filename)
{
    std::filesystem::path inputfile{filename};
    if (std::filesystem::exists(inputfile.replace_extension("in")))
        return false;

    std::string line;
    std::ifstream src{filename};
    std::ofstream testdata{inputfile};
    bool rewrite{false}, found{false};
    while (std::getline(src, line)) {
        if (line == "Input:")
            rewrite = true;
        else if (line == "Output:")
            break;
        else if (rewrite && !line.empty()) {
            testdata << std::move(line) << '\n';
            found = true;
        }
    }

    src.close();
    testdata << '\n';
    testdata.close();

    return found;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << std::format("Usage: {} <task_name>\n", argv[0]);
        return 1;
    }

    std::ifstream src{g_seedname};
    if (!src.good()) {
        std::cerr << std::format("Template file: {} not found.\n", g_seedname);
        return 2;
    }

    std::string replace_with{argv[1]};
    std::string filename = replace_with + ".cpp";
    if (std::filesystem::exists(filename)) {
        if (!init_input(filename))
            std::cout << std::format("Specialization for {} already exists.\n", replace_with);
        else
            std::cout << std::format("Input data for {} restored.\n", replace_with);
        return 3;
    }

    std::string line;
    std::ofstream dst{filename};
    if (!dst.good()) {
        std::cout << std::format("Task file name {} is invalid.\n", filename);
        return 4;
    }

    while (std::getline(src, line)) {
        size_t pos{0};
        while (true) {
            pos = line.find(g_replace_pattern, pos);
            if (pos == std::string::npos) break;
            line.replace(pos, g_replace_pattern_size, replace_with);
            pos += replace_with.size();
        }
        dst << std::move(line) << '\n';
    }
    dst.close();
    src.close();

    init_input(filename);
}

/*
clang++.exe -Wall -ggdb3 -O3 -std=c++20 competition_init.cpp -o competition_init.exe

competition_init task_name

*/