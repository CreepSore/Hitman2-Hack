#include "stdafx.h"
#include "Hacks.h"
#include <Windows.h>
#include <iostream>
#include <stdio.h>

vec3d * getPosition() {
	DWORD64 base = (DWORD64)GetModuleHandleA("hitman2.exe");
	DWORD curraddr = *(DWORD*)(base + 0x2BC9668);

	DWORD * p1 = (DWORD*)(curraddr + 0x68);
	if (!p1 || p1 == nullptr) return (vec3d*)0;
	DWORD * p2 = (DWORD*)(*p1);
	if (!p2 || p2 == nullptr) return (vec3d*)0;


	return (vec3d*)(*p2 + 0x210);
}

DWORD64 getEntityList() {
	return (*(DWORD64*)((DWORD64)GetModuleHandleA("hitman2.exe") + 0x23B6B10)) + 0x9f10;
}

int * getEntityCount() {
	return (int*)((*(DWORD64*)((DWORD64)GetModuleHandleA("hitman2.exe") + 0x23B6B10)) + 0xbb30);
}


void Hooks::hook() {
	DWORD64 d = 0;
	BYTE b = 0;

	// Infinite Ammo
	d = 0x1451CBC42;
	b = 0x90;
	memset((void*)d, b, 7);

	// Animation
	d = 0x1451CBC20;
	memset((void*)d, b, 6);

	// Rapid Fire
	d = 0x1451cbc5c;
	memset((void*)d, b, 7);
}


vec3d Jump::latestPos = vec3d();
void Jump::run() {
	vec3d * pos = getPosition();
	if (pos == 0) return;



	if (GetAsyncKeyState(VK_LMENU)) {
		pos->z = Jump::latestPos.z + 5;
		//jumpstep = 1;
		return;
	}

	if (GetAsyncKeyState(VK_BACK)) {
		pos->z = Jump::latestPos.z - 5;
		return;
	} 

	if (GetAsyncKeyState(VK_RIGHT) & 1) {
		int * count = getEntityCount();
		currentEntityId++;
		if (currentEntityId > *count) {
			currentEntityId = 0;
		}
		vec3f * epos = (vec3f*)((*(DWORD64*)(getEntityList() + (0x10 * currentEntityId))) + 0x10);

		pos->z = epos->z;
		pos->x = epos->x;
		pos->y = epos->y;

		std::cout << "id: " << std::dec << currentEntityId << " vtable: " << std::hex << *(DWORD64*)(*(DWORD64*)(getEntityList() + (0x10 * currentEntityId))) << std::endl;
	}

	if (GetAsyncKeyState(VK_LEFT) & 1) {
		int * count = getEntityCount();
		currentEntityId--;
		if (currentEntityId < 0) {
			currentEntityId = *count;
		}

		vec3f * epos = (vec3f*)((*(DWORD64*)(getEntityList() + (0x10 * currentEntityId))) + 0x10);

		pos->z = epos->z;
		pos->x = epos->x;
		pos->y = epos->y;

		std::cout << "id: " << std::dec << currentEntityId << " vtable: " << std::hex << *(DWORD64*)(*(DWORD64*)(getEntityList() + (0x10 * currentEntityId))) << std::endl;
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