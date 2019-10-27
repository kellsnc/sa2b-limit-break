#include "stdafx.h"

extern "C"
{
	__declspec(dllexport) void Init(const char *path, const HelperFunctions &helperFunctions)
	{
		DrawDist_Init();
		Objects_Init();
	}

	__declspec(dllexport) void OnFrame() {
		DrawDist_OnFrame();
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}