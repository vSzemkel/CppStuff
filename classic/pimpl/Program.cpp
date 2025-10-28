
#include "PimplSample.h"

#include <chrono>
#include <iostream>
#include <thread>

int main()
{
    PimplSample ps("Jozefa");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << ps.HowOldAreYou() << "\n";
}
