#include "stdafx.h"
#include "Hacks.h"
#include <iostream>
#include <Windows.h>
#include <stdio.h>

using namespace std;

void __stdcall mainthread(const LPVOID param) {
    AllocConsole();
    freopen_s(reinterpret_cast<FILE * *>(stdout), "CONOUT$", "w", stdout);

    Hooks::init();
    Hooks::hook();

    while (!GetAsyncKeyState(VK_NUMPAD0)) {
        Hooks::loop();
        Teleport::loop();
        Airwalk::loop();
        Sleep(20);
    }

    Hooks::unhook();

    Beep(1024, 1000);
    FreeLibraryAndExitThread(HMODULE(param), 0);
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(mainthread), LPVOID(hModule), NULL, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

