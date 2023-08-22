#pragma once
#ifdef DYNAMICLIB_EXPORTS
#define DYNAMICLIB_API __declspec(dllexport)
#else
#define DYNAMICLIB_API __declspec(dllimport)
#endif

extern "C" DYNAMICLIB_API bool elementExists(int* a, int left, int right);
