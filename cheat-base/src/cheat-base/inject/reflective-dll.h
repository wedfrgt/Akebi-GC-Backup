#pragma once

#include <Windows.h>

typedef HMODULE(WINAPI* LoadLibrary_t)(LPCWSTR);
typedef FARPROC(WINAPI* GetProcAddress_t)(HMODULE, LPCSTR);

typedef struct _REFLECTIVE_LOADER_DATA {
	LoadLibrary_t pLoadLibrary;
	GetProcAddress_t pGetProcAddress;
} REFLECTIVE_LOADER_DATA, * PREFLECTIVE_LOADER_DATA;



bool ReflectiveLoadLibrary(HANDLE hProc, const std::wstring& dllPath, PVOID* pDllBase, DWORD* pDllSize, PVOID* pLoaderData);