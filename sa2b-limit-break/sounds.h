#pragma once

void Sounds_Init();
void Sounds_OnFrame();

struct SoundHeader 
{
	int StartIndex; // the code that output a free index starts searching from this index
	int OverflowIndex; // if the list is full, it will replace sounds starting from 38, this is between 38 and 43.
	int unk3;
	int unk4;
};

struct SoundEntry
{
	__int8 unk1;
	__int8 unk2;
	__int8 Bank;
	__int8 unk4;
	__int8 unk5;
	__int8 VolumeA;
	__int8 VolumeB;
	__int8 unk6;
	__int16 unk7;
	__int16 unk8;
	__int16 unk9;
	__int16 unk10;
	int Entity;
	int ID;
	int unk11;
	NJS_VECTOR Position;
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
	void* unk9;
	void* unk10;
	void* unk11;
	void* unk12;
	void* unk13;
	void* unk14;
	void* unk15;
	void* unk16;
};

// The game's struct allocated in memory
struct SoundSystem {
	SoundHeader soundHeader; // 16 bytes
	SoundEntry soundEntries[43]; // 48 bytes each
	SoundSystemPointers soundHead; // 64 bytes
};

static constexpr int SoundCount_Expended = 255;
static constexpr int SoundLimit_Expended = 250;
static constexpr int SoundLimit2_Expended = 244;

struct SoundSystem_Expended {
	SoundHeader soundHeader; // 16 bytes
	SoundEntry soundEntries[SoundCount_Expended]; // 48 bytes each
	SoundSystemPointers soundHead; // 64 bytes
};

DataPointer(SoundSystem*, SoundSystemPtr, 0x1A55874);