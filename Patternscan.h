#pragma once
//#include <Windows.h>
//#include <TlHelp32.h>
#include "Memory.h"

uintptr_t GetProcId(const wchar_t* procname);

uintptr_t GetModuleBaseAddress(uintptr_t procId, const wchar_t* modname);
uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);
MODULEENTRY32 GetModule(uintptr_t dwProcID, const wchar_t* moduleName);

void* PatternScan(char* base, size_t size, const char* pattern, const char* mask);
void* PatternScanEx(HANDLE hProcess, uintptr_t begin, uintptr_t end, const char* pattern, const char* mask);
uintptr_t PatternScanExModule(HANDLE hProcess, const wchar_t* exeName, const wchar_t* module, const char* pattern, const char* mask);