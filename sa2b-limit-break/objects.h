#pragma once

void Objects_Init(const IniFile* config);

//char __usercall Play3DSound_EntityAndPos@<al>(ObjectMaster* entity@<ebx>, int id@<edi>, NJS_VECTOR* position@<esi>, char volume)
//static const void* const Play3DSound_EntityAndPosPtr = (void*)0x437420;
//static inline void Play3DSound_EntityAndPos(ObjectMaster* entity, int id, NJS_VECTOR* position, char volume)
//{
//	__asm
//	{
//		push[volume]
//		mov esi, [position]
//		mov edi, [id]
//		mov ebx, [entity]
//		call Play3DSound_EntityAndPosPtr
//		add esp, 4
//	}
//}