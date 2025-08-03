#include "stdafx.h"
#include "SA2ModLoader.h"
#include "IniFile.hpp"
#include "FastFunctionHook.hpp"
#include "clip.h"

static Uint32 ClipDistanceMultiplier = 70;
static Uint32 LandClipDistanceMultiplier = 5;
static bool RemoveChunks = false;

FastFunctionHook<void> ListGroundForDrawing_h(0x47CAE0);
FastUsercallHookPtr<Bool(*)(NJS_VECTOR*, Float, Float, Float, Float), rEAX, rECX, stack4, stack4, stack4, stack4> CheckRangeXYZRP_h(0x488340);
FastUsercallHookPtr<Bool(*)(NJS_VECTOR*, NJS_VECTOR*, Float, Float, Float, Float), rEAX, rECX, stack4, stack4, stack4, stack4> CheckRange2PXYZRP_h(0x4881F0);

UsercallFunctionPtr<int(*)(int fallback, float x, float z), rEAX, rECX, stack4, stack4> GetBlockbitFromMap(0x47C8D0);

DataArray(int, MaskBlock, 0x174B060, 2);
DataArray(int, DisplayBlock, 0x174B068, 2);
DataPointer(int, numDisplayEntry, 0x1945A00);
DataPointer(COL**, pDisplayEntry, 0x1A5A2E4);

#define SHORT_ANG(ang) ((ang) & 0xFFFF)
#define ROTATEX(m, ang) if (ang != 0) njRotateX(m, SHORT_ANG(ang));
#define ROTATEY(m, ang) if (ang != 0) njRotateY(m, SHORT_ANG(ang));
#define ROTATEZ(m, ang) if (ang != 0) njRotateZ(m, SHORT_ANG(ang));

Bool __cdecl CheckRangeXYZRP_r(NJS_VECTOR* from, Float x, Float y, Float z, Float dist)
{
	return CheckRangeXYZRP_h.Original(from, x, y, z, dist * ClipDistanceMultiplier);
}

Bool __cdecl CheckRange2PXYZRP_r(NJS_VECTOR* from, NJS_VECTOR* p2pos, Float x, Float y, Float z, Float dist)
{
	return CheckRange2PXYZRP_h.Original(from, p2pos, x, y, z, dist * ClipDistanceMultiplier);
}

void __cdecl ListGroundForDrawing_r()
{
	auto land = CurrentLandTable;
	int screen = CurrentScreen;
	int maskblock = MaskBlock[screen ? 1 : 0];

	float clippingDistance = CurrentLandTable->ClippingDistance * LandClipDistanceMultiplier;
	ChunkLandTable.ClippingDistance = clippingDistance;

	auto camloc = pCameraLocations[screen];

	NJS_VECTOR v = { 0.0f, 0.0f, -0.5f * clippingDistance };
	njPushMatrix(_nj_unit_matrix_);
	ROTATEY(_nj_current_matrix_ptr_, camloc->ang.y);
	ROTATEX(_nj_current_matrix_ptr_, camloc->ang.x);
	njCalcPoint(_nj_current_matrix_ptr_, &v, &v, 0);
	njPopMatrix(1);

	NJS_VECTOR center;
	center.x = camloc->pos.x + v.x;
	center.y = camloc->pos.y + v.y;
	center.z = camloc->pos.z + v.z;

	int displayblock = RemoveChunks ? 0xFFFFFFFF : (GetBlockbitFromMap(maskblock, camloc->pos.x, camloc->pos.z) & maskblock);
	DisplayBlock[screen ? 1 : 0] = displayblock;

	int count = 0;
	if (CurrentLandTable->VisibleModelCount < 0)
	{
		count = CurrentLandTable->COLCount;
	}
	else
	{
		count = CurrentLandTable->VisibleModelCount;
	}

	int numEntry = 0;

	for (int i = 0; i < count; ++i)
	{
		auto col = &CurrentLandTable->COLList[i];
		if (col->Chunks == 0 || (col->Chunks & displayblock))
		{
			if (col->Flags & SurfaceFlag_Visible)
			{
				float dist;
				if (col->Flags & 0x800000)
				{
					dist = MaxDrawDistance + col->Radius;
				}
				else
				{
					dist = clippingDistance * 0.5f + col->Radius;
				}

				Float x = center.x - col->Center.x;
				Float y = center.y - col->Center.y;
				Float z = center.z - col->Center.z;

				if (x * x + y * y + z * z <= dist * dist)
				{
					pDisplayEntry[numEntry++] = col;
					if (numEntry >= 1024)
						break;
				}
			}
		}
	}

	numDisplayEntry = numEntry;
}

void Clip_Init(const IniFile* config)
{
	if (config->getBool("Clip", "ClipDist", true))
	{
		ClipDistanceMultiplier = config->getInt("Clip", "ClipMultiplier", ClipDistanceMultiplier);
		CheckRangeXYZRP_h.Hook(CheckRangeXYZRP_r);
		CheckRange2PXYZRP_h.Hook(CheckRange2PXYZRP_r);
	}
	
	if (config->getBool("Clip", "LandDist", true))
	{
		LandClipDistanceMultiplier = config->getInt("Clip", "LandMultiplier", LandClipDistanceMultiplier);
		RemoveChunks = config->getBool("Clip", "RemoveChunks", RemoveChunks);
		WriteJump(ListGroundForDrawing, ListGroundForDrawing_r);
	}
}