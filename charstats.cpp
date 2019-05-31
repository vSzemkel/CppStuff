
#include <filesystem>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

int main(int argc, char* argv[]) {
    filesystem::path file_path;
    if (argc > 1)
        file_path = argv[1];
    else {
        file_path = argv[0];
        file_path.concat(".cpp");
    }

    if (!filesystem::exists(file_path) || !filesystem::is_regular_file(file_path)) {
        cout << file_path << " is not a regular file\n";
        return -1;
    }

    map<char, uint> stats;
    cout << "Character statistics for: " << file_path << "\n";
    ifstream file;
    file.open(file_path);
    do {
        const char c = file.get();
        const auto result = stats.emplace(c, 1);
        if (!result.second) {
            auto& stat = *result.first;
            ++(stat.second);
        }
    } while (file.good());

    for (const auto& s : stats)
        cout << "[" << setfill('0') << setw(3) << static_cast<int>(s.first) << "] " << s.first << ": " << s.second << "\n";
}