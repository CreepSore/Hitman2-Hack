#include "stdafx.h"
#include "Comparator.h"

int Comparator::Compare(DWORD64 obj1, DWORD64 obj2)
{
	int * o1;
	int * o2;

	o1 = (int*)obj1;
	o2 = (int*)obj2;

	if (*o1 < *o2) {
		return -1;
	}
	else if (*o1 == *o2) {
		return 0;
	}
	
	return 1;
}

int CharComparator::Compare(DWORD64 obj1, DWORD64 obj2)
{
	char curr1 = 0x1;
	char curr2 = 0x1;

	for (int i = 0; curr1 != '\0' && curr2 != '\0'; i++) {
		curr1 = *(char*)obj1;
		curr2 = *(char*)obj2;
		
		if (curr1 != curr2) {
			return -1;
		}
	}

	return 0;
}