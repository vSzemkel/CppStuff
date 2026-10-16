#include "NativeWorker.h"

#include <iostream>

#ifdef _MANAGED
#error NativeWorker.cpp must be compiled without CLR support
#endif

void NativeWorker::SetCallback(managed_function_t callback)
{
    _callback = callback;
}

void NativeWorker::Run()
{
    if (_callback != nullptr) {
        std::cout << "NativeWorker: invoking callback" << std::endl;
        _callback(42);
    }
}