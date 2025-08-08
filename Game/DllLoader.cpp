#include "DllLoader.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <filesystem>
#include <string>
namespace fs = std::filesystem;

DllHandle LoadDll(const char* path) 
{
    /*
      On Windows, LoadLibrary locks the file on disk, which prevents recompiling the DLL while the application is running. To avoid this, copy the DLL to a temporary uniquely named file (shadow copy) and load that instead.  
    */

    DllHandle result{ nullptr, {} };

    try
    {
        fs::path srcPath = fs::path(path);
        if (!fs::exists(srcPath))
        {
            // Fall back to trying to load directly (will fail similarly if missing)
            HMODULE direct = LoadLibraryA(path);
            result.handle = reinterpret_cast<void*>(direct);
            result.shadow_path = path;
            return result;
        }

        // Determine destination directory: use the executable directory
        char exePathBuffer[MAX_PATH]{0};
        GetModuleFileNameA(nullptr, exePathBuffer, MAX_PATH);
        fs::path exeDir = fs::path(exePathBuffer).parent_path();

        // Build a unique filename: GameLogic.shadow.<pid>.<tick>.dll
        DWORD pid = GetCurrentProcessId();
        DWORD ticks = GetTickCount64();

        std::string baseName = srcPath.stem().string();
        std::string uniqueName = baseName + ".shadow." + std::to_string(pid) + "." + std::to_string(ticks) + srcPath.extension().string();
        fs::path dstPath = exeDir / uniqueName;

        // Copy to destination (overwrite not expected due to uniqueness)
        fs::copy_file(srcPath, dstPath, fs::copy_options::overwrite_existing);

        HMODULE mod = LoadLibraryA(dstPath.string().c_str());
        result.handle = reinterpret_cast<void*>(mod);
        result.shadow_path = dstPath.string();
        return result;
    }
    catch (...)
    {
        // As a last resort, try direct load
        HMODULE mod = LoadLibraryA(path);
        result.handle = reinterpret_cast<void*>(mod);
        result.shadow_path = path;
        return result;
    }
}

void UnloadDll(DllHandle dll) 
{
    if (dll.handle) 
    {
        FreeLibrary(reinterpret_cast<HMODULE>(dll.handle));
    }

    // Attempt to delete the shadow copy after unloading. Ignore failures.
    if (!dll.shadow_path.empty())
    {
        try
        {
            fs::path p = fs::path(dll.shadow_path);
            // Only delete if it looks like one of our shadow copies
            std::string filename = p.filename().string();
            if (filename.find(".shadow.") != std::string::npos)
            {
                std::error_code ec;
                fs::remove(p, ec);
            }
        }
        catch (...)
        {
            // swallow
        }
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