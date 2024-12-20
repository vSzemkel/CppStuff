
#include <iostream>
#include <filesystem>
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
        std::cout << "Usage: " << argv[0] << " <task_name>\n";
        return 1;
    }

    std::ifstream src{g_seedname};
    if (!src.good()) {
        std::cerr << "Template file: " << g_seedname << " not found.\n";
        return 2;
    }

    std::string replace_with{argv[1]};
    std::string filename = replace_with + ".cpp";
    if (std::filesystem::exists(filename)) {
        if (!init_input(filename))
            std::cout << "Specialization for " << replace_with << " already exists.\n";
        else
            std::cout << "Input data for " << replace_with << " restored.\n";
        return 3;
    }

    std::string line;
    std::ofstream dst{filename};
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