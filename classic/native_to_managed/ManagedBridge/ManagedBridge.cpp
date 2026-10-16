#include "NativeWorker.h"

using namespace System;
using namespace System::Runtime::InteropServices;

[UnmanagedFunctionPointer(CallingConvention::StdCall)]
public delegate void CompletionDelegate(int value);

public ref class ManagedBridge
{
  public:
    ManagedBridge()
    {
        _worker = new NativeWorker();
        CompletionDelegate^ callback = gcnew CompletionDelegate(this, &ManagedBridge::OnCompleted);
        _callbackHandle = GCHandle::Alloc(callback);
        IntPtr callbackPointer = Marshal::GetFunctionPointerForDelegate(callback);
        _worker->SetCallback(reinterpret_cast<managed_function_t>(callbackPointer.ToPointer()));
    }

    ~ManagedBridge()
    {
        this->!ManagedBridge();
    }

    void Run()
    {
        _worker->Run();
    }

  private:
    void OnCompleted(int value)
    {
        Console::WriteLine("ManagedBridge: received {0}", value);
    }

    !ManagedBridge()
    {
        if (_worker != nullptr) {
            _worker->SetCallback(nullptr);
            delete _worker;
            _worker = nullptr;
        }

        if (_callbackHandle.IsAllocated) {
            _callbackHandle.Free();
        }
    }

    NativeWorker* _worker = nullptr;
    GCHandle _callbackHandle;
};