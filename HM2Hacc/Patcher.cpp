#include "stdafx.h"
#include "Patcher.h"
#include "SignatureScanning.h"
#include <iostream>

PatchInfo::PatchInfo(const DWORD64 address, const char* toWrite, const char* name) {
    this->type = PATCHTYPE_ADDRESS;
    this->address = address;
    this->toWrite = new char[255];
    this->name = new char[255];

    this->oldBytes = nullptr;
    this->module = nullptr;
    this->pattern = nullptr;
    this->mask = nullptr;

    memcpy(this->name, name, strlen(name) + 1);
    memcpy(this->toWrite, toWrite, strlen(toWrite) + 1);
}

PatchInfo::PatchInfo(const char* module, const char* pattern, const char* mask, const char* toWrite, const char* name) {
    this->type = PATCHTYPE_PATTERN;
    this->address = 0;

    this->module = new char[strlen(module) + 1];
    this->pattern = new char[strlen(mask) + 1];
    this->mask = new char[strlen(mask) + 1];
    this->toWrite = new char[strlen(toWrite) + 1];
    this->name = new char[255];

    this->oldBytes = nullptr;

    memcpy(this->name, name, strlen(name) + 1);
    memcpy(this->module, module, strlen(module) + 1);
    memcpy(this->pattern, pattern, strlen(mask) + 1);
    memcpy(this->mask, mask, strlen(mask) + 1);
    memcpy(this->toWrite, toWrite, strlen(toWrite) + 1);
}

bool PatchInfo::patchPattern() {
    if (this->prefetchAddress())
    {
        return this->patchAddress();
    }

    return false;
}

bool PatchInfo::patchAddress() {
    if (this->address == 0) return false;

    SIZE_T len = strlen(this->toWrite);
    this->oldBytes = new char[len];
    for (int i = 0; i < len; i++)
    {
        PCHAR(this->oldBytes)[i] = PCHAR(this->address)[i];
    }

    len = strlen(this->toWrite);
    DWORD oldProt = 0;
    VirtualProtect(LPVOID(this->address), len, PAGE_EXECUTE_READWRITE, &oldProt);
    for (int i = 0; i < len; i++)
    {
        *PBYTE(this->address + i) = this->toWrite[i];
    }
    VirtualProtect(LPVOID(this->address), len, oldProt, &oldProt);

    return true;
}

bool PatchInfo::prefetchAddress() {
    this->type = PATCHTYPE_ADDRESS;
    this->address = SigScanner::findPattern(this->module, this->pattern, this->mask);
    if (this->address == 0)
    {
        std::cout << "Couldn't find Signature for pattern with mask [" << this->mask << "]";
        return false;
    }
    return true;
}

bool PatchInfo::patch() {
    if (patched) return false;

    bool success = false;
    switch (this->type)
    {
    case PATCHTYPE_ADDRESS: {
        success = this->patchAddress();
        break;
    }

    case PATCHTYPE_PATTERN: {
        success = this->patchPattern();
        break;
    }
    default: break;
    }

    return this->patched = success;
}

bool PatchInfo::unpatch() {
    if (!this->patched) return false;

    const SIZE_T len = strlen(this->toWrite);
    DWORD oldProt = 0;
    VirtualProtect(LPVOID(this->address), len, PAGE_EXECUTE_READWRITE, &oldProt);
    for (int i = 0; i < len; i++)
    {
        PCHAR(this->address)[i] = this->oldBytes[i];
    }
    VirtualProtect(LPVOID(this->address), len, oldProt, &oldProt);

    this->patched = false;
    return true;
}
