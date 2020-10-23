
#include <iostream>
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
    std::ofstream testdata{(filename + ".in").c_str()};
    testdata << "\n";
    testdata.close();

    std::string line;
    std::ofstream dst{(filename + ".cpp").c_str()};
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
}

/*
clang++.exe -Wall -ggdb3 -O0 -std=c++17 competition_init.cpp -o competition_init.exe

Input:


Output:


*/