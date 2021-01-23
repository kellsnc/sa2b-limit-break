#pragma once

void Sounds_Init();

struct soundHeader 
{
	int unk1;
	int unk2;
	int unk3;
	int unk4;
};

struct SoundEntry
{
	__int8 unk1;
	__int8 unk2;
	__int8 unk3;
	__int8 unk4;
	__int8 unk5;
	__int8 VolumeA;
	__int8 VolumeB;
	__int8 unk6;
	__int16 unk7;
	__int16 unk8;
	__int16 unk9;
	__int16 unk10;
	int entity;
	int ID;
	int unk11;
	NJS_VECTOR pos;
	Float unk12;
	Float unk13;
};

struct SoundSystemPointers
{
	void* unk1;
	void* unk2;
	void* unk3;
	void* unk4;
	void* unk5;
	void* unk6;
	void* unk7;
	void* unk8;
};

// The game's struct allocated in memory, everything is accessed relative to this D:
struct SoundSystem {
	soundHeader soundHeader; // 16 bytes
	SoundEntry soundEntries[43]; // 48 bytes each
	SoundSystemPointers soundHead; // 32 bytes
};

VoidFunc(SetWholeSoundQueueSomeFlag, 0x437E90);
VoidFunc(FreeSoundQueueAndLevelSounds, 0x435F80);
DataPointer(SoundEntry*, SoundQueuePtr, 0x1A55874);
DataPointer(int, dword_1AF0ECC, 0x1AF0ECC);
DataPointer(char, byte_174AFD9, 0x174AFD9);
DataArray(char, byte_A00648, 0xA00648, 8);

//void __usercall sub_459070(int character@<eax>, void *address@<ecx>, int count, int idk)
static const void* const sub_459070Ptr = (void*)0x459070;
static inline void sub_459070(int character, void* address, int count, void* idk)
{
	__asm
	{
		push[idk]
		push[count]
		mov ecx, [address]
		mov eax, [character]
		call sub_459070Ptr
		add esp, 8
	}
}