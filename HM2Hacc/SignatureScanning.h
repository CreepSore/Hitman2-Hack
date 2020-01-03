#pragma once
#include <Windows.h>
#include <string>

class SigScanner {
private:

public:
    static DWORD64 findPattern(DWORD64 addrFrom, DWORD64 addrTo, const char* pattern, const char* mask);
    static DWORD64 findPattern(HMODULE module, const char* pattern, const char* mask);
    static DWORD64 findPattern(const char* module, const char* pattern, const char* mask);
};
