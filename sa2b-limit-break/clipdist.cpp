#include "stdafx.h"

static Uint32 ClipDistanceMultiplier = 70;
static Uint32 LandClipDistanceMultiplier = 5;

static Trampoline* LoadLandManager_t;
static Trampoline* LoadChunkLandManager_t;

static Trampoline* SETDistanceCheckThing_t;
static Trampoline* SETDistanceCheckThing2P_t;

static bool RemoveChunks = false;

signed int __cdecl SETDistanceCheckThing_original(NJS_VECTOR* from, float x, float y, float z, float dist)
{
	const auto fptr = SETDistanceCheckThing_t->Target();

	signed int result;

	__asm
	{
		push[dist]
		push[z]
		push[y]
		push[x]
		mov ecx, [from]
		call fptr
		mov result, eax
		add esp, 16
	}

	return result;
}

signed int __cdecl SETDistanceCheckThing_c(NJS_VECTOR* from, float x, float y, float z, float dist)
{
	// Multiply the distance by the specific multiplier.
	dist *= ClipDistanceMultiplier;

	return SETDistanceCheckThing_original(from, x, y, z, dist);
}

static void __declspec(naked) SETDistanceCheckThing_asm()
{
	__asm
	{
		push[esp + 10h] // dist
		push[esp + 10h] // z
		push[esp + 10h] // y
		push[esp + 10h] // x
		push ecx // from
		call SETDistanceCheckThing_c
		add esp, 4 // ret
		pop ecx // from
		add esp, 12
		retn
	}
}

signed int __cdecl SETDistanceCheckThing2P_original(NJS_VECTOR* from, NJS_VECTOR* p2pos, float x, float y, float z, float dist)
{
	const auto fptr = SETDistanceCheckThing2P_t->Target();

	signed int result;

	__asm
	{
		push[dist]
		push[z]
		push[y]
		push[x]
		mov ecx, [p2pos]
		mov eax, [from]
		call fptr
		mov result, eax
		add esp, 16
	}

	return result;
}

signed int __cdecl SETDistanceCheckThing2P_c(NJS_VECTOR* from, NJS_VECTOR* p2pos, float x, float y, float z, float dist)
{
	// Multiply the distance by the specific multiplier.
	dist *= ClipDistanceMultiplier;

	return SETDistanceCheckThing2P_original(from, p2pos, x, y, z, dist);
}

static void __declspec(naked) SETDistanceCheckThing2P_asm()
{
	__asm
	{
		push[esp + 10h] // dist
		push[esp + 10h] // z
		push[esp + 10h] // y
		push[esp + 10h] // x
		push ecx // p2pos
		push eax // from
		call SETDistanceCheckThing2P_c
		add esp, 4 // ret
		pop ecx // p2pos
		add esp, 16
		retn
	}
}

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
	NonStaticFunctionPointer(void, original, (LandTable * land), LoadLandManager_t->Target());

	IncreaseLandTable(land);

	original(land);
}

void __cdecl LoadChunkLandManager_r(LandTable* land)
{
	NonStaticFunctionPointer(void, original, (LandTable * land), LoadChunkLandManager_t->Target());

	IncreaseLandTable(land);

	original(land);
}

void ClipDist_Init(const IniFile* config)
{
	if (config->getBool("Clip", "ClipDist", true))
	{
		SETDistanceCheckThing_t = new Trampoline(0x488340, 0x488346, SETDistanceCheckThing_asm, false);
		SETDistanceCheckThing2P_t = new Trampoline(0x4881F0, 0x4881F5, SETDistanceCheckThing2P_asm, false);

		ClipDistanceMultiplier = config->getInt("Clip", "ClipMultiplier", ClipDistanceMultiplier);
	}
	
	if (config->getBool("Clip", "LandDist", true))
	{
		LoadLandManager_t = new Trampoline((int)LoadLandManager, (int)LoadLandManager + 0x7, LoadLandManager_r, false);
		LoadChunkLandManager_t = new Trampoline(0x492C70, 0x492C77, LoadChunkLandManager_r, false);

		LandClipDistanceMultiplier = config->getInt("Clip", "LandMultiplier", LandClipDistanceMultiplier);
		RemoveChunks = config->getBool("Clip", "RemoveChunks", RemoveChunks);
	}
}