
#include <iostream>
#include <fstream>
#include <string>

// Remove single and multiline comments from a file

const std::string mlb = "/*"; // multiline comment begin
const std::string mle = "*/"; // multiline comment end
const std::string slb = "//"; // singleline comment begin
const std::string sle = "\n"; // singleline comment end

static std::string solve(const std::string& code) {
    std::string ret;
    ret.reserve(code.size());
    size_t pos{0}, next_slc = code.find(slb), next_mlc = code.find(mlb);

    while (true) {
        if (next_slc < pos)
            next_slc = code.find(slb, pos);
        if (next_mlc < pos)
            next_mlc = code.find(mlb, pos);

        if (next_slc != std::string::npos && (next_slc < next_mlc || next_mlc == std::string::npos)) {
            ret.insert(ret.end(), code.begin() + pos, code.begin() + next_slc);
            const auto next_slc_end = code.find(sle, next_slc);
            if (next_slc_end == std::string::npos)
                break;
            else {
                pos = next_slc_end + (pos == next_slc ? sle.size() : 0); // keep sle for non empty lines only
                next_slc = code.find(slb, pos);
            }
            continue;
        }

        if (next_mlc != std::string::npos && (next_mlc < next_slc || next_slc == std::string::npos)) {
            ret.insert(ret.end(), code.begin() + pos, code.begin() + next_mlc);
            const auto next_mlc_end = code.find(mle, next_mlc);
            if (next_mlc_end == std::string::npos) {
                std::cerr << "Unclosed multiline comment found at pos " << pos << '\n';
                break;
            } else {
                pos = next_mlc_end + mle.size(); // throw away mle
                next_mlc = code.find(mlb, pos);
            }
            continue;
        }

        ret.insert(ret.end(), code.begin() + pos, code.end());
        break;
    }

    return ret;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <src_file> [<dst_file>]\n";
        return 0;
    }

    std::ifstream src{argv[1]};
    if (!src.good()) {
        std::cout << "File: " << argv[1] << " can not be opened\n";
        return 0;
    }

    std::string code{std::istreambuf_iterator<char>(src), {}};
    src.close();

    std::ofstream dst{argv[argc - 1]};
    dst << solve(code);
    dst.close();
}

/*

Compile:
clang++.exe -Wall -Wextra -g -O0 -std=c++2a uncomment.cpp -o uncomment.exe
g++ -Wall -Wextra -g3 -Og -std=c++2a -fsanitize=address uncomment.cpp -o uncomment

Run:
uncomment.exe commented.cpp uncommented.cpp

*/
