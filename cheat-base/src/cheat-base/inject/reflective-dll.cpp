#include <pch.h>
#include "reflective-dll.h"
#include <string>

std::string wstr_to_str(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], static_cast<int>(wstr.size()), &str[0], size_needed, nullptr, nullptr);
    return str;
}

bool ReflectiveLoadLibrary(HANDLE hProc, const std::wstring& dllPath) {
    HMODULE hKernel32 = GetModuleHandleA("Kernel32");
    LPVOID lpLoadLibraryA = GetProcAddress(hKernel32, "LoadLibraryA");
    if (lpLoadLibraryA == NULL) {
        return false;
    }

    LPVOID lpRemoteMemory = VirtualAllocEx(hProc, NULL, dllPath.size(), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (lpRemoteMemory == NULL) {
        return false;
    }

    SIZE_T nBytesWritten;
    if (!WriteProcessMemory(hProc, lpRemoteMemory, &dllPath[0], dllPath.size(), &nBytesWritten)) {
        VirtualFreeEx(hProc, lpRemoteMemory, 0, MEM_RELEASE);
        return false;
    }

    HANDLE hThread = CreateRemoteThread(hProc, NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(lpLoadLibraryA), lpRemoteMemory, 0, NULL);
    if (hThread == NULL) {
        VirtualFreeEx(hProc, lpRemoteMemory, 0, MEM_RELEASE);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);

    DWORD exitCode;
    if (!GetExitCodeThread(hThread, &exitCode)) {
        CloseHandle(hThread);
        VirtualFreeEx(hProc, lpRemoteMemory, 0, MEM_RELEASE);
        return false;
    }

    CloseHandle(hThread);
    VirtualFreeEx(hProc, lpRemoteMemory, 0, MEM_RELEASE);

    return exitCode != NULL;
}