#include "stdafx.h"
#include "Hacks.h"
#include "Patcher.h"
#include <Windows.h>
#include <iostream>
#include <experimental/filesystem>

PatchInfo pInfiniteAmmo = PatchInfo("hitman2.exe",
    "\xff\x90\x00\x00\x00\x00\x83\xbb\x00\x00\x00\x00\x00\x76",
    "xx????xx?????x",
    "\x90\x90\x90\x90\x90\x90",
    "INFINITE_AMMO");

PatchInfo pAutoPistol = PatchInfo("hitman2.exe",
    "\xff\x83\x00\x00\x00\x00\x33\xd2\x48\x8b\x07",
    "xx????xxxxx",
    "\x90\x90\x90\x90\x90\x90",
    "AUTO_PISTOL");

PatchInfo pNoSpread = PatchInfo("hitman2.exe",
    "\xf3\x0f\x59\x87\x00\x00\x00\x00\xf3\x0f\x58\xc6",
    "xxxx????xxxx",
    "\xF3\x41\x0F\x59\xC7\x90\x90\x90\x90\x90\x90\x90",
    "NO_SPREAD");

PatchInfo pAntiBotAwareness = PatchInfo("hitman2.exe",
    "\xe8\x00\x00\x00\x00\x8b\xd6\x49\x8b\xcd",
    "x????xxxxx",
    "\x90\x90\x90\x90\x90",
    "BOT_AWARENESS");

PatchInfo pNoBotActions = PatchInfo("hitman2.exe",
    "\xe8\x00\x00\x00\x00\x48\x8d\x55\x00\x48\x8b\xcf\xe8\x00\x00\x00\x00\x48\x83\xc3",
    "x????xxx?xxxx????xxx",
    "\x90\x90\x90\x90\x90",
    "BOT_ACTIONS");

PatchInfo pFreezeBots = PatchInfo("hitman2.exe",
    "\xe8\x00\x00\x00\x00\x80\xbb\x00\x00\x00\x00\x00\x75\x00\x48\x8d\x8b",
    "x????xx?????x?xxx",
    "\x90\x90\x90\x90\x90",
    "FREEZE_BOTS");

PatchInfo pFreezeBotAnimation = PatchInfo("hitman2.exe",
    "\xe8\x00\x00\x00\x00\x8b\x83\x00\x00\x00\x00\x48\x8d\x93\x00\x00\x00\x00\x48\x89\x54\x24",
    "x????xx????xxx????xxxx",
    "\x90\x90\x90\x90\x90",
    "FREEZE_BOT_ANIMATION");

PatchInfo pPhysXNoFalling = PatchInfo("PhysX3CharacterKinematic_x64.dll",
    "\xf2\x0f\x11\x8e",
    "xxxx",
    "\x90\x90\x90\x90\x90\x90\x90\x90",
    "PHYSX_NO_FALL");

std::vector<PatchInfo*> patches = std::vector<PatchInfo*>();

vec3d* Utils::getPosition() {
    auto currAddr = PDWORD64(DWORD64(GetModuleHandleA("hitman2.exe")) + 0x2BD6A38);

    if (!currAddr || currAddr == nullptr || *currAddr == 0) return nullptr;
    currAddr = PDWORD64(*currAddr + 0x20);
    if (!currAddr || currAddr == nullptr || *currAddr == 0) return nullptr;

    return reinterpret_cast<vec3d*>(*currAddr + 0x218);
}


void Hooks::init()
{
    patches.push_back(&pInfiniteAmmo);
    patches.push_back(&pAutoPistol);
    patches.push_back(&pNoSpread);
    patches.push_back(&pAntiBotAwareness);
    patches.push_back(&pNoBotActions);
    patches.push_back(&pFreezeBots);
    patches.push_back(&pFreezeBotAnimation);
}

void Hooks::hook() {
    for (PatchInfo* patch : patches)
    {
        patch->prefetchAddress();
        pPhysXNoFalling.prefetchAddress();
    }

    for (PatchInfo* patch : patches)
    {
        if (patch->patch())
            std::cout << "Hooked " << patch->name << ": 0x" << std::hex << patch->address << std::dec << std::endl;
    }
}

void Hooks::unhook() {
    for (PatchInfo* patch : patches)
    {
        patch->unpatch();
    }
    pPhysXNoFalling.unpatch();
}

void Hooks::loop() {
    if (GetAsyncKeyState(VK_NUMPAD7) & 0x1)
    {
        if (pFreezeBots.patched) {
            pFreezeBots.unpatch();
            pFreezeBotAnimation.unpatch();
            std::cout << "Unfreezed Bots" << std::endl;
        }
        else
        {
            pFreezeBots.patch();
            pFreezeBotAnimation.patch();
            std::cout << "Freezed Bots" << std::endl;
        }
    }

    if (GetAsyncKeyState(VK_NUMPAD9) & 0x1)
    {
        if (pNoBotActions.patched) {
            pNoBotActions.unpatch();
            std::cout << "Enabled Bot Actions" << std::endl;
        }
        else
        {
            pNoBotActions.patch();
            std::cout << "Disabled Bot Actions" << std::endl;
        }
    }

    if (GetAsyncKeyState(VK_NUMPAD1) & 0x1)
    {
        if (pAntiBotAwareness.patched) {
            pAntiBotAwareness.unpatch();
            std::cout << "Enabled Bot Awareness" << std::endl;
        }
        else
        {
            pAntiBotAwareness.patch();
            std::cout << "Disabled Bot Awareness" << std::endl;
        }
    }

    if (GetAsyncKeyState(VK_NUMPAD3) & 0x1)
    {
        if (!pPhysXNoFalling.patched) {
            pPhysXNoFalling.patch();
        }
        else
        {
            pPhysXNoFalling.unpatch();
        }
    }
}


double Teleport::speed = 2;
vec3d Teleport::latestPos = vec3d();
void Teleport::loop() {
    vec3d* pos = Utils::getPosition();
    if (!pos || pos == nullptr) return;

    // RESET VERTICAL HEIGHT
    if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000) {
        pos->z = 2;
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

    latestPos = *pos;
}


void Airwalk::loop() {
    vec3d* pos = Utils::getPosition();
    if (!pos || pos == nullptr) return;

    if (GetAsyncKeyState(VK_LMENU) & 0x8000) {
        if (!pPhysXNoFalling.patched) {
            pPhysXNoFalling.patch();
        }

        pos->z += 0.1;
        return;
    }

    if (GetAsyncKeyState(VK_BACK) & 0x8000) {
        if (!pPhysXNoFalling.patched) {
            pPhysXNoFalling.patch();
        }

        pos->z -= 0.1;
        return;
    }
}
