#include "stdafx.h"
#include "clipdist.h"
#include "objects.h"
#include "sound.h"
#include "collisions.h"
#include "drawqueue.h"

const HelperFunctions* gHelperFunctions;

extern "C"
{
	__declspec(dllexport) void Init(const char *path, const HelperFunctions &helperFunctions)
	{
		gHelperFunctions = &helperFunctions;
		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
		
		ClipDist_Init(config);
		Objects_Init(config);
		Sound_Init(config);
		Collisions_Init(config);
		DrawQueue_Init(config);
		
		delete(config);
	}

	__declspec(dllexport) void OnFrame()
	{
		for (int i = 0; i < 43; ++i)
		{
			gHelperFunctions->SetDebugFontSize(10);
			auto& soundentry = SoundSystemPtr->SoundEntries[i];
			if (soundentry.SoundID != -1)
				gHelperFunctions->SetDebugFontColor(0xFF00FF00);
			else
				gHelperFunctions->SetDebugFontColor(0xBBBBBBBB);
			if (soundentry.Flags & SoundFlags::SndFlag_AutoVolume)
				gHelperFunctions->SetDebugFontColor(0xFFAAFF00);
			gHelperFunctions->DisplayDebugStringFormatted(NJM_LOCATION(2, 2 + i), "[%i] BANK %i | SOUND %i | ENTITY %x | FLAG %x", i, soundentry.Bank, soundentry.SoundID == -1 ? -1 : (soundentry.SoundID & 0xFFF), soundentry.Entity, soundentry.Flags);
		}
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}