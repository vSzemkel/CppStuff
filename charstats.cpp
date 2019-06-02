
#include <filesystem>
#include <iostream>
#include <fstream>
#include <locale>
#include <string>
#include <array>
#include <map>

using namespace std;

struct diacritic {
    string character;
    uint occurences;
    uint8_t bytes[3];
    uint8_t width;
};

array<diacritic,1> diacritics = {
    {"ą", 0, {0xc4, 0x85, 0}, 2}
};

uint8_t g_diacritic_index = 0;
diacritic* g_current_diacritic = nullptr;

int main(int argc, char* argv[]) {
    filesystem::path file_path;
    if (argc > 1)
        file_path = argv[1];
    else {
        file_path = argv[0];
        file_path.concat(".cpp");
    }

    if (!filesystem::is_regular_file(file_path)) {
        cout << file_path << " is not a regular file\n";
        return -1;
    }

    map<const char, uint> stats;
    cout << "Character statistics for: " << file_path << "\n";
    ifstream file;
    file.open(file_path);
    do {
        const uint8_t c = file.get();
        if (g_current_diacritic != nullptr && c == g_current_diacritic->bytes[g_diacritic_index]) {
            if (g_diacritic_index == g_current_diacritic->width - 1) {
                g_current_diacritic->occurences++;
                g_current_diacritic =  nullptr;
            } else
                g_diacritic_index++;

            continue;
        }

        for (auto& d : diacritics)
            if (d.bytes[0] == c) {
                g_current_diacritic = &d;
                g_diacritic_index = 1;
            }

        if (g_current_diacritic != nullptr)
            continue;

        const auto result = stats.emplace(c, 1);
        if (!result.second) {
            auto& stat = *result.first;
            ++stat.second;
        }
    } while (file.good());

    for (const auto& s : stats)
        cout << "[" << setfill('0') << setw(3) << static_cast<int>(s.first) << "] " << s.first << ": " << s.second << "\n";
    cout << "Diacritics found:\n";
    for (const auto& d : diacritics)
        if (d.occurences > 0)
            cout << d.character << ": " << d.occurences << "\n";
}