#include "stdafx.h"

static constexpr int MaxPlayers = 2;

static Uint32 ClipDistanceMultiplier = 10000;
static Uint32 LandClipDistanceMultiplier = 100;

static Trampoline* LoadLandManager_t;
static Trampoline* LoadChunkLandManager_t;

static int __cdecl ClipObject_r(ObjectMaster* obj, float dist) {
	if (dist == 0.0f) {
		return 0;
	}

	if (obj->SETData) {
		if (obj->SETData->Flags & 0x8) {
			return 0;
		}

		ObjectListEntry* obj_entry = &CurrentObjectList->List[obj->SETData->SETEntry->ID & 0x7FFF];

		// 0 and 1 aren't level objects
		if (obj_entry->List > 1) {
			dist *= ClipDistanceMultiplier;
		}
	}
	else {
		dist *= ClipDistanceMultiplier;
	}

	NJS_VECTOR* pos = &obj->Data1.Entity->Position;

	if (isLoading == false && CameraData1 &&
		SETDistanceCheckThing(&CameraData1->Position, pos->x, pos->y, pos->z, dist)) {
		return 0;
	}

	for (int i = 0; i < MaxPlayers; ++i) {
		if (MainCharObj1[i] && SETDistanceCheckThing(&MainCharObj1[i]->Position, pos->x, pos->y, pos->z, dist)) {
			return 0;
		}
	}

	obj->MainSub = DeleteObject_;
	return 1;
}

static void __declspec(naked) ClipObject_()
{
	__asm
	{
		push[esp + 04h] // distance
		push edx // object

		call ClipObject_r

		pop edx // object
		add esp, 4 // distance
		retn
	}
}

bool __cdecl SETDistanceCheckThing_r(NJS_VECTOR* from, float x, float y, float z, float dist) {
	dist *= ClipDistanceMultiplier;

	return SETDistanceCheckThing(from, x, y, z, dist);
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
		call SETDistanceCheckThing_r
		add esp, 4
		pop ecx // from
		add esp, 12
		retn
	}
}

bool __cdecl SETDistanceCheckThing2P_r(NJS_VECTOR* from, NJS_VECTOR* p2pos, float x, float y, float z, float dist) {
	dist *= ClipDistanceMultiplier;

	return SETDistanceCheckThing2P(from, p2pos, x, y, z, dist);
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
		call SETDistanceCheckThing2P_r
		add esp, 4 // from<eax> is also used for return value
		pop ecx // p2pos
		add esp, 12
		retn
	}
}

// todo: replace the visible list by vectors and do that in ListGroundForDrawing
void IncreaseLandTable(LandTable* land) {
	land->field_C *= LandClipDistanceMultiplier;

	if (CurrentLevel != LevelIDs_HiddenBase && CurrentLevel != LevelIDs_HiddenBase2P) {
		for (int col = 0; col < land->COLCount; ++col) {
			COL* currentcol = &land->COLList[col];

			if (currentcol->Flags & SurfaceFlag_Visible) {
				currentcol->Radius *= LandClipDistanceMultiplier;
				currentcol->field_14 = 0;
				currentcol->field_18 = 0;
			}
		}
	}
}

void __cdecl LoadLandManager_r(LandTable* land) {
	NonStaticFunctionPointer(void, original, (LandTable * land), LoadLandManager_t->Target());

	IncreaseLandTable(land);

	original(land);
}

void __cdecl LoadChunkLandManager_r(LandTable* land) {
	NonStaticFunctionPointer(void, original, (LandTable * land), LoadChunkLandManager_t->Target());

	IncreaseLandTable(land);

	original(land);
}

void ClipDist_Init(const IniFile* config) {
	if (config->getBool("Clip", "ClipDist", true)) {
		WriteJump((void*)0x488C80, ClipObject_);
		WriteCall((void*)0x488717, SETDistanceCheckThing_asm);
		WriteCall((void*)0x4884D5, SETDistanceCheckThing2P_asm);

		ClipDistanceMultiplier = config->getInt("", "ClipMultiplier", ClipDistanceMultiplier);
	}

	if (config->getBool("Clip", "LandDist", true)) {
		LoadLandManager_t = new Trampoline((int)LoadLandManager, (int)LoadLandManager + 0x7, LoadLandManager_r);
		LoadChunkLandManager_t = new Trampoline(0x492C70, 0x492C77, LoadChunkLandManager_r);

		LandClipDistanceMultiplier = config->getInt("", "LandMultiplier", LandClipDistanceMultiplier);
	}
}