#include "stdafx.h"
#include "SA2ModLoader.h"
#include "IniFile.hpp"

#include "clip.h"
#include "set.h"
#include "sound.h"
#include "collision.h"
#include "drawqueue.h"

#ifdef _DEBUG
const HelperFunctions* gHelperFunctions;
#endif

extern "C"
{
	__declspec(dllexport) void Init(const char *path, const HelperFunctions &helperFunctions)
	{
#ifdef _DEBUG
		gHelperFunctions = &helperFunctions;
#endif

		const IniFile* config = new IniFile(std::string(path) + "\\config.ini");
		
		Clip_Init(config);
		Set_Init(config);
		Sound_Init(config);
		Collisions_Init(config);
		DrawQueue_Init(config);
		
		delete(config);
	}

#ifdef _DEBUG
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
#endif

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}