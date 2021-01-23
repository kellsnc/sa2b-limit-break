#pragma once

void DrawDist_Init();

DataPointer(EntityData1*, CameraData1, 0x1A5A784);

//bool __usercall SETDistanceCheckThing2P@<eax>(NJS_VECTOR *from@<eax>, NJS_VECTOR *p2pos@<ecx>, float x, float y, float z, float dist)
static const void* const SETDistanceCheckThing2PPtr = (void*)0x4881F0;
static inline int SETDistanceCheckThing2P(NJS_VECTOR* from, NJS_VECTOR* p2pos, float x, float y, float z, float dist)
{
	int result;
	__asm
	{
		push[dist]
		push[z]
		push[y]
		push[x]
		mov ecx, [p2pos]
		mov eax, [from]
		call SETDistanceCheckThing2PPtr
		mov result, eax
		add esp, 16
	}
	return result;
}