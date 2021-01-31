/*
ID: marcin.8
LANG: C++
TASK: gift1
CONTENT: https://train.usaco.org/usacoprob2?a=CcFbOqTd1uM&S=gift1
*/

#include <iostream>
#include <fstream>
#include <unordered_map>

std::ifstream task_in("gift1.in");
std::ofstream task_out("gift1.out");

int main(int, char**)
{
    std::string n;
    int size; task_in >> size;
    std::unordered_map<int, std::string> order; // order number, name
    for (int i = 0; i < size; ++i) {
        task_in >> n;
        order[i] = n;
    }

    std::unordered_map<std::string, int> accounts; // name, balance
    for (int i = 0; i < size; ++i) {
        task_in >> n;
        int withdrawal, num_of_people; task_in >> withdrawal >> num_of_people;
        if (num_of_people == 0) continue;
        const int amount_to_give = withdrawal / num_of_people;
        const int amount_to_stay = withdrawal % num_of_people;
        accounts[n] += amount_to_stay - withdrawal;
        for (int j = 0; j < num_of_people; ++j) {
            task_in >> n;
            accounts[n] += amount_to_give;
        }
    }

    for (int i = 0; i < size; ++i)
        task_out << order[i] << " " << accounts[order[i]] << "\n";
}

/*

Compile:
clang++.exe -Wall -Wextra -ggdb3 -O0 -std=c++17 gift1.cpp -o gift1.exe
g++ -Wall -Wextra -ggdb3 -Og -std=c++17 gift1.cpp -o gift1.o

Input:

5
dave
laura
owen
vick
amr
dave
200 3
laura
owen
vick
owen
500 1
dave
amr
150 2
vick
owen
laura
0 2
amr
vick
vick
0 0

Output:

GO

*/
