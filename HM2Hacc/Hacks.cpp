#include "stdafx.h"
#include "Hacks.h"
#include "SignatureScanning.h"
#include "Patcher.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <experimental/filesystem>

PatchInfo pInfiniteAmmo = PatchInfo("hitman2.exe",
    "\xff\x90\x00\x00\x00\x00\x83\xbb\x00\x00\x00\x00\x00\x76",
    "xx????xx?????x",
    "\x90\x90\x90\x90\x90\x90");

PatchInfo pAutoPistol = PatchInfo("hitman2.exe",
    "\xff\x83\x00\x00\x00\x00\x33\xd2\x48\x8b\x07",
    "xx????xxxxx",
    "\x90\x90\x90\x90\x90\x90");

vec3d* getPosition() {
    DWORD64 base = DWORD64(GetModuleHandleA("hitman2.exe"));
    DWORD64 curraddr = *PDWORD64(base + 0x2BD6A38);

    if (curraddr == 0) return nullptr;

    PDWORD64 p1 = PDWORD64(curraddr + 0x20);
    if (!p1 || p1 == nullptr || *p1 == 0) return nullptr;

    return reinterpret_cast<vec3d*>(*p1 + 0x218);
}

void Hooks::hook() {
    // Infinite Ammo
    if(pInfiniteAmmo.patch())
    {
        std::cout << "Hooked infinite Ammo: 0x" << std::hex << pInfiniteAmmo.address << std::dec << std::endl;
    }

    // Auto Pistol
    if(pAutoPistol.patch())
    {
        std::cout << "Hooked Auto Pistol: 0x" << std::hex << pAutoPistol.address << std::dec << std::endl;
    }
}

void Hooks::unhook() {
    // Infinite Ammo
    pInfiniteAmmo.unpatch();

    // Auto Pistol
    pAutoPistol.unpatch();
}


vec3d Jump::latestPos = vec3d();
void Jump::run() {
    vec3d* pos = getPosition();
    if (!pos || pos == nullptr) return;

    if (GetAsyncKeyState(VK_LMENU)) {
        pos->z = Jump::latestPos.z + 5;
        //jumpstep = 1;
        return;
    }

    if (GetAsyncKeyState(VK_BACK)) {
        pos->z = Jump::latestPos.z - 5;
        return;
    }

    if (GetAsyncKeyState(VK_NUMPAD5)) {
        pos->z = 2;
        jumpstep = -5;
        return;
    }

    if (GetAsyncKeyState(VK_NUMPAD4)) {
        pos->y = latestPos.y + 5;
        return;
    }

    if (GetAsyncKeyState(VK_NUMPAD6)) {
        pos->y = latestPos.y - 5;
        return;
    }

    if (GetAsyncKeyState(VK_NUMPAD8)) {
        pos->x = latestPos.x + 5;
        return;
    }

    if (GetAsyncKeyState(VK_NUMPAD2)) {
        pos->x = latestPos.x - 5;
        return;
    }



    if (jumpstep != -1) {
        if (GetTickCount64() - lastJumpUpdate > 25) {
            latestPos.z += jumpstep;
            jumpstep -= 0.05f;

            if (latestPos.z < 0) {
                latestPos.z = 0;
                jumpstep = -1;
            }

            lastJumpUpdate = GetTickCount64();
        }
        else
        {
            pos->z = latestPos.z;
        }
        return;
    }

    Jump::latestPos = *pos;
}