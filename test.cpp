
#include <iostream>
#include <vector>
#include <tuple>

using namespace std;

int main() {
    vector<int> vs(10);
    vs.push_back(36);
    vs[0] = 812;

    cout << "Hello GCC " << vs[0] << endl;

    tuple<string, float> piwo("Okocim", 5.6);
    cout << "Piwo " << get<string>(piwo) << " ma " << get<1>(piwo) << "% alkoholu" << endl;
    return 0;
}