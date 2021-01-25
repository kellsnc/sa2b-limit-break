#include "stdafx.h"

extern "C"
{
	__declspec(dllexport) void Init(const char *path, const HelperFunctions &helperFunctions)
	{
		DrawDist_Init();
		Collisions_Init(); // Dynamic object collision lists
		Sounds_Init();
	}

	__declspec(dllexport) void OnFrame() {
		Sounds_OnFrame();
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}