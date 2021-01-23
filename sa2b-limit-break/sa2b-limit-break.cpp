#include "stdafx.h"

void DrawDist_Init();
void Sounds_Init();
void DrawDist_OnFrame();
void Sounds_OnFrame();

extern "C"
{
	__declspec(dllexport) void Init(const char *path, const HelperFunctions &helperFunctions)
	{
		//DrawDist_Init();
		Collisions_Init(); // Dynamic object collision lists
		//Sounds_Init();
	}

	__declspec(dllexport) void OnFrame() {
		//DrawDist_OnFrame();
		//Sounds_OnFrame();
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}