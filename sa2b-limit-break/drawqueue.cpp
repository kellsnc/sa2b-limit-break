#include "stdafx.h"
#include "SA2ModLoader.h"
#include "IniFile.hpp"
#include "FastFunctionHook.hpp"
#include "drawqueue.h"

// Expand draw sorting queue if needed

FastFunctionHook<void> RunDrawQueue_h(0x493A90);

DataPointer(int, ParticleCount, 0x1A5A3CC); // Current amount of sorted models
DataPointer(int, ParticleMax, 0x1A5A3D0); // Maximum amount of sorted models
UsercallFunctionPtr<void(*)(Sint32), noret, rEBX> LoadParticleCore(0x492D00);
VoidFunc(FreeParticleCore, 0x492E80);

void RunDrawQueue_r()
{
	RunDrawQueue_h.Original();
	if (ParticleMax && ParticleCount >= ParticleMax) // We've reached the maximum, expand list for next frame
	{
		int newsize = ParticleMax * 2;
		FreeParticleCore();
		LoadParticleCore(newsize);
	}
	else
	{
		ParticleCount = 0;
	}
}

void DrawQueue_Init(const IniFile* config)
{
	if (config->getBool("Limits", "DrawQueue", true))
	{
		WriteData<6>((void*)0x493C9E, 0x90); // Remove `ParticleCount = 0;`
		RunDrawQueue_h.Hook(RunDrawQueue_r);
	}
}