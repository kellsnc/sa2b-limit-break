#include "stdafx.h"
#include "FastFunctionHook.hpp"

static Uint32 ClipDistanceMultiplier = 70;
static Uint32 LandClipDistanceMultiplier = 5;
static bool RemoveChunks = false;

FastFunctionHook<void, LandTable*> LoadLandManager_h(0x47BD30);
FastFunctionHook<void, LandTable*> LoadChunkLandManager_h(0x492C70);
FastUsercallHookPtr<Bool(*)(NJS_VECTOR*, Float, Float, Float, Float), rEAX, rECX, stack4, stack4, stack4, stack4> SETDistanceCheckThing_h(0x488340);
FastUsercallHookPtr<Bool(*)(NJS_VECTOR*, NJS_VECTOR*, Float, Float, Float, Float), rEAX, rECX, stack4, stack4, stack4, stack4> SETDistanceCheckThing2P_h(0x4881F0);

Bool __cdecl SETDistanceCheckThing_r(NJS_VECTOR* from, Float x, Float y, Float z, Float dist)
{
	// Multiply the distance by the configured multiplier.
	dist *= ClipDistanceMultiplier;

	return SETDistanceCheckThing_h.Original(from, x, y, z, dist);
}

Bool __cdecl SETDistanceCheckThing2P_r(NJS_VECTOR* from, NJS_VECTOR* p2pos, Float x, Float y, Float z, Float dist)
{
	// Multiply the distance by the configured multiplier.
	dist *= ClipDistanceMultiplier;

	return SETDistanceCheckThing2P_h.Original(from, p2pos, x, y, z, dist);
}

// Todo: don't do this
void IncreaseLandTable(LandTable* land)
{
	land->ClippingDistance *= LandClipDistanceMultiplier;

	// Remove chunks
	if (RemoveChunks == true)
	{
		for (int col = 0; col < land->COLCount; ++col)
		{
			COL* currentcol = &land->COLList[col];

			if (currentcol->Flags & SurfaceFlag_Visible)
			{
				currentcol->Chunks = 0;
			}
		}
	}
}

void __cdecl LoadLandManager_r(LandTable* land)
{
	IncreaseLandTable(land);
	LoadLandManager_h.Original(land);
}

void __cdecl LoadChunkLandManager_r(LandTable* land)
{
	IncreaseLandTable(land);
	LoadChunkLandManager_h.Original(land);
}

void ClipDist_Init(const IniFile* config)
{
	if (config->getBool("Clip", "ClipDist", true))
	{
		SETDistanceCheckThing_h.Hook(SETDistanceCheckThing_r);
		SETDistanceCheckThing2P_h.Hook(SETDistanceCheckThing2P_r);

		ClipDistanceMultiplier = config->getInt("Clip", "ClipMultiplier", ClipDistanceMultiplier);
	}
	
	if (config->getBool("Clip", "LandDist", true))
	{
		LoadLandManager_h.Hook(LoadLandManager_r);
		LoadChunkLandManager_h.Hook(LoadChunkLandManager_r);

		LandClipDistanceMultiplier = config->getInt("Clip", "LandMultiplier", LandClipDistanceMultiplier);
		RemoveChunks = config->getBool("Clip", "RemoveChunks", RemoveChunks);
	}
}