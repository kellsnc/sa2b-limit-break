#include "stdafx.h"

int CookieSize = 4;
int SoundQueue_SizeBytes = 2144 * 2; //Original size multiplied by 4
int SoundQueue_SizeBytesCookie = SoundQueue_SizeBytes + CookieSize;
char SoundEntriesCount = 44 * 2; //Original count multiplied by 4

struct SoundEntry
{
	int unk1;
	__int8 unk2;
	__int8 unk3;
	__int8 unk4;
	__int8 unk5;
	__int16 unk6;
	__int16 unk7;
	__int16 unk8;
	__int16 unk9;
	int unk10;
	int ID;
	int unk12;
	NJS_VECTOR pos;
	int unk13;
	int unk14;
};

DataPointer(SoundEntry*, SoundQueue, 0x1A55874);

void Sounds_Init() {
	MemoryManager->Deallocate((AllocatedMem*)SoundQueue - CookieSize, (char*)"..\\..\\src\\sound.c", 789); //Remove the allocated SoundQueue
	AllocatedMem *mem = MemoryManager->Allocate(SoundQueue_SizeBytesCookie, (char*)"..\\..\\src\\sound.c", 652); //Allocate our own.
	SoundQueue = (SoundEntry*)&mem->Data;
	mem->Cookie = 0x12345678;
	memset(&mem->Data, 0, SoundQueue_SizeBytes);

	WriteData((char*)0x437F03, SoundEntriesCount); //NoIdea, loaded in LoadKartSounds 0x4357C0
	WriteData((char*)0x435F24, SoundEntriesCount); //ResetSoundQueue 0x435F10
	WriteData((char*)0x436BC0, SoundEntriesCount); //In GetFreeSoundIndex
	WriteData((char*)0x436B93, SoundEntriesCount); //In GetFreeSoundIndex
	WriteData((char*)0x436B44, SoundEntriesCount); //In GetFreeSoundIndex
}

void Sounds_OnFrame() {
	SoundEntry* lol;
	
	for (char i = 0; i < SoundEntriesCount; ++i) {
		lol = &SoundQueue[i] + 16;
		lol = lol;
	}
}