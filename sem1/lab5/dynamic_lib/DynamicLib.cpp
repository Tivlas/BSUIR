#include "pch.h"
#include "DynamicLib.h"

bool elementExists(int* a, int left, int right) {

	if (left == right) {
		if (a[left] * a[left] + 2 < 1000) {
			return true;
		}
			return false;
	}
	int l = right - left; // length of the current segment
	int pos =  l / 3; // current position
	return elementExists(a, left, left+pos) || elementExists(a, left+pos+1, right);
}

