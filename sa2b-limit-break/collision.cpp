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

	if (entities[2].size() > 0) {
		WriteData((ObjectMaster***)0x4C4ADD, &entities[2][0]);
		WriteData((ObjectMaster***)0x74A25D, &entities[2][0]);
	}

	if (entities[3].size() > 0) {
		WriteData((ObjectMaster***)0x4C4AF7, &entities[3][0]);
		WriteData((ObjectMaster***)0x74A266, &entities[3][0]);
	}
	
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
		entities[i].clear();
	}
}

ObjectMaster* GetClosestCollisionDyn(int index, CharObj2Base* co2, float* dist) {

}

static void __cdecl ScanMechTargets_r(CharObj2Base* co2, MechEggmanCharObj2* eggco2) {
	if (eggco2->field_35C && eggco2->field_368 < 32) {
		float dist_target = 0;
		float dist_temp_target = 0;

		ObjectMaster* target = GetClosestCollisionDyn(2, co2, &dist_target);
		ObjectMaster* temp_target = GetClosestCollisionDyn(3, co2, &dist_temp_target);
		ObjectMaster* target_player = MainCharacter[1];

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

		if (co2->PlayerNum) {
			target_player = MainCharacter[0];
		}
		if (target_player) {
			ObjectMaster* playerobj = GetClosestPlayer(target_player, co2, &dist_temp_target);

			if (*&dist_temp_target > 0.0 && *&dist_target > *&dist_temp_target) {
				target = playerobj;
			}
		}

		// todo : continue
	}
}

static void __declspec(naked) ScanMechTargets_asm()
{
	__asm
	{
		push[esp + 04h] // eggco2
		push eax // co2
		call sub_74CCF0
		pop eax
		add esp, 4
		retn
	}
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
}