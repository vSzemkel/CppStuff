
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
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
    // check input
    filesystem::path file_path;
    if (argc > 1)
        file_path = argv[1];
    else {
        file_path = argv[0];
        file_path.concat(".cpp");
    }

    if (!filesystem::is_regular_file(file_path)) {
        cout << file_path << " is not a regular file.\n";
        return EXIT_FAILURE;
    }

    ifstream file(file_path);
    if (file)
        cout << "Character statistics for: " << file_path << ".\n";
    else
        return EXIT_FAILURE;

    // compute statistics
    map<const char, uint> stats;
    auto start = chrono::steady_clock::now();
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
                    break;
                } else {
                    if (c3 != '\0')
                        c3 = file.get();
                    if (d.character.c_str()[2] != c3)
                        continue;
                    d.occurences++;
                    break;
                }
            }

        if (c2 != '\0') {
            c2 = c3 = '\0';
            continue;
        }
        
        const auto result = stats.emplace(c, 1);
        if (!result.second) {
            auto& stat = *result.first;
            ++stat.second;
        }
    } while (file.good());

    // present results
    chrono::duration<double> duration = chrono::steady_clock::now() - start;
    auto eof = stats.find((const char)-1);
    if (eof != stats.end())
        stats.erase(eof);
    eof = stats.find((const char)10);
    if (eof != stats.end())
        stats.erase(eof);
    for (const auto& s : stats)
        cout << "[" << setfill('0') << setw(3) << static_cast<int>(s.first) << "] " << s.first << ": " << s.second << "\n";
    cout << "Diacritics found:\n";
    for (const auto& d : g_diacritics)
        if (d.occurences > 0)
            cout << d.character << ": " << d.occurences << "\n";
    cout << "Processing time: " << duration.count() << " seconds.\n";
}