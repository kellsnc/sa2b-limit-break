#include "stdafx.h"

float DrawDistance = 30000000;

static int __cdecl SETDistanceCheckThing_r(NJS_VECTOR* from, float x, float y, float z, float range) {
	return SETDistanceCheckThing(from, x, y, z, DrawDistance);
}

static void __declspec(naked) SETDistanceCheckThing_()
{
	__asm
	{
		push[esp + 10h] // range
		push[esp + 10h] // z
		push[esp + 10h] // y
		push[esp + 10h] // x
		push ecx // from

		call SETDistanceCheckThing_r

		pop ecx // from
		add esp, 4
		add esp, 4
		add esp, 4
		add esp, 4
		retn
	}
}

static int __cdecl ClipObject_r(ObjectMaster* a1, float dist) {
	if (a1->SETData && a1->SETData->Flags & 0x8) {
		return 0;
	}
	if (dist == 0) {
		return 0;
	}
	
	NJS_VECTOR* pos = &a1->Data1.Entity->Position;

	if (a1->DisplaySub || !a1->DeleteSub) {
		if (SETDistanceCheckThing(&MainCharObj1[0]->Position, pos->x, pos->y, pos->z, DrawDistance)) {
			return false;
		}

		a1->MainSub = DeleteObject_;
		return true;
	}
	else {
		if (SETDistanceCheckThing(&MainCharObj1[0]->Position, pos->x, pos->y, pos->z, dist)) {
			return false;
		}
		
		a1->MainSub = DeleteObject_;
		return true;
	}
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

void DrawDist_Init() {
	WriteJump((void*)0x488C80, ClipObject_);
	WriteCall((void*)0x488717, SETDistanceCheckThing_);
}

void DrawDist_OnFrame() {
	if (GameState == 7 && CurrentLandTable) {
		if (CurrentLandTable->COLList[0].field_18 != 0xFF) {
			for (uint16_t col = 0; col < CurrentLandTable->ChunkModelCount; ++col) {
				COL* currentcol = &CurrentLandTable->COLList[col];
				
				currentcol->Radius = DrawDistance;
				currentcol->field_14 = 0;
				currentcol->field_18 = 0;
			}

			CurrentLandTable->COLList[0].field_18 = 0xFF;
		}
	}
}