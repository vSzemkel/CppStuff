
#include <algorithm>
#include <cstring>
#include <charconv>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <list>
#include <tuple>

using namespace std;

/* Shift-Japko-B to build */
/* INCLUDE brane z ustawień XCode: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1 */
/* Aby odpalić debuggowanie w GDB trzeba podpisać kod https://sourceware.org/gdb/wiki/BuildingOnDarwin */
/* LLDB: settings set target.x86-disassembly-flavor intel */

int main()
{
    vector<int> vs{1, 2, 3, 41, 5, 6, 7, 18, 9, 10};
    vs.push_back(36);

    int iconv = -91;
    const char sconv[] = "812";
    std::from_chars(sconv, sconv + sizeof(sconv), iconv);
    vs[0] = iconv;

    std::sort(vs.begin(), vs.end());
    auto p = std::lower_bound(vs.begin(), vs.end(), 8);
    cout << "Hello GCC: with vs lower bound for 8 is " << *p << endl;

    tuple<string, float, float> piwo("Okocim", 5.6f, 2.65f);
    float moc;
    tie(ignore, moc, ignore) = piwo;
    auto [_, moc2, ignore] = piwo; // structured binding
    cout << "Piwo " << get<string>(piwo) << " ma " << moc2 << "% alkoholu i kosztuje " << get<2>(piwo) << "PLN" << endl;

    list<int16_t> l;
    l.push_back(10);
    l.push_back(20);
    l.push_back(30);
    auto n = *std::next(begin(l), 1);

    auto pos = std::find(l.begin(), l.end(), n + 10);
    if (pos != end(l))
        n = std::distance(l.begin(), pos);

    auto a = iconv;
    if (int b = a + 4; b > 1)
        cout << "This variable is inline declared: " << b << endl;

    const char* path = "d:\\Code\\x\\IDA\\install.pass";
    auto slash = strrchr(path, '\\');
    std::string_view dir{path, static_cast<size_t>(slash - path)};
    cout << "Path: " << dir << '\n';

    string ts[] = {"abc", "def", "ghi"};

    return 0;
}
