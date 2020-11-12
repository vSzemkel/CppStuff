
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

constexpr char g_seedname[] = "competition_template.cpp";
const std::string g_replace_pattern{"$TASKNAME$"};
const int g_replace_size = g_replace_pattern.size();

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <task_name>\n";
        return 1;
    }

    std::ifstream src{g_seedname};
    if (!src.good()) {
        std::cout << "Template file: " << g_seedname << " not found\n";
        return 2;
    }

    std::string filename{argv[1]};
    filename += ".cpp";
    if (std::filesystem::exists(filename)) {
        std::cout << "Specializattion for: " << g_seedname << " already exists\n";
        return 3;
    }

    std::string line;
    std::ofstream dst{(filename).c_str(), std::ios::app};
    while (std::getline(src, line)) {
        size_t pos{0};
        while (true) {
            pos = line.find(g_replace_pattern, pos);
            if (pos == std::string::npos) break;
            line.replace(pos, g_replace_size, filename);
            pos += g_replace_size;
        }
        dst << line << "\n";
    }
    dst.close();
    src.close();

    std::ofstream testdata{filename.replace(filename.size() - 3, filename.size(), "in")};
    testdata << "\n";
    testdata.close();
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 competition_init.cpp -o competition_init.exe

Input:


Output:


*/