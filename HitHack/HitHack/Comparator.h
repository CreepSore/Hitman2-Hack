#pragma once
#include <Windows.h>

struct Comparator {
	int Compare(DWORD64 obj1, DWORD64 obj2);
};

struct CharComparator : Comparator {
	int Compare(DWORD64 obj1, DWORD64 obj2);
};