#include "DllLoader.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

DllHandle LoadDll(const char* path) 
{
    HMODULE mod = LoadLibraryA(path);
    return { reinterpret_cast<void*>(mod) };
}

void UnloadDll(DllHandle dll) 
{
    if (dll.handle) 
    {
        FreeLibrary(reinterpret_cast<HMODULE>(dll.handle));
    }
}

void* GetDllSymbol(DllHandle dll, const char* symbolName) 
{
    if (!dll.handle) 
    {
        return nullptr;
    }
    return reinterpret_cast<void*>(GetProcAddress(reinterpret_cast<HMODULE>(dll.handle), symbolName));
}