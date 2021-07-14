#include "stdafx.h"

#define TRAMPOLINE(name) ((decltype(name##_r)*)name##_t->Target())

static constexpr int MaxPlayers = 2;

static Trampoline* TaimatuExec_t = nullptr;

static float GetDistance(NJS_VECTOR* orig, NJS_VECTOR* dest)
{
	return sqrtf(powf(dest->x - orig->x, 2) + powf(dest->y - orig->y, 2) + powf(dest->z - orig->z, 2));
}

static bool isPlayerAround(NJS_VECTOR* position, float radius)
{
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
	TRAMPOLINE(TaimatuExec)(obj);

	EntityData1* data = obj->Data1.Entity;

	if (isPlayerAround(&data->Position, 200.0f) == true)
	{
		Play3DSound_EntityAndPos(data, 4, &data->Position, 0);
	}
}

void Objects_Init(const IniFile* config)
{
	if (config->getBool("Clip", "ClipDist", true))
	{
		// Fix Taimatu
		TaimatuExec_t = new Trampoline(0x630B50, 0x630B55, TaimatuExec_r, false);
		WriteData<5>(reinterpret_cast<void*>(0x630B32), 0x90); // nop sound queue registering
	}
}