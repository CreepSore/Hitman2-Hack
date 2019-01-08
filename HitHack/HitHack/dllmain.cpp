// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
#include "stdafx.h"
#include "Vectors.h"
#include <stdio.h>
#include <iostream>

void InitConsole() {
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
	std::cout << "Console initialized";
}

BOOL __stdcall StartThread(LPVOID hModule) {
	Beep(500, 500);
	InitConsole();

	Beep(250, 500);
	FreeLibraryAndExitThread((HMODULE)hModule, 0);
	return TRUE;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
		case DLL_PROCESS_ATTACH:
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)StartThread, (LPVOID)hModule, NULL, NULL);
			break;
    }

    return TRUE;
}

