#include "stdafx.h"

const uint32_t SETCount = 4096;
static SETObjectData SetTable[SETCount]{};

void Objects_Init() {
	WriteData((SETObjectData**)0x488652, &SetTable[0]); //LOADSET_1P
	WriteData((SETObjectData**)0x488424, &SetTable[0]); //LOADSET_2P
	WriteData((SETObjectData**)0x488960, &SetTable[0]);
	WriteData((SETObjectData**)0x488A23, &SetTable[0]);
	WriteData((SETObjectData**)0x4889BD, &SetTable[0]);
	WriteData((SETObjectData**)0x488A1F, &SetTable[0]);
	WriteData((SETObjectData**)0x488AFA, &SetTable[0]);
	WriteData((SETObjectData**)0x488B61, &SetTable[0]);
	WriteData((SETObjectData**)0x488BFE, &SetTable[0]);
	WriteData((SETObjectData**)0x4C7B7E, &SetTable[0]);

	WriteData((uint32_t*)0x48894D, SETCount);
	WriteData((uint32_t*)0x488952, SETCount);
}

void Objects_OnFrame() {

}