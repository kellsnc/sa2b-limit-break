#include "stdafx.h"
#include "clipdist.h"
#include "objects.h"
#include "sound.h"
#include "collisions.h"

extern "C"
{
	__declspec(dllexport) void Init(const char *path, const HelperFunctions &helperFunctions)
	{
		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
		
		ClipDist_Init(config);
		Objects_Init(config);
		Sound_Init(config);
		
		if (config->getBool("Limits", "Collisions", true))
		{
			Collisions_Init();
		}
		
		delete(config);
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}