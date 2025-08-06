#include "stdafx.h"
#include <deque>
#include <algorithm>
#include "SA2ModLoader.h"
#include "IniFile.hpp"
#include "FastFunctionHook.hpp"
#include "dynacol.h"

//FastFunctionHook<void> MakeLandCollLandEntryRangeIn_h(0x47D160);

DataPointer(CL_ObjInfo*, ds_link_b_class, 0x1A5A400);

struct UsedObject
{
	bool used;
	NJS_OBJECT* object;
};

std::deque<UsedObject> object_array;

struct _OBJ_LANDCOLL
{
	int slAttribute;
	obj* pObject;
	task* pTask;
};

std::vector<_OBJ_LANDCOLL> mobile_list;

NJS_OBJECT* GetMobileLandObject_r()
{
	const auto pred = [](const UsedObject& e) -> bool
	{
		return e.used == false;
	};

	const auto it = std::find_if(object_array.begin(), object_array.end(), pred);

	if (it != object_array.end())
	{
		it->used = true;
		*it->object = {};
		return it->object;
	}
	else
	{
		const auto object = new NJS_OBJECT{};
		const UsedObject element
		{
			true,
			object
		};
		object_array.push_back(element);
		return object;
	}
}

void ReleaseMobileLandObject_r(NJS_OBJECT* pObjLandObject)
{
	for (auto& element : object_array)
	{
		if (element.object == pObjLandObject)
		{
			element.used = false;
		}
	}
}

void RegisterCollisionEntry_r(int slAttribute, task* pTask, NJS_OBJECT* pObject)
{
	if ((pObject->ang[0] | pObject->ang[1] | pObject->ang[2]) & 0xFFF0)
	{
		slAttribute |= 0x10000000;
	}

	const _OBJ_LANDCOLL element = {
		slAttribute,
		pObject,
		pTask
	};

	mobile_list.push_back(element);

	pTask->Data1.twp->flag |= 0x100;

	MobileColList = (DynColInfo*)mobile_list.data();
	MobileColList_Count = mobile_list.size();
}

void RemoveObjInfo(NJS_OBJECT* pObject)
{
	CL_ObjInfo* pInfo = ds_link_b_class;
	while (pInfo)
	{
		if (pInfo->objptr == pObject)
		{
			pInfo->objptr = NULL;
			return;
		}
		pInfo = pInfo->next;
	}
}

void WithdrawCollisionEntry_r(task* pTask, NJS_OBJECT* pObject)
{
	for (auto it = mobile_list.begin(); it != mobile_list.end(); ++it)
	{
		if (it->pTask == pTask)
		{
			RemoveObjInfo(pObject);
			mobile_list.erase(it);
			MobileColList = (DynColInfo*)mobile_list.data();
			MobileColList_Count = mobile_list.size();
			break;
		}
	}
}

void Dynacol_Init(const IniFile* config)
{
	if (config->getBool("Limits", "Dynacol", true))
	{
		// Mobile:
		WriteData((uint8_t*)0x47BBEF, (uint8_t)0xEB); // load MobileColList
		WriteData((uint8_t*)0x47BA8C, (uint8_t)0xEB); // free MobileColList
		WriteData((uint8_t*)0x47BC54, (uint8_t)0xEB); // alloc objMobileLandObject
		WriteData((uint8_t*)0x47BAED, (uint8_t)0xEB); // free objMobileLandObject
		WriteJump((void*)0x47D7F0, GetMobileLandObject_r);
		GenerateUsercallHookC(ReleaseMobileLandObject_r, rEAX, 0x47D840, 1, rESI);
		GenerateUsercallHookC(RegisterCollisionEntry_r, noret, 0x47D6B0, 3, rEAX, rEDX, rESI);
		GenerateUsercallHookC(WithdrawCollisionEntry_r, noret, 0x47D760, 2, rEDI, rEBX);
		mobile_list.reserve(256);
		MobileColList = (DynColInfo*)mobile_list.data();

		// Todo: land
	}
}