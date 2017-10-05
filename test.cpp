
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

/* Shift-Japko-B to build */
/* INCLUDE brane z ustawień XCode: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1 */
/* Aby odpalić debuggowanie w GDB trzeba podpisać kod https://sourceware.org/gdb/wiki/BuildingOnDarwin */

int main()
{
    vector<int> vs{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vs.push_back(36);
    vs[0] = 812;

    cout << "Hello GCC " << vs[0] << endl;

    float moc;
    tuple<string, float, float> piwo("Okocim", 5.6, 2.65);
    tie(ignore, moc, ignore) = piwo;
    cout << "Piwo " << get<string>(piwo) << " ma " << moc << "% alkoholu i kosztuje " << get<2>(piwo) << "PLN" << endl;
    return 0;
}