#include "stdafx.h"
#include "SA2ModLoader.h"
#include "IniFile.hpp"
#include "FastFunctionHook.hpp"
#include "objects.h"

// Patch objects relying on old clip behavior

UsercallFunctionPtr<TaskFuncPtr, noret, rEDX> CheckRangeOut(0x488C50);
DataArray(task*, ElvTp, 0x1AEDE40, 5);

FastFunctionHookPtr<TaskFuncPtr> OElv_h(0x609B60);
FastFunctionHookPtr<TaskFuncPtr> Rocket_Exec_h(0x6D50F0);

// Check if a task is within vanilla clip distance
bool CheckRangeVanilla(task* tp)
{
	taskwk* twp = tp->Data1.twp;
	for (int i = 0; i < 2; ++i)
	{
		if (playertwp[i])
		{
			Float x = playertwp[i]->pos.x - twp->pos.x;
			Float y = playertwp[i]->pos.y - twp->pos.y;
			Float z = playertwp[i]->pos.z - twp->pos.z;

			if (x * x + y * y + z * z < (tp->ocp ? tp->ocp->fRangeOut : 168100.0f))
			{
				return true;
			}
		}
	}
	return false;
}

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

// Fix rocket launcher opening too soon
void Rocket_Exec_r(task* tp)
{
	taskwk* twp = tp->Data1.twp;
	if (twp && twp->mode == 1)
	{
		CheckRangeOut(tp);
		if (!CheckRangeVanilla(tp))
		{
			return; // don't run opening behavior if too far
		}
	}
	Rocket_Exec_h.Original(tp);
}

void Objects_Init(const IniFile* config)
{
	if (config->getBool("Clip", "PatchObjects", true))
	{
		// Patch Prison Lane elevator
		OElv_h.Hook(OElv_r);
		WriteData((uint16_t*)0x609CFF, (uint16_t)0x36EB);
		WriteData((uint8_t*)0x609D38, (uint8_t)0xEB);

		// Patch Rocket
		Rocket_Exec_h.Hook(Rocket_Exec_r);
	}
}