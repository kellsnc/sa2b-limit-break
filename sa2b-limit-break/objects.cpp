#include "stdafx.h"
#include "SA2ModLoader.h"
#include "IniFile.hpp"
#include "FastFunctionHook.hpp"
#include "objects.h"

// Patch objects relying on old clip behavior

UsercallFunctionPtr<TaskFuncPtr, noret, rEDX> CheckRangeOut(0x488C50);
DataArray(task*, ElvTp, 0x1AEDE40, 5);

FastFunctionHookPtr<TaskFuncPtr> OElv_h(0x609B60);

// Fix mistake in Prison Lane elevator init function that incorrectly writes to its task array
void OElv_r(task* tp)
{
	OElv_h.Original(tp);
	for (int i = 0; i < 5; ++i)
	{
		if (ElvTp[i] == nullptr)
		{
			ElvTp[i] = tp;
			break; // <- this was missing
		}
	}
}

void Objects_Init(const IniFile* config)
{
	if (config->getBool("Clip", "ClipDist", true))
	{
		// Patch Prison Lane elevator
		OElv_h.Hook(OElv_r);
		WriteData((uint16_t*)0x609CFF, (uint16_t)0x36EB);
		WriteData((uint8_t*)0x609D38, (uint8_t)0xEB);
	}
}