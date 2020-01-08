#include "stdafx.h"

DataArray(ObjectMaster*, PlayerCollision_List, 0x1946660, 16);
DataArray(ObjectMaster*, CollisionList_1_List, 0x19466A0, 128);
DataArray(ObjectMaster*, CollisionList_2_List, 0x1DE4EE0, 128);
DataArray(ObjectMaster*, CollisionList_3_List, 0x1DE6DA0, 128);
DataArray(ObjectMaster*, CollisionList_4_List, 0x1945E18, 511);
DataArray(ObjectMaster*, CollisionList_5_List, 0x19468B8, 128);
DataArray(ObjectMaster*, CollisionList_6_List, 0x1946AC8, 128);
DataArray(ObjectMaster*, CollisionList_7_List, 0x1946CC8, 128);
DataArray(ObjectMaster*, CollisionList_8_List, 0x1945A08, 256);
DataArray(ObjectMaster*, CollisionList_9_List, 0x1946618, 16);

DataPointer(int, PlayerCollision_Count, 0x1946AB8);
DataPointer(int, CollisionList_1_Count, 0x1945E0C);
DataPointer(int, CollisionList_2_Count, 0x1DE8C2A);
DataPointer(int, CollisionList_3_Count, 0x1DE944C);
DataPointer(int, CollisionList_4_Count, 0x194665C);
DataPointer(int, CollisionList_5_Count, 0x1946AC0);
DataPointer(int, CollisionList_6_Count, 0x19468B0);
DataPointer(int, CollisionList_7_Count, 0x1945E10);
DataPointer(int, CollisionList_8_Count, 0x1946658);
DataPointer(int, CollisionList_9_Count, 0x1946ABC);

struct CollisionThing
{
	char field_0;
	char field_1;
	__int16 FlagsMaybe;
	EntityData1 *Entity;
};

struct CollisionInfo2
{
	__int16 List;
	__int16 ThingCount;
	__int16 Flags;
	__int16 Count;
	float Radius;
	CollisionData *CollisionArray;
	CollisionThing CollisionThings[16];
	int CollisionThingsEnd;
	int field_94;
	int field_98;
	ObjectMaster *Object;
	__int16 field_A0;
	__int16 field_A2;
	CollisionInfo *CollidingObject;
};

static const void *const Collision_InitThings = (void*)0x47E6C0;

// Add the object to a collision list for that frame
// Only if the player is close enough
void AddToCollisionList_r(ObjectMaster *a1)
{
	CollisionInfo2 *collision = (CollisionInfo2*)a1->Data1.Entity->Collision;
	int count;

	NJS_VECTOR* pos = &a1->Data1.Entity->Position;
	float dist = 168100;
	if (a1->SETData && a1->SETData->field_C) dist = a1->SETData->field_C;

	if (collision && (SETDistanceCheckThing(&MainCharObj1[0]->Position, pos->x, pos->y, pos->z, dist) ||
		(MainCharObj1[1] && SETDistanceCheckThing(&MainCharObj1[1]->Position, pos->x, pos->y, pos->z, dist)))) {
		__asm {
			mov     eax, esi
			call    Collision_InitThings
		}

		switch (collision->List)
		{
		case 0:
			if (PlayerCollision_Count < 16u)
			{
				PlayerCollision_List[PlayerCollision_Count++] = a1;
			}
			break;
		case 1:
			if (CollisionList_1_Count < 128u)
			{
				CollisionList_1_List[CollisionList_1_Count++] = a1;
			}
			break;
		case 2:
			if (CollisionList_2_Count < 128u)
			{
				CollisionList_2_List[CollisionList_2_Count++] = a1;
			}
			break;
		case 3:
			if (CollisionList_3_Count < 128u)
			{
				CollisionList_3_List[CollisionList_3_Count++] = a1;
			}
			break;
		case 4:
			if (CollisionList_4_Count < 511u)
			{
				CollisionList_4_List[CollisionList_4_Count++] = a1;
			}
			break;
		case 5:
			if (CollisionList_5_Count < 128u)
			{
				CollisionList_5_List[CollisionList_5_Count++] = a1;
			}
			break;
		case 6:
			if (CollisionList_6_Count < 128u)
			{
				CollisionList_6_List[CollisionList_6_Count++] = a1;
			}
			break;
		case 7:
			if (CollisionList_7_Count < 128u)
			{
				CollisionList_7_List[CollisionList_7_Count++] = a1;
			}
			break;
		case 8:
			if (CollisionList_8_Count < 256u)
			{
				CollisionList_8_List[CollisionList_8_Count++] = a1;
			}
			break;
		case 9:
			if (CollisionList_9_Count < 16u)
			{
				CollisionList_9_List[CollisionList_9_Count++] = a1;
			}
			break;
		default:
			return;
		}
	}
}

static void __declspec(naked) AddToCollisionList_()
{
	__asm
	{
		push esi // a1

		// Call your __cdecl function here:
		call AddToCollisionList_r

		pop esi // a1
		retn
	}
}

void Collision_Init() {
	WriteJump((void*)0x47E750, AddToCollisionList_);
}