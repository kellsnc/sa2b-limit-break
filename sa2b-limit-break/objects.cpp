#include "stdafx.h"
#include "SA2ModLoader.h"
#include "IniFile.hpp"
#include "FastFunctionHook.hpp"

#ifdef _DEBUG
static constexpr uint32_t SETCount = 4096;
static SETObjectData SetTable[SETCount]{};
#endif

void Objects_Init(const IniFile* config)
{
#ifdef _DEBUG
	if (config->getBool("Limits", "SetList", true))
	{
		WriteData(reinterpret_cast<SETObjectData**>(0x488652), &SetTable[0]); //LOADSET_1P
		WriteData(reinterpret_cast<SETObjectData**>(0x488424), &SetTable[0]); //LOADSET_2P
		WriteData(reinterpret_cast<SETObjectData**>(0x488960), &SetTable[0]);
		WriteData(reinterpret_cast<SETObjectData**>(0x488A23), &SetTable[0]);
		WriteData(reinterpret_cast<SETObjectData**>(0x4889BD), &SetTable[0]);
		WriteData(reinterpret_cast<SETObjectData**>(0x488A1F), &SetTable[0]);
		WriteData(reinterpret_cast<SETObjectData**>(0x488AFA), &SetTable[0]);
		WriteData(reinterpret_cast<SETObjectData**>(0x488B61), &SetTable[0]);
		WriteData(reinterpret_cast<SETObjectData**>(0x488BFE), &SetTable[0]);
		WriteData(reinterpret_cast<SETObjectData**>(0x4C7B7E), &SetTable[0]);

		WriteData(reinterpret_cast<uint32_t*>(0x48894D), SETCount);
		WriteData(reinterpret_cast<uint32_t*>(0x488952), SETCount);
		WriteData(reinterpret_cast<uint32_t*>(0x488BEC), SETCount);
		WriteData(reinterpret_cast<uint32_t*>(0x488BF3), SETCount);
	}
#endif
}