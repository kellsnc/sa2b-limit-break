#include "stdafx.h"

static Uint32 DrawDistanceMultiplier = 10000;
static constexpr int MaxPlayers = 2;

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
			dist *= DrawDistanceMultiplier;
		}
	}
	else {
		dist *= DrawDistanceMultiplier;
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

void SETDistanceCheckThing_r(SETObjectData* SETData, NJS_VECTOR* from, float x, float y, float z, float dist) {
	ObjectListEntry* obj_entry = &CurrentObjectList->List[SETData->SETEntry->ID & 0x7FFF];

	if (obj_entry->List > 1) {
		dist *= DrawDistanceMultiplier;
	}

	SETDistanceCheckThing(from, x, y, z, dist);
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
		push esi // hack to get SETObjectData
		call SETDistanceCheckThing_r
		pop esi
		pop ecx // from
		add esp, 4 // x
		add esp, 4 // y
		add esp, 4 // z
		add esp, 4 // dist
		retn
	}
}

void SETDistanceCheckThing2P_r(SETObjectData* SETData, NJS_VECTOR* from, NJS_VECTOR* p2pos, float x, float y, float z, float dist) {
	ObjectListEntry* obj_entry = &CurrentObjectList->List[SETData->SETEntry->ID & 0x7FFF];

	if (obj_entry->List > 1) {
		dist *= DrawDistanceMultiplier;
	}

	SETDistanceCheckThing2P(from, p2pos, x, y, z, dist);
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
		push esi // hack to get SETObjectData
		call SETDistanceCheckThing2P_r
		add esp, 4 // from<eax> is also used for return value
		pop esi
		pop ecx // p2pos
		add esp, 4 // x
		add esp, 4 // y
		add esp, 4 // z
		add esp, 4 // dist
		retn
	}
}

void DrawDist_Init() {
	WriteJump((void*)0x488C80, ClipObject_);
	WriteCall((void*)0x488717, SETDistanceCheckThing_asm);
	WriteCall((void*)0x4884D5, SETDistanceCheckThing2P_asm);
}