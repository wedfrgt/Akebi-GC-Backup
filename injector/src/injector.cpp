#include "injector.h"
#include <cheat-base/inject/manual-map.h>
#include <cheat-base/inject/load-library.h>
#include <cheat-base/inject/reflective-dll.h>

bool InjectDLL(HANDLE hProc, const std::string& filepath)
{
#ifdef MANUAL_MAP
    bool result = ManualMapDLL(hProc, filepath);
#elif defined(REFLECTIVE_DLL)
    std::wstring wfilepath(filepath.begin(), filepath.end());
    bool result = ReflectiveLoadLibrary(hProc, wfilepath.c_str(), nullptr, nullptr, nullptr);
#else
    bool result = LoadLibraryDLL(hProc, filepath);
#endif
    return result;
}