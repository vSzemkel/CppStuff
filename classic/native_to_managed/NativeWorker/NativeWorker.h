#pragma once

using managed_function_t = void(__stdcall*)(int);

class NativeWorker
{
public:
    void SetCallback(managed_function_t callback);
    void Run();

private:
    managed_function_t _callback = nullptr;
};