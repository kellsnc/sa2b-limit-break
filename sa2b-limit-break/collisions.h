#pragma once

void Collisions_Init();

VoidFunc(RunObjectCollisions, 0x486190);
VoidFunc(RunPlayerCollision, 0x485920);
VoidFunc(RunProjectileCollisionn, 0x485B20);
VoidFunc(RunChaoCollision, 0x485C70);
VoidFunc(RunEnemyCollision, 0x485E10);
VoidFunc(RunRegularCollision, 0x485EF0);

FunctionPointer(void, CheckCollision, (ObjectMaster* entity, ObjectMaster* test), 0x485850);
FastcallFunctionPointer(signed int, sub_46DE60, (int a1, int a2), 0x46DE60);

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

struct TargetThing
{
	int unk0;
	int unk4;
	__int8 playernum;
	__int8 unk9;
	__int16 unkA;
	int unkC;
	NJS_VECTOR position;
	ObjectMaster* target;
};

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
static const void* const GetTargetPlayerPtr = (void*)0x74CC40;
static inline ObjectMaster* GetTargetPlayer(ObjectMaster* player, CharObj2Base* co2, float* dist_output)
{
	ObjectMaster* result;
	__asm
	{
		push[dist_output]
		mov esi, [co2]
		mov ebx, [player]
		call GetTargetPlayerPtr
		mov result, eax
		add esp, 4
	}
	return result;
}

//ObjectMaster *__usercall GetTargetCollision@<eax>(int colcount@<eax>, CharObj2Base *co2@<edx>, ObjectMaster **collist@<ecx>, float *dist)
static const void* const GetTargetCollisionPtr = (void*)0x74CB30;
static inline ObjectMaster* GetTargetCollision(int colcount, CharObj2Base* co2, ObjectMaster** collist, float* dist_output)
{
	ObjectMaster* result;
	__asm
	{
		push[dist_output]
		mov ecx, [collist]
		mov edx, [co2]
		mov eax, [colcount]
		call GetTargetCollisionPtr
		mov result, eax
		add esp, 4
	}
	return result;
}

//void __usercall sub_47EEC0(EntityData1 *data@<eax>, CollisionData *col@<edi>, NJS_VECTOR *out_vec)
static const void* const sub_47EEC0Ptr = (void*)0x47EEC0;
static inline void sub_47EEC0(EntityData1* data, CollisionData* col, NJS_VECTOR* out_vec)
{
	__asm
	{
		push[out_vec]
		mov edi, [col]
		mov eax, [data]
		call sub_47EEC0Ptr
		add esp, 4
	}
}

//signed int __usercall GetObjectColList@<eax>(ObjectMaster *obj@<eax>)
static const void* const GetObjectColListPtr = (void*)0x486CB0;
static inline signed int GetObjectColList(ObjectMaster* obj)
{
	signed int result;
	__asm
	{
		mov eax, [obj]
		call GetObjectColListPtr
		mov result, eax
	}
	return result;
}

//void __usercall sub_74AE30(MechEggmanCharObj2 *eggco2@<ecx>, ObjectMaster *obj@<esi>)
static const void* const sub_74AE30Ptr = (void*)0x74AE30;
static inline void sub_74AE30(MechEggmanCharObj2* eggco2, ObjectMaster* obj)
{
	__asm
	{
		mov esi, [obj]
		mov ecx, [eggco2]
		call sub_74AE30Ptr
	}
}