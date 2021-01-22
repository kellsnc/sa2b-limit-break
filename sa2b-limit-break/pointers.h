#pragma once

VoidFunc(RunObjectCollisions, 0x486190);
VoidFunc(RunPlayerCollision, 0x485920);
VoidFunc(RunProjectileCollisionn, 0x485B20);
VoidFunc(RunChaoCollision, 0x485C70);
VoidFunc(RunEnemyCollision, 0x485E10);
VoidFunc(RunRegularCollision, 0x485EF0);

FunctionPointer(void, CheckCollision, (ObjectMaster* entity, ObjectMaster* test), 0x485850);

DataArray(ObjectMaster*, CollisionList_1_List, 0x19466A0, 128);
DataArray(ObjectMaster*, CollisionList_2_List, 0x1DE4EE0, 128);
DataArray(ObjectMaster*, CollisionList_3_List, 0x1DE6DA0, 128);
DataArray(ObjectMaster*, CollisionList_4_List, 0x1945E18, 511);
DataArray(ObjectMaster*, CollisionList_5_List, 0x19468B8, 128);
DataArray(ObjectMaster*, CollisionList_6_List, 0x1946AC8, 128);
DataArray(ObjectMaster*, CollisionList_7_List, 0x1946CC8, 128);
DataArray(ObjectMaster*, CollisionList_8_List, 0x1945A08, 256);
DataArray(ObjectMaster*, CollisionList_9_List, 0x1946618, 16);

DataPointer(unsigned __int16, PlayerCollision_Count, 0x1946AB8);
DataPointer(unsigned __int16, CollisionList_1_Count, 0x1945E0C);
DataPointer(unsigned __int16, CollisionList_2_Count, 0x1DE8C2A);
DataPointer(unsigned __int16, CollisionList_3_Count, 0x1DE944C);
DataPointer(unsigned __int16, CollisionList_4_Count, 0x194665C);
DataPointer(unsigned __int16, CollisionList_5_Count, 0x1946AC0);
DataPointer(unsigned __int16, CollisionList_6_Count, 0x19468B0);
DataPointer(unsigned __int16, CollisionList_7_Count, 0x1945E10);
DataPointer(unsigned __int16, CollisionList_8_Count, 0x1946658);
DataPointer(unsigned __int16, CollisionList_9_Count, 0x1946ABC);

DataPointer(unsigned __int16, TargetCollisions_LastCount, 0x1DE4EC8);
DataPointer(unsigned __int16, EnemiesCollisions_LastCount, 0x1DE944A);

//void __usercall Collision_InitThings(ObjectMaster *obj@<eax>)
static const void* const Collision_InitThingsPtr = (void*)0x47E6C0;
static inline void Collision_InitThings(ObjectMaster* obj)
{
	__asm
	{
		mov eax, [obj]
		call Collision_InitThingsPtr
	}
}

//ObjectMaster *__usercall GetClosestPlayer@<eax>(ObjectMaster *player@<ebx>, CharObj2Base *co2@<esi>, float *dist_output)
static const void* const GetClosestPlayerPtr = (void*)0x74CC40;
static inline ObjectMaster* GetClosestPlayer(ObjectMaster* player, CharObj2Base* co2, float* dist_output)
{
	ObjectMaster* result;
	__asm
	{
		push[dist_output]
		mov esi, [co2]
		mov ebx, [player]
		call GetClosestPlayerPtr
		mov result, eax
		add esp, 4
	}
	return result;
}