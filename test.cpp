
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

int main() {
    vector<int> vs(10);
    vs.push_back(36);
    vs[0] = 812;

    cout << "Hello GCC " << vs[0] << endl;

    float moc;
    tuple<string, float, float> piwo("Okocim", 5.6, 2.65);
    tie(ignore, moc, ignore) = piwo;
    cout << "Piwo " << get<string>(piwo) << " ma " << moc << "% alkoholu i kosztuje " << get<2>(piwo) << "PLN" << endl;
    return 0;
}