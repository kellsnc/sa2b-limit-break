#include "stdafx.h"

static const SoundEntry emptySoundEntry = { -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
static std::vector<SoundEntry> soundQueue(44, emptySoundEntry);
static SoundSystemPointers soundPtrs = { };
static constexpr int MaxSounds = _UI8_MAX; // for now it's limited to size of unsigned char, gotta port 20 functions to remove this limiations

void __cdecl ResetSoundQueue_r() {
	for (auto& i : soundQueue) {
        i.unk1 = -1;
        i.unk2 = -1;
        i.unk3 = -1;
        i.unk4 = 0;
        i.unk5 = 0;
        i.VolumeA = 0;
        i.VolumeB = 0;
        i.unk6 = 0;
        i.unk7 = 0;
        i.unk8 = 0;
        i.unk9 = 0;
        i.entity = 0;
        i.ID = -1;
        i.unk11 = 0;
        i.pos.x = 0.0f;
        i.pos.y = 0.0f;
        i.pos.z = 0.0f;
        i.unk12 = 0.0f;
        i.unk13 = 0.0f;
	}
}

void __cdecl FreeSoundQueueAndLevelSounds_r() {
    ResetSoundQueue_r();
    soundPtrs = { };
}

void __cdecl SetupSoundQueue_r() {
	SoundQueuePtr = soundQueue.data();
	FreeSoundQueueAndLevelSounds_r;
}

void __cdecl LoadStageSounds_r(const char* filename, void* address) {
    soundPtrs.unk5 = 0;
    soundPtrs.unk6 = 0;
    soundPtrs.unk7 = 0;
    soundPtrs.unk8 = 0;

    byte_A00648[dword_1AF0ECC] = 0;

    ResetSoundQueue_r();

    if (filename) {
        LoadMLT(filename);
        soundPtrs.unk5 = (void*)0x8ACED8;
        soundPtrs.unk8 = (void*)0x8ACF28;
        soundPtrs.unk6 = address;
    }

    byte_A00648[dword_1AF0ECC] = 1;
}

static void __declspec(naked) LoadStageSounds_asm() {
    __asm
    {
        push[esp + 04h]
        push esi
        call LoadStageSounds_r
        pop esi
        add esp, 4
        retn
    }
}

void __cdecl LoadLevelSounds2_r(const char* filename, void* address, char count) {
    soundPtrs.unk5 = 0;
    soundPtrs.unk6 = 0;
    soundPtrs.unk7 = 0;
    soundPtrs.unk8 = 0;

    byte_174AFD9 = 0;
    byte_A00648[dword_1AF0ECC] = 0;

    ResetSoundQueue_r();

    if (filename) {
        LoadMLT(filename);
        soundPtrs.unk5 = (void*)0x8ACED8;
        soundPtrs.unk8 = (void*)0x8ACF28;
        soundPtrs.unk6 = address;
    }

    if ((CurrentCharacter & 0x80u) == 0) {
        sub_459070(CurrentCharacter, address, count, soundPtrs.unk4);
        byte_A00648[dword_1AF0ECC] = 1;
    }
}

static void __declspec(naked) LoadLevelSounds2_asm() {
    __asm
    {
        push[esp + 08h]
        push[esp + 08h]
        push edi
        call LoadLevelSounds2_r

        pop edi
        add esp, 4
        add esp, 4
        retn
    }
}

void __cdecl LoadEventSounds_r(const char* filename) {
    SetWholeSoundQueueSomeFlag();

    soundPtrs.unk5 = 0;
    soundPtrs.unk6 = 0;
    soundPtrs.unk7 = 0;
    soundPtrs.unk8 = 0;

    byte_174AFD9 = 0;
    byte_A00648[dword_1AF0ECC] = 0;

    ResetSoundQueue_r();

    if (filename)
    {
        if (!strncmp(filename, "e", 1u))
        {
            LoadMLT("e_test.mlt");
            soundPtrs.unk1 = (void*)0x8AC908;
        }
        LoadMLT(filename);
        soundPtrs.unk6 = (void*)0x8A0ED0;
    }

    byte_A00648[dword_1AF0ECC] = 1;
}

void __cdecl LoadKartSounds_r(const char* filename, void* address) {
    SetWholeSoundQueueSomeFlag(); //SA2 does this two times because why not
    byte_174AFD9 = 0;
    byte_A00648[dword_1AF0ECC] = 0;
    FreeSoundQueueAndLevelSounds();

    if (filename) {
        LoadMLT(filename);

        soundPtrs.unk1 = (void*)0x8AC908;
        soundPtrs.unk2 = (void*)0x8ACA38;
        soundPtrs.unk5 = (void*)0x8ACED8;
        soundPtrs.unk6 = address;
        soundPtrs.unk8 = (void*)0x8ACF28;
    }

    byte_A00648[dword_1AF0ECC] = 1;
}

int __fastcall CheckLevelSoundListSomething_r(int ID) {
    int ID_;
    char idk;
    int* unk1;
    int unk1_0x4;

    if (ID < 0) {
        return 0;
    }

    ID_ = ID & 0xF000;

    if (ID_ > 0x4000) {
        switch (ID_) {
        case 0x6000:
            idk = 6;
            goto LABEL_21;
        case 0x7000:
            idk = 7;
            goto LABEL_21;
        case 0x8000:
            idk = 0;
            goto LABEL_21;
        }
        return 0;
    }

    if (ID_ == 0x4000) {
        idk = 4;
        goto LABEL_21;
    }
    if (ID_ > 0x2000) {
        if (ID_ == 12288) {
            idk = 3;
            goto LABEL_21;
        }
        return 0;
    }
    if (ID_ == 0x2000) {
        idk = 2;
        goto LABEL_21;
    }
    if (!(ID & 0xF000)) {
        idk = 5;
        goto LABEL_21;
    }

    if (ID_ != 4096) {
        return 0;
    }

    idk = 1;

LABEL_21:
    unk1 = (int*)(&soundPtrs.unk1 + idk);

    if (!unk1){
        return 0;
    }
    if (ID >= *unk1){
        return 0;
    }

    unk1_0x4 = *(unk1 + 4);

    if (!unk1_0x4) {
        return 0;
    }

    return unk1_0x4 + 8 * ID;
}

void UpdateSoundListSize(int size) {
    WriteData<1>((void*)0x437F03, size);
    WriteData<1>((void*)0x438AD0, size);
}

char __cdecl GetFreeSoundIndex_r(NJS_VECTOR* pos, int ID, char bank) {
    if (pos && (CheckDistance(pos, &CameraScreensInfoArray[0]->pos) > 500 ||
        CheckDistance(pos, &CameraScreensInfoArray[1]->pos) > 500)) {
        return 0;
    }

    for (int i = 0; i < soundQueue.size(); ++i) {
        if (soundQueue[i].ID < 0) {
            return i;
        }
    }

    int size = static_cast<char>(soundQueue.size());

    if (size - 16 < 250) {
        soundQueue.resize(size + 16);
        UpdateSoundListSize(soundQueue.size());
        return static_cast<char>(size);
    }

    return -1;
}
    

static void __declspec(naked) GetFreeSoundIndex_asm() {
    __asm
    {
        push[esp + 08h]
        push[esp + 08h]
        push eax
        call GetFreeSoundIndex_r
        add esp, 12
        retn
    }
}

void Sounds_Init() {
	SetupSoundQueue_r();
    WriteJump((void*)0x435F10, ResetSoundQueue_r);
    WriteJump((void*)0x435F80, FreeSoundQueueAndLevelSounds_r);
    WriteJump((void*)0x4355A0, LoadStageSounds_asm);
    WriteJump((void*)0x435630, LoadLevelSounds2_asm);
    WriteJump((void*)0x4356E0, LoadEventSounds_r);
    WriteJump((void*)0x4357C0, LoadKartSounds_r);
    WriteJump((void*)0x436180, CheckLevelSoundListSomething_r);
    WriteJump((void*)0x4370D0, GetFreeSoundIndex_asm);
}