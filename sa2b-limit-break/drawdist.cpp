#include "stdafx.h"

float DrawDistance = 30000000;

int IsPlayerInsideSphere(NJS_VECTOR *center, float radius) {
	for (uint8_t player = 0; player < 2; ++player) {
		if (!MainCharObj1[player]) continue;

		NJS_VECTOR *pos = &MainCharObj1[player]->Position;
		if ((powf(pos->x - center->x, 2) + pow(pos->y - center->y, 2) + pow(pos->z - center->z, 2)) <= pow(radius, 2)) {
			return player + 1;
		}
	}
	return 0;
}

static int __cdecl ClipObject_r(ObjectMaster* a1, float a2) {
	if (IsPlayerInsideSphere(&a1->Data1.Entity->Position, DrawDistance)) {
		return false;
	}
	
	return true;
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

static int __cdecl SETDistanceCheckThing_r(NJS_VECTOR *from, float x, float y, float z, float range) {
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
		add esp, 10h
		retn
	}
}

void DrawDist_Init() {
	WriteJump((void*)0x488C80, ClipObject_);
	WriteCall((void*)0x488717, SETDistanceCheckThing_);
}

void DrawDist_OnFrame() {

}