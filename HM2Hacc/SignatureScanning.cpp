#include "stdafx.h"
#include "SignatureScanning.h"
#include <Windows.h>
#include <iostream>

DWORD64 SigScanner::findPattern(const DWORD64 addrFrom, const DWORD64 addrTo, const char* pattern, const char* mask)
{
    const int patternLength = strlen(mask);
    if (patternLength == 0) return 0;

    for (DWORD64 i = addrFrom; i < addrTo; i++)
    {
        char* current = reinterpret_cast<char*>(i);
        bool patternFailed = false;
        for (int pOff = 0; pOff < patternLength; pOff++)
        {
            if (mask[pOff] == '?' || pattern[pOff] == current[pOff])
            {
                continue;
            }
            patternFailed = true;
        }
        if (!patternFailed)
        {
            return DWORD64(i);
        }
    }

    return 0;
}

DWORD64 SigScanner::findPattern(const HMODULE module, const char* pattern, const char* mask)
{
    DWORD64 from = DWORD64(module);
    DWORD64 to = DWORD64(module);
    MEMORY_BASIC_INFORMATION meminfo = MEMORY_BASIC_INFORMATION();

    while (VirtualQuery(reinterpret_cast<LPCVOID>(to), &meminfo, sizeof(MEMORY_BASIC_INFORMATION))) {
        if (DWORD64(meminfo.AllocationBase) != from)
            break;

        to += meminfo.RegionSize;
    }

    return findPattern(from, to, pattern, mask);
}

DWORD64 SigScanner::findPattern(const char* module, const char* pattern, const char* mask)
{
    return findPattern(GetModuleHandleA(module), pattern, mask);
}
