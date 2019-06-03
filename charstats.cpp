
#include <filesystem>
#include <iostream>
#include <fstream>
#include <locale>
#include <string>
#include <array>
#include <map>

using namespace std;

struct diacritic_t {
    string character;
    uint occurences;
};

array<diacritic_t,18> g_diacritics = {
    diacritic_t{"ą", 0},
    diacritic_t{"ć", 0},
    diacritic_t{"ę", 0},
    diacritic_t{"ł", 0},
    diacritic_t{"ń", 0},
    diacritic_t{"ó", 0},
    diacritic_t{"ś", 0},
    diacritic_t{"ź", 0},
    diacritic_t{"ż", 0},
    diacritic_t{"Ą", 0},
    diacritic_t{"Ć", 0},
    diacritic_t{"Ę", 0},
    diacritic_t{"Ł", 0},
    diacritic_t{"Ń", 0},
    diacritic_t{"Ó", 0},
    diacritic_t{"Ś", 0},
    diacritic_t{"Ź", 0},
    diacritic_t{"Ż", 0},
};

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
        char c3 = '\0', c2 = '\0', c = file.get();
        for (auto& d : g_diacritics)
            if (d.character.c_str()[0] == c) {
                if (c2 == '\0')
                    c2 = file.get();
                if (d.character.c_str()[1] != c2)
                    continue;
                if (d.character.size() == 2) {
                    d.occurences++;
                    c2 = '\0';
                    break;
                } else {
                    if (c3 != '\0')
                        c3 = file.get();
                    if (d.character.c_str()[2] != c3)
                        continue;
                    d.occurences++;
                    c2 = c3 = '\0';
                    break;
                }
            }

        const auto result = stats.emplace(c, 1);
        if (!result.second) {
            auto& stat = *result.first;
            ++stat.second;
        }
    } while (file.good());

    for (const auto& s : stats)
        cout << "[" << setfill('0') << setw(3) << static_cast<int>(s.first) << "] " << s.first << ": " << s.second << "\n";
    cout << "Diacritics found:\n";
    for (const auto& d : g_diacritics)
        if (d.occurences > 0)
            cout << d.character << ": " << d.occurences << "\n";
}