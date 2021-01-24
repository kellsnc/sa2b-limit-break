#include "stdafx.h"

void Sounds_Init() {
	MemoryManager->Deallocate((AllocatedMem*)SoundSystemPtr - 0x4, (char*)"..\\..\\src\\sound.c", 789);
	AllocatedMem* mem = MemoryManager->Allocate(sizeof(SoundSystem_Expended) + 0x4, (char*)"..\\..\\src\\sound.c", 652); //Allocate our own.
	SoundSystemPtr = (SoundSystem*)&mem->Data;
	mem->Cookie = 0x12345678;
	memset(&mem->Data, 0, sizeof(SoundSystem_Expended));

	// Now for replacing the offset of everything
	int soundlimithingoffset = sizeof(SoundHeader) + (sizeof(SoundEntry) * SoundLimit_Expended);
	int soundlimithing2offset = sizeof(SoundHeader) + (sizeof(SoundEntry) * SoundLimit2_Expended);
	int unk1offset = sizeof(SoundHeader) + (sizeof(SoundEntry) * SoundCount_Expended);
	int unk2offset = unk1offset + 4;
	int unk3offset = unk1offset + 8;
	int unk4offset = unk1offset + 12;
	int unk5offset = unk1offset + 16;
	int unk6offset = unk1offset + 20;
	int unk7offset = unk1offset + 24;
	int unk8offset = unk1offset + 28;
	int unk9offset = unk1offset + 32;
	int unk10offset = unk1offset + 36;
	int unk11offset = unk1offset + 40;
	int unk12offset = unk1offset + 44;
	int unk13offset = unk1offset + 48;
	int unk14offset = unk1offset + 52;
	int unk15offset = unk1offset + 56;
	int unk16offset = unk1offset + 60;

	// LoadStageSounds
	WriteData((int*)0x4355C0, unk13offset);
	WriteData((int*)0x4355C6, unk14offset);
	WriteData((int*)0x4355CC, unk15offset);
	WriteData((int*)0x4355D2, unk16offset);
	WriteData((int*)0x4355F7, unk13offset);
	WriteData((int*)0x435601, unk16offset);
	WriteData((int*)0x43560B, unk14offset);

	// LoadLevelSounds2
	WriteData((int*)0x43564E, unk13offset);
	WriteData((int*)0x435654, unk14offset);
	WriteData((int*)0x43565A, unk15offset);
	WriteData((int*)0x435660, unk16offset);
	WriteData((int*)0x435680, unk13offset);
	WriteData((int*)0x43568A, unk16offset);
	WriteData((int*)0x435694, unk14offset);
	WriteData((int*)0x4356A8, unk12offset);

	// LoadEventSounds
	WriteData((int*)0x435725, unk13offset);
	WriteData((int*)0x43572B, unk14offset);
	WriteData((int*)0x435731, unk15offset);
	WriteData((int*)0x435737, unk16offset);
	WriteData((int*)0x435769, unk9offset);
	WriteData((int*)0x435785, unk14offset);

	// LoadKartSounds
	WriteData((int*)0x435828, unk14offset);
	WriteData((int*)0x43582E, unk9offset);
	WriteData((int*)0x435838, unk10offset);
	WriteData((int*)0x435842, unk13offset);
	WriteData((int*)0x43584C, unk16offset);

	WriteData((int*)0x435F24, SoundCount_Expended); // ResetSoundQueue

	// ResetSoundSystem
	WriteData((int*)0x435F95, unk9offset);
	WriteData((int*)0x435F9B, unk10offset);
	WriteData((int*)0x435FA1, unk11offset);
	WriteData((int*)0x435FA7, unk12offset);
	WriteData((int*)0x435FAD, unk13offset);
	WriteData((int*)0x435FB3, unk14offset);
	WriteData((int*)0x435FB9, unk15offset);
	WriteData((int*)0x435FBF, unk16offset);

	// CheckLevelSoundListSomething
	WriteData((int*)0x4361F1, unk9offset);
	
	// sub_436AD0
	WriteData((int*)0x436B44, SoundCount_Expended);
	WriteData<1>((void*)0x436B93, static_cast<char>(SoundCount_Expended));
	WriteData<1>((void*)0x436BC0, static_cast<char>(SoundCount_Expended));
	WriteData((int*)0x436BFE, soundlimithing2offset);
	WriteData((int*)0x436C03, SoundLimit2_Expended);
	WriteData((int*)0x436C09, soundlimithing2offset);
	WriteData((int*)0x436C0E, SoundLimit_Expended - 1);
	WriteData<1>((void*)0x436C19, static_cast<char>(SoundLimit2_Expended));
	WriteData<1>((void*)0x436C4D, static_cast<char>(SoundLimit_Expended));
	WriteData<1>((void*)0x436B55, static_cast<char>(SoundLimit_Expended));
	WriteData<1>((void*)0x436B97, static_cast<char>(SoundLimit_Expended));
	WriteData<1>((void*)0x436BCD, static_cast<char>(SoundLimit_Expended));
	WriteData((int*)0x436B9D, soundlimithingoffset);
	WriteData((int*)0x436C8F, SoundLimit2_Expended);
	WriteData<1>((void*)0x436CC4, static_cast<char>(SoundLimit2_Expended));
	WriteData<1>((void*)0x436CE7, static_cast<char>(SoundLimit2_Expended));

	WriteData<1>((void*)0x436EBF, static_cast<char>(SoundCount_Expended)); // sub_436EA0
	WriteData<1>((void*)0x436F3B, static_cast<char>(SoundLimit_Expended)); // sub_436ED0
	WriteData<1>((void*)0x437003, static_cast<char>(SoundLimit_Expended)); // sub_436F90
	WriteData<1>((void*)0x43739F, static_cast<char>(SoundCount_Expended)); // sub_437360
	WriteData<1>((void*)0x43744F, static_cast<char>(SoundCount_Expended)); // Play3DSound_EntityAndPos
	WriteData<1>((void*)0x43750F, static_cast<char>(SoundCount_Expended)); // Play3DSound_Object
	WriteData<1>((void*)0x4375BF, static_cast<char>(SoundCount_Expended)); // OneOfTheManyPlay3DSound
	WriteData<1>((void*)0x437680, static_cast<char>(SoundCount_Expended)); // sub_437640
	WriteData<1>((void*)0x43773F, static_cast<char>(SoundCount_Expended)); // sub_437710
	WriteData((int*)0x4377EA, unk9offset); // sub_4377D0
	WriteData<1>((void*)0x437CA2, static_cast<char>(SoundCount_Expended)); // sub_437C70
	WriteData<1>((void*)0x437D44, static_cast<char>(SoundCount_Expended)); // sub_437D10
	WriteData<1>((void*)0x437F03, static_cast<char>(SoundCount_Expended)); // SetWholeSoundQueueSomeFlag
	WriteData((int*)0x437F6A, SoundCount_Expended); // sub_437F60
	WriteData((int*)0x437FBA, SoundCount_Expended); // sub_437FB0
	WriteData((int*)0x43800A, SoundCount_Expended); // sub_438000
	WriteData((int*)0x43805A, SoundCount_Expended); // sub_438050
	WriteData((int*)0x43809A, SoundCount_Expended); // sub_438090
	WriteData<1>((void*)0x438AD0, static_cast<char>(SoundCount_Expended)); // SoundLoopMaybe

	// sub_438B00
	WriteData((int*)0x438B2C, unk12offset);
	WriteData((int*)0x438B32, unk11offset);
	WriteData((int*)0x438B49, unk9offset);
	WriteData((int*)0x438B53, unk10offset);

	// sub_438B70
	WriteData((int*)0x438BBF, unk9offset);
	WriteData((int*)0x438BC9, unk14offset);

	
	WriteData((int*)0x438C6D, unk14offset); // sub_438C30
	WriteData((int*)0x44FB10, SoundCount_Expended); // CalcTotalScore

	// This game hates me
	WriteData<1>((void*)0x4996F6, static_cast<char>(SoundCount_Expended - 0x9)); // sub_499570
	WriteData<1>((void*)0x499881, static_cast<char>(SoundCount_Expended - 0x11)); // sub_4997D0
	WriteData<1>((void*)0x499A98, static_cast<char>(SoundCount_Expended - 0x3)); // sub_4999C0
	WriteData<1>((void*)0x499CED, static_cast<char>(SoundCount_Expended - 0x10)); // sub_499C70
	WriteData<1>((void*)0x49A393, static_cast<char>(SoundCount_Expended - 0x1)); // sub_49A2A0
	WriteData<1>((void*)0x4A1E22, static_cast<char>(SoundCount_Expended - 0x7)); // sub_49A2A0

	// sub_4A1DD0
	WriteData<1>((void*)0x4A1E22, static_cast<char>(SoundCount_Expended - 0x7)); 
	WriteData<1>((void*)0x4A1ED8, static_cast<char>(SoundCount_Expended - 0x8));

	// sub_4A2130
	WriteData<1>((void*)0x4A2164, static_cast<char>(SoundCount_Expended - 0x7));
	WriteData<1>((void*)0x4A2213, static_cast<char>(SoundCount_Expended - 0x8));

	// sub_4A22F0
	WriteData<1>((void*)0x4A2325, static_cast<char>(SoundCount_Expended - 0x7));
	WriteData<1>((void*)0x4A23D7, static_cast<char>(SoundCount_Expended - 0x8));

	WriteData<1>((void*)0x4A2732, static_cast<char>(SoundCount_Expended - 0x7)); // sub_4A2700

	// sub_4A34B0
	WriteData<1>((void*)0x4A3513, static_cast<char>(SoundCount_Expended - 0xD));
	WriteData<1>((void*)0x4A35C5, static_cast<char>(SoundCount_Expended - 0xD));
	WriteData<1>((void*)0x4A362E, static_cast<char>(SoundCount_Expended - 0xD));

	WriteData<1>((void*)0x4A3921, static_cast<char>(SoundCount_Expended - 0x11)); //sub_4A3DB0
	WriteData<1>((void*)0x4A3C61, static_cast<char>(SoundCount_Expended)); // sub_4A3B40
	WriteData<1>((void*)0x51F6B0, static_cast<char>(SoundCount_Expended - 0x9)); // sub_51F5D0
}