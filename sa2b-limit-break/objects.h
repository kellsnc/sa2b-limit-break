#pragma once

void Objects_Init(const IniFile* config);

//char __usercall Play3DSound_EntityAndPosOnce@<al>(EntityData1 *entity@<ebx>, int id@<edi>, NJS_VECTOR *position@<esi>, char bank, char volume, __int16 idk)
static const void* const Play3DSound_EntityAndPosOncePtr = (void*)0x437590;
static inline char Play3DSound_EntityAndPosOnce(EntityData1* entity, int id, NJS_VECTOR* pos, char bank, char volume, __int16 idk)
{
	char result;
	__asm
	{
		push[idk]
		push[volume]
		push[bank]
		mov esi, [pos]
		mov edi, [id]
		mov ebx, [entity]
		call Play3DSound_EntityAndPosPtr
		mov result, al
		add esp, 12
	}
	return result;
}

//void __usercall DeleteSoundsOfObject(EntityData1* data@<edx>)
static const void* const DeleteSoundsOfObjectPtr = (void*)0x437E10;
static inline void DeleteSoundsOfObject(EntityData1* entity)
{
	__asm
	{
		mov edx, [entity]
		call DeleteSoundsOfObjectPtr
	}
}