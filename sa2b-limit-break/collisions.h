#pragma once

void Collisions_Init();

FastcallFunctionPointer(signed int, sub_46DE60, (int a1, int a2), 0x46DE60);

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