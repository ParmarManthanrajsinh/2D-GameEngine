#pragma once

#include <cstddef>
#include <string>

struct DllHandle 
{
    void* handle;
    // Absolute path of the shadow-copied DLL actually loaded via LoadLibrary.
    // This allows unloading and deleting the copy so the original DLL remains
    // writable for recompilation while the application is running.
    std::string shadowPath;
};

DllHandle LoadDll(const char* path);
void UnloadDll(DllHandle dll);
void* GetDllSymbol(DllHandle dll, const char* symbolName);