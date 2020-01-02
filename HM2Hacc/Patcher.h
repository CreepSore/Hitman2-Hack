#pragma once
#include <Windows.h>
#define PATCHTYPE_ADDRESS 0x0
#define PATCHTYPE_PATTERN 0x1

class PatchInfo
{
private:
    bool patchPattern();
    bool patchAddress();
    char* oldBytes;
    int type = 0;
public:
    DWORD64 address = 0;
    bool patched = false;
    char* pattern;
    char* mask;
    char* module;
    char* toWrite;
    bool patch();
    bool unpatch();

    PatchInfo(DWORD64 address, const char* toWrite);
    PatchInfo(const char* module, const char* pattern, const char* mask, const char* toWrite);
};