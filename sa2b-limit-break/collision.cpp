#include "stdafx.h"

namespace CollisionList
{
	enum _enum
	{
		Players,
		Projectiles,
		Targetable,
		Enemies,
		Regular, // only checks collision with elements of list 5
		Unknown_5, // only checks collision with elements from its list
		Rings, // No interaction
		Unknown_7, // No interaction
		Unknown_8, // No interaction
		Chao,
		COUNT
	};
}

static std::vector<ObjectMaster*> entities[CollisionList::COUNT] = {};
static std::vector<ObjectMaster*> old_entities[CollisionList::COUNT] = {};

static void __cdecl AddToCollisionList_r(ObjectMaster* obj)
{
	const auto collision = obj->Data1.Entity->Collision;

	if (collision && collision->Object->MainSub != DeleteObject_)
	{
		Collision_InitThings(obj);

		const auto list = collision->char0; // collision->List

		if (list < 0 || list > 9)
		{
			return;
		}

		if (std::find(entities[list].begin(), entities[list].end(), obj) == entities[list].end())
		{
			entities[list].push_back(obj);
		}
	}
}

static void __declspec(naked) AddToCollisionList_asm()
{
	__asm
	{
		push esi
		call AddToCollisionList_r
		pop esi
		retn
	}
}

static void __cdecl CheckSelfCollision(int num)
{
	auto& list = entities[num];

	for (size_t i = 0; i < list.size(); i++)
	{
		for (Uint32 x = 0; x < list.size(); x++)
		{
			if (x != i)
			{
				CheckCollision(list[i], list[x]);
			}
		}
	}
}

static void __cdecl RunPlayerCollision_r()
{
	CheckSelfCollision(0);

	for (auto& i : entities[0])
	{
		for (auto& x : entities[2])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[3])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[4])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[5])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[6])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[7])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[8])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[9])
		{
			CheckCollision(i, x);
		}
	}
}

static void __cdecl RunProjectileCollision_r() {
	for (auto& i : entities[1])
	{
		for (auto& x : entities[0])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[2])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[3])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[4])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[5])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[9])
		{
			CheckCollision(i, x);
		}
	}
}

static void __cdecl RunChaoCollision_r() {
	CheckSelfCollision(9);

	for (auto& i : entities[9])
	{
		for (auto& x : entities[2])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[3])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[4])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[5])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[6])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[7])
		{
			CheckCollision(i, x);
		}
	}
}

static void __cdecl RunEnemyCollision_r() {
	CheckSelfCollision(3);

	for (auto& i : entities[3])
	{
		for (auto& x : entities[4])
		{
			CheckCollision(i, x);
		}

		for (auto& x : entities[5])
		{
			CheckCollision(i, x);
		}
	}
}

static void __cdecl RunRegularCollision_r()
{
	for (auto& i : entities[4])
	{
		for (auto& j : entities[5])
		{
			CheckCollision(i, j);
		}
	}
}

static void __cdecl RunCollision_5_r()
{
	CheckSelfCollision(5);
}

static void __cdecl ClearCollisionLists_r()
{
	TargetCollisions_LastCount = entities[CollisionList::Targetable].size();
	EnemiesCollisions_LastCount = entities[CollisionList::Enemies].size();

	for (size_t i = 0; i < CollisionList::COUNT; i++)
	{
		old_entities[i].clear();
		old_entities[i].reserve(entities->size());

		for (auto& j : entities[i]) {
			old_entities[i].push_back(j);
		}

		entities[i].clear();
	}
}

static void __cdecl ScanMechTargets_r(CharObj2Base* co2, MechEggmanCharObj2* eggco2) {
	if (eggco2->field_35C && eggco2->field_368 < 32) {
		float dist_target = 0;
		float dist_temp_target = 0;

		// Get objects that intersect with the mech ray
		ObjectMaster* target = GetTargetCollision(TargetCollisions_LastCount, co2, old_entities[CollisionList::Targetable].data(), &dist_target);
		ObjectMaster* temp_target = GetTargetCollision(EnemiesCollisions_LastCount, co2, old_entities[CollisionList::Enemies].data(), &dist_temp_target);
		
		if (target) {
			if (temp_target && dist_temp_target < dist_target) {
				dist_target = dist_temp_target;
				target = temp_target;
			}
		}
		else
		{
			target = temp_target;
		}

		// Get players that intersect with the mech ray
		ObjectMaster* target_player = MainCharacter[1];

		if (co2->PlayerNum) {
			target_player = MainCharacter[0];
		}
		if (target_player) {
			ObjectMaster* playerobj = GetTargetPlayer(target_player, co2, &dist_temp_target);

			if (dist_temp_target > 0.0 && dist_target > dist_temp_target) {
				target = playerobj;
			}
		}

		// Target acquired
		if (target) {
			EntityData1* target_data = target->Data1.Entity;

			if (!target_data || (target_data->field_2 != 22 && target_data->field_2 != 23)) {
				ObjectMaster* targettask = LoadChildObject((LoadObj)(LoadObj_Data1), (ObjectFuncPtr)0x74C260, MainCharacter[co2->PlayerNum]);
				
				if (targettask) {
					TargetThing* targetinfo = new TargetThing();

					targettask->Data2.Undefined = (void*)targetinfo;

					targetinfo->playernum = co2->PlayerNum;
					targetinfo->target = target;

					sub_47EEC0(target_data, target_data->Collision->CollisionArray, &targetinfo->position);
					sub_74AE30(eggco2, targettask);

					if (GetObjectColList(target)) {
						target->EntityData2->field_10 = *(float*)targettask;
					}
					else {
						*(&target->Data2.Undefined + 227) = (void*)targettask;
					}

					eggco2->field_368 += 1;
					eggco2->field_35E = 1;
					eggco2->field_390 = targetinfo->position.x;
					eggco2->field_394 = targetinfo->position.y;
					eggco2->field_398 = targetinfo->position.z;
					eggco2->field_366 = 7;
					targettask->field_1C = (ObjectFuncPtr)0x74BEC0;
					targettask->DeleteSub = (ObjectFuncPtr)0x74BDB0;
					PlaySoundProbably(8207, 0, 0, 0);
				}
				else {
					DeleteObject_(targettask);
				}
			}
		}
	}
}

static void __declspec(naked) ScanMechTargets_asm()
{
	__asm
	{
		push[esp + 04h] // eggco2
		push eax // co2
		call ScanMechTargets_r
		pop eax
		add esp, 4
		retn
	}
}

signed int sub_486D60(int a1)
{
	if (!a1) {
		return TargetCollisions_LastCount;
	}
	else if (a1 == 1) {
		return EnemiesCollisions_LastCount;
	}

	return -1;
}

bool sub_74A140(EntityData1* data, MechEggmanCharObj2* eggco2) {
	int v2; // ecx
	char v3; // al
	signed int v4; // eax
	MechEggmanCharObj2* v5; // eax
	Float v6; // ecx
	Float v7; // edx
	Float v8; // eax
	int v10; // ecx
	ObjectMaster** v11; // ebx
	int v12; // eax
	signed int v13 = 0; // ecx
	CollisionInfo* v14; // ecx
	ObjectMaster* v15; // eax
	EntityData1* v16; // eax
	CollisionData* v17; // ecx
	signed int v18; // [esp+14h] [ebp-20h]
	signed int v19; // [esp+18h] [ebp-1Ch]
	NJS_VECTOR vec1; // [esp+1Ch] [ebp-18h]
	NJS_VECTOR vec2; // [esp+28h] [ebp-Ch]

	if (data->Action && data->Action != 1 && data->Action != 17) {
		return 0;
	}
	
	int idk = sub_46DE60(0, eggco2->base.PlayerNum);

	if (idk == -1) {
		goto LABEL_23;
	}

	v5 = (MechEggmanCharObj2*)MainCharObj2[idk];
	vec1 = *(NJS_VECTOR*)v5->field_1BC[92];
	vec2 = { 1, 0, 0 };
	
	CalcVector_PlayerRot(data, &vec2);

	vec2 = { vec2.x * 20.0f, vec2.y * 20.0f, vec2.z * 20.0f };
	
	vec1.x = (v5->field_1BC[92] - v5->field_1BC[92]) - vec2.x;
	vec1.y = (v5->field_1BC[96] - v5->field_1BC[96]) - vec2.y;
	vec1.z = (v5->field_1BC[100] - v5->field_1BC[100]) - vec2.z;

	if (njScalor(&vec1) >= 15.0f)
	{
	LABEL_23:
		v10 = 2;
		v19 = 2;
		while (1)
		{
			if (v10 == 2)
			{
				v11 = old_entities[CollisionList::Targetable].data();
				v12 = 0;
			}
			else
			{
				v11 = old_entities[CollisionList::Enemies].data();
				v12 = 1;
			}
			v18 = sub_486D60(v12);
			if (v18 > 0)
			{
				break;
			}
		LABEL_19:
			v10 = v13 - 1;
			v19 = v10;
			if (v10 <= 0)
			{
				return 0;
			}
		}
		while (1)
		{
			v14 = v11[0]->Data1.Entity->Collision;
			++v11;
			if (v14)
			{
				v15 = v14->Object;
				if (v15)
				{
					v16 = v15->Data1.Entity;
					if (v16)
					{
						v17 = v14->CollisionArray;
						if (v17->kind != 12)
						{
							sub_47EEC0(v16, v17, &vec1);
							vec2.x = 1.0;
							vec2.z = 0.0;
							vec2.y = 0.0;
							CalcVector_PlayerRot(data, &vec2);
							vec2.x = vec2.x * 30.0;
							vec2.y = vec2.y * 30.0;
							vec2.z = 30.0 * vec2.z;
							vec1.x = vec1.x - *&eggco2->field_1BC[92];
							vec1.y = vec1.y - *&eggco2->field_1BC[96];
							vec1.z = vec1.z - *&eggco2->field_1BC[100];
							vec1.x = vec1.x - vec2.x;
							vec1.y = vec1.y - vec2.y;
							vec1.z = vec1.z - vec2.z;
							if (njScalor(&vec1) < 15.0)
							{
								break;
							}
						}
					}
				}
			}
			if (--v18 <= 0)
			{
				v13 = v19;
				goto LABEL_19;
			}
		}
	}
	return 1;
}

void Collision_Init() {
	WriteJump((void*)0x47E750, AddToCollisionList_asm);
	WriteJump((void*)0x485920, RunPlayerCollision_r);
	WriteJump((void*)0x485B20, RunProjectileCollision_r);
	WriteJump((void*)0x485C70, RunChaoCollision_r);
	WriteJump((void*)0x485E10, RunEnemyCollision_r);
	WriteJump((void*)0x485EF0, RunRegularCollision_r);
	WriteJump((void*)0x485F50, RunCollision_5_r);
	WriteJump((void*)0x485FD0, ClearCollisionLists_r);
	WriteJump((void*)0x74CCF0, ScanMechTargets_asm);
	WriteJump((void*)0x74A140, sub_74A140);

	WriteData<5>((void*)0x74BE8B, 0x90);
}