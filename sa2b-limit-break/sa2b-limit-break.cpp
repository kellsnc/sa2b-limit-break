#include "stdafx.h"
#include "SA2ModLoader.h"
#include "IniFile.hpp"

#include "clip.h"
#include "set.h"
#include "sound.h"
#include "collision.h"
#include "dynacol.h"
#include "drawqueue.h"
#include "objects.h"

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
		Dynacol_Init(config);
		DrawQueue_Init(config);
		Objects_Init(config);
		
		delete(config);
	}

#ifdef _DEBUG
	__declspec(dllexport) void OnFrame()
	{
		int row = 0;
		for (int i = 0; i < 43; ++i)
		{
			if (i == 0)
			{
				gHelperFunctions->SetDebugFontColor(0xFFFFFFFF);
				gHelperFunctions->DisplayDebugString(NJM_LOCATION(2, 2 + row), "[RANGE 1]");
				++row;
			}
			else if (i == 32)
			{
				++row;
				gHelperFunctions->SetDebugFontColor(0xFFFFFFFF);
				gHelperFunctions->DisplayDebugString(NJM_LOCATION(2, 2 + row), "[RANGE 2]");
				++row;
			}
			else if (i == 38)
			{
				++row;
				gHelperFunctions->SetDebugFontColor(0xFFFFFFFF);
				gHelperFunctions->DisplayDebugString(NJM_LOCATION(2, 2 + row), "[RANGE 3]");
				++row;
			}

			gHelperFunctions->SetDebugFontSize(10);
			auto& soundentry = SoundSystemPtr->SoundEntries[i];
			if (soundentry.SoundID != -1)
				gHelperFunctions->SetDebugFontColor(0xFF00FF00);
			else
				gHelperFunctions->SetDebugFontColor(0xBBBBBBBB);
			if (soundentry.Flags & SoundFlags::SndFlag_AutoVolume)
				gHelperFunctions->SetDebugFontColor(0xFFAAFF00);
			gHelperFunctions->DisplayDebugStringFormatted(NJM_LOCATION(2, 2 + row), "[%i] BANK %i | SOUND %i | ID %x | FLAG %x", i, soundentry.Unknown1, soundentry.Unknown2, soundentry.Entity, soundentry.Flags);
			++row;
		}
	}
#endif

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}