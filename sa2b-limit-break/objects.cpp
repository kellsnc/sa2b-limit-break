#include "stdafx.h"
#include "SA2ModLoader.h"
#include "IniFile.hpp"
#include "FastFunctionHook.hpp"

UsercallFunctionPtr<Sint8(*)(EntityData1* entity, Sint32 id, NJS_VECTOR* pos, Sint8 volume), rAL, rEBX, rEDI, rESI, stack4> dsPlay_iloop_v(0x437420);
UsercallFunctionPtr<Sint8(*)(EntityData1* entity, Sint32 id, NJS_VECTOR* pos, Sint8 bank, Sint8 volume, Sint16 timer), rAL, rEBX, rEDI, rESI, stack4, stack4, stack4> dsPlay_timer_v(0x437590);
UsercallFunctionPtr<void(*)(Sint32 id), noret, rEDX> dsStop_twp(0x437E10);

FastFunctionHook<void, ObjectMaster*> TaimatuExec_h(0x630B50);

static constexpr int MaxPlayers = 2;

#ifdef _DEBUG
static constexpr uint32_t SETCount = 4096;
static SETObjectData SetTable[SETCount]{};
#endif

static float GetDistance(NJS_VECTOR* orig, NJS_VECTOR* dest)
{
	return sqrtf(powf(dest->x - orig->x, 2) + powf(dest->y - orig->y, 2) + powf(dest->z - orig->z, 2));
}

static bool isPlayerAround(NJS_VECTOR* position, float radius)
{
	if (CameraData1 && GetDistance(position, &CameraData1->Position) < radius)
	{
		return true;
	}

	for (int i = 0; i < MaxPlayers; ++i)
	{
		if (MainCharObj1[i])
		{
			if (GetDistance(position, &MainCharObj1[i]->Position) < radius)
			{
				return true;
			}
		}
	}

	return false;
}

static void __cdecl TaimatuExec_r(ObjectMaster* obj)
{
	TaimatuExec_h.Original(obj);

	EntityData1* data = obj->Data1.Entity;

	if (isPlayerAround(&data->Position, 300.0f) == true)
	{
		dsPlay_iloop_v(data, 4, &data->Position, 0);
		data->Index = 1;
	}
	else if (data->Index == 1)
	{
		dsStop_twp((Sint32)data);
		data->Index = 0;
	}
}

void Objects_Init(const IniFile* config)
{
	if (config->getBool("Clip", "ClipDist", true))
	{
		// Fix Taimatu
		TaimatuExec_h.Hook(TaimatuExec_r);
		WriteData<5>(reinterpret_cast<void*>(0x630B32), 0x90); // nop sound queue registering
	}

#ifdef _DEBUG
	if (config->getBool("Limits", "SetList", true))
	{
		WriteData(reinterpret_cast<SETObjectData**>(0x488652), &SetTable[0]); //LOADSET_1P
		WriteData(reinterpret_cast<SETObjectData**>(0x488424), &SetTable[0]); //LOADSET_2P
		WriteData(reinterpret_cast<SETObjectData**>(0x488960), &SetTable[0]);
		WriteData(reinterpret_cast<SETObjectData**>(0x488A23), &SetTable[0]);
		WriteData(reinterpret_cast<SETObjectData**>(0x4889BD), &SetTable[0]);
		WriteData(reinterpret_cast<SETObjectData**>(0x488A1F), &SetTable[0]);
		WriteData(reinterpret_cast<SETObjectData**>(0x488AFA), &SetTable[0]);
		WriteData(reinterpret_cast<SETObjectData**>(0x488B61), &SetTable[0]);
		WriteData(reinterpret_cast<SETObjectData**>(0x488BFE), &SetTable[0]);
		WriteData(reinterpret_cast<SETObjectData**>(0x4C7B7E), &SetTable[0]);

		WriteData(reinterpret_cast<uint32_t*>(0x48894D), SETCount);
		WriteData(reinterpret_cast<uint32_t*>(0x488952), SETCount);
		WriteData(reinterpret_cast<uint32_t*>(0x488BEC), SETCount);
		WriteData(reinterpret_cast<uint32_t*>(0x488BF3), SETCount);
	}
#endif
}