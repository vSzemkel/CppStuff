
#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<int> vs(10);
    vs.push_back(36);
    vs[0] = 812;

    cout << "Hello GCC " << vs[0] << endl ;
    return 0;
}