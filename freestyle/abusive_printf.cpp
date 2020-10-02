#include <stdio.h>

int main() {
    // store a what(1337) in where(&dest) with printf %n specifier
    int dest{42};
    printf("%*i%n\n", 1337, 1, &dest);
    printf("dest == %i\n", dest);

    // positional printf parameter reference
    printf("Alice has got %i%1$i %s\n", 4, "cats");
}

/* clang++.exe -Wall -g -std=c++17 abusive_printfs.cpp -o abusive_printf.o

Output:

dest == 1337
Alice has got 44 cats

*/