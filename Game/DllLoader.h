#pragma once

#include <cstddef>

struct DllHandle 
{
    void* handle;
};

DllHandle LoadDll(const char* path);
void UnloadDll(DllHandle dll);
void* GetDllSymbol(DllHandle dll, const char* SYMBOL_NAME);