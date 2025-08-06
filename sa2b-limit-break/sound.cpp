#include "stdafx.h"
#include "SA2ModLoader.h"
#include "IniFile.hpp"
#include "FastFunctionHook.hpp"
#include "sound.h"

// Series of changes to save space in the sound queue
// - Spatial timer sounds are ignored until close enough
// - Spatial loop sounds are buffered seperatly and played only when close enough
// - The queue is expanded to the full range for every sound

typedef struct _SEcallbuf
{
	Sint8 banknum; // Bank ID
	Sint8 soundnum; // Sound ID
	Sint8 pri;
	Sint8 angle; // Panning
	Sint8 vol3D;
	Sint8 vol;
	Sint8 volmax;
	Sint8 unknown_flags;
	Sint16 flags;
	Sint16 dist;
	Sint16 timer;
	Sint16 timer2; // Restart timer for some dsPlay functions (flag 0x4000)
	Sint32 id; // Used to identify the sound. Can be a task pointer (if flag 0x1000 is set, sound is auto removed when task is deleted.)
	Sint32 tone; // Bank and sound IDs (0xBSSS)
	NJS_POINT3* pos_ptr;
	NJS_POINT3 pos;
	Float fdistmax;
	Float fdist; // Distance from camera before conversion to Sint16 (dist)
} SEcallbuf;

UsercallFunctionPtr<MLTSoundEntry* (*)(int), rEAX, rECX> GetMLTSoundEntry(0x436180);

FastUsercallHookPtr<Sint8(*)(Sint32 tone, NJS_POINT3* pos, Sint32 id, Sint8 pri, Sint8 volume, Sint16 timer), rAL, rEDI, rESI, rEBX, stack4, stack4, stack4> dsPlay_timer_v_437590_h(0x437590);
FastUsercallHookPtr<Sint8(*)(Sint32 tone, NJS_POINT3* pos, Sint32 id, Sint8 pri, Sint8 volume, Sint16 timer), rAL, rEDI, rESI, rEBX, stack4, stack4, stack4> dsPlay_timer_v_437710_h(0x437710);
FastUsercallHookPtr<Sint8(*)(task* tp, Sint32 tone, Sint8 volume, Sint16 timer), rAL, rEAX, rECX, stack4, stack4> dsPlay_timer_tp_437BB0_h(0x437BB0);
FastUsercallHookPtr<Sint8(*)(Sint32 tone, NJS_POINT3* pos, Sint32 id, Sint8 volume, Sint16 timer), rAL, rEDI, rESI, stack4, stack4, stack4> dsPlay_timer_v_437D10_h(0x437D10);
FastUsercallHookPtr<Sint8(*)(Sint32 tone, NJS_POINT3* pos, Sint32 id, Sint8 pri, Sint8 volume), rAL, rEDI, rESI, stack4, stack4, stack4> dsPlay_oneshsot_v_h(0x4372E0);

FastUsercallHookPtr<Sint8(*)(Sint32 tone, NJS_VECTOR* pos, Sint32 id, Sint8 volume), rAL, rEDI, rESI, rEBX, stack4> dsPlay_iloop_v_h(0x437420);
FastUsercallHookPtr<Sint8(*)(task* tp, Sint32 tone, Sint8 volume), rAL, rEAX, rECX, stack4> dsPlay_iloop_tp_h(0x437A30);
FastUsercallHookPtr<void(*)(Sint32 id), noret, rEDX> dsStop_num_h(0x437DD0);
FastUsercallHookPtr<void(*)(Sint32 id), noret, rEDX> dsStop_id_h(0x437E10);
FastUsercallHookPtr<void(*)(Sint32 id, Sint32 tone), noret, rEDX, rESI> dsStop_tone_id_h(0x437E50);
FastFunctionHook<void> dsStop_all_h(0x437E90);
FastFunctionHook<void> dsSoundServer_h(0x438750);

bool CheckSoundDistance(Sint32 id, NJS_VECTOR* position);
bool CheckSoundExists(Sint32 id, Sint32 tone);

struct iloop_v_data
{
	Sint32 tone;
	NJS_VECTOR pos;
	Sint32 id;
	Sint8 volume;

	int soundid = -1;

	void Exec()
	{
		if (CheckSoundDistance(tone, &pos))
		{
			if (soundid == -1 && !CheckSoundExists(id, tone))
			{
				soundid = dsPlay_iloop_v_h.Original(tone, &pos, id, volume);
			}
		}
		else
		{
			if (soundid != -1)
			{
				dsStop_tone_id_h.Original(id, tone);
				soundid = -1;
			}
		}
	}
};

struct iloop_tp_data
{
	task* tp;
	Sint32 tone;
	Sint8 volume;

	int soundid = -1;

	void Exec()
	{
		if (tp && tp->Data1.twp && CheckSoundDistance(tone, &tp->Data1.twp->pos) && !CheckSoundExists((Sint32)tp, tone))
		{
			if (soundid == -1)
			{
				soundid = dsPlay_iloop_tp_h.Original(tp, tone, volume);
			}
		}
		else
		{
			if (soundid != -1)
			{
				dsStop_tone_id_h.Original((Sint32)tp, tone);
				soundid = -1;
			}
		}
	}
};

std::list<iloop_v_data> iloop_v_queue;
std::list<iloop_tp_data> iloop_tp_queue;

bool CheckSoundDistance(Sint32 id, NJS_VECTOR* position)
{
	auto entry = GetMLTSoundEntry(id);

	const float radius = entry && entry->DefaultDistance != 0.0f ? entry->DefaultDistance : 600.0f;
	
	for (int i = 0; i < 2; ++i)
	{
		if (pCameraLocations[i])
		{
			Float x = pCameraLocations[i]->pos.x - position->x;
			Float y = pCameraLocations[i]->pos.y - position->y;
			Float z = pCameraLocations[i]->pos.z - position->z;

			if (x * x + y * y + z * z < radius * radius)
			{
				return true;
			}
		}
	}
	return false;
}

bool CheckSoundExists(Sint32 id, Sint32 tone)
{
	for (int i = 0; i < 43; ++i)
	{
		auto soundentry = (SEcallbuf*)&SoundSystemPtr->SoundEntries[i];
		if (soundentry->id == id && soundentry->tone == tone)
		{
			return true;
		}
	}
	return false;
}

Sint8 dsPlay_timer_v_437590_r(Sint32 tone, NJS_POINT3* pos, Sint32 id, Sint8 pri, Sint8 volume, Sint16 timer)
{
	if (CheckSoundDistance(tone, pos))
	{
		return dsPlay_timer_v_437590_h.Original(tone, pos, id, pri, volume, timer);
	}
	return -1;
}

Sint8 dsPlay_timer_v_437710_r(Sint32 tone, NJS_POINT3* pos, Sint32 id, Sint8 pri, Sint8 volume, Sint16 timer)
{
	if (CheckSoundDistance(tone, pos))
	{
		return dsPlay_timer_v_437710_h.Original(tone, pos, id, pri, volume, timer);
	}
	return -1;
}

Sint8 dsPlay_timer_tp_437BB0_r(task* tp, Sint32 tone, Sint8 volume, Sint16 timer)
{
	if (tp && tp->Data1.twp && CheckSoundDistance(tone, &tp->Data1.twp->pos))
	{
		return dsPlay_timer_tp_437BB0_h.Original(tp, tone, volume, timer);
	}
	return -1;
}

Sint8 dsPlay_timer_v_437D10_r(Sint32 tone, NJS_POINT3* pos, Sint32 id, Sint8 volume, Sint16 timer)
{
	if (CheckSoundDistance(tone, pos))
	{
		return dsPlay_timer_v_437D10_h.Original(tone, pos, id, volume, timer);
	}
	return -1;
}

Sint8 dsPlay_oneshsot_v_r(Sint32 tone, NJS_POINT3* pos, Sint32 id, Sint8 pri, Sint8 volume)
{
	if (CheckSoundDistance(tone, pos))
	{
		return dsPlay_oneshsot_v_h.Original(tone, pos, id, pri, volume);
	}
	return -1;
}

Sint8 dsPlay_iloop_v_r(Sint32 tone, NJS_VECTOR* pos, Sint32 id, Sint8 volume)
{
	for (auto& item : iloop_v_queue)
	{
		if (item.id == id && item.tone == tone)
		{
			return -1;
		}
	}
	iloop_v_queue.push_back({ tone, *pos, id, volume });
	return -1;
}

Sint8 dsPlay_iloop_tp_r(task* tp, Sint32 tone, Sint8 volume)
{
	for (auto& item : iloop_v_queue)
	{
		if (item.id == (Sint32)tp && item.tone == tone)
		{
			return -1;
		}
	}
	iloop_tp_queue.push_back({ tp, tone, volume });
	return -1;
}

void dsStop_num_r(Sint32 tone)
{
	iloop_v_queue.remove_if([tone](const iloop_v_data& item) {
		return item.tone == tone;
		});
	iloop_tp_queue.remove_if([tone](const iloop_tp_data& item) {
		return item.tone == tone;
		});
	dsStop_num_h.Original(tone);
}

void dsStop_id_r(Sint32 id)
{
	iloop_v_queue.remove_if([id](const iloop_v_data& item) {
		return item.id == id;
		});
	iloop_tp_queue.remove_if([id](const iloop_tp_data& item) {
		return (Sint32)item.tp == id;
		});
	dsStop_id_h.Original(id);
}

void dsStop_tone_id_r(Sint32 id, Sint32 tone)
{
	iloop_v_queue.remove_if([id, tone](const iloop_v_data& item) {
		return item.id == id && item.tone == tone;
		});
	iloop_tp_queue.remove_if([id, tone](const iloop_tp_data& item) {
		return (Sint32)item.tp == id && item.tone == tone;
		});
	dsStop_tone_id_h.Original(id, tone);
}

void dsStop_all_r()
{
	iloop_v_queue.clear();
	iloop_tp_queue.clear();
	dsStop_all_h.Original();
}

void dsSoundServer_r()
{
	for (auto& item : iloop_v_queue)
	{
		item.Exec();
	}

	for (auto& item : iloop_tp_queue)
	{
		item.Exec();
	}

	dsSoundServer_h.Original();
}

void Sound_Init(const IniFile* config)
{
	if (config->getBool("Limits", "Sounds", true))
	{
		// timer calls (continuous play until the function stops getting called)
		dsPlay_timer_v_437590_h.Hook(dsPlay_timer_v_437590_r);
		dsPlay_timer_v_437710_h.Hook(dsPlay_timer_v_437710_r);
		dsPlay_timer_tp_437BB0_h.Hook(dsPlay_timer_tp_437BB0_r);
		dsPlay_timer_v_437D10_h.Hook(dsPlay_timer_v_437D10_r);

		// oneshot calls (played once, stays 120 frames in queue)
		dsPlay_oneshsot_v_h.Hook(dsPlay_oneshsot_v_r);

		// iloop calls (continuous play, call dsStop to stop)
		dsPlay_iloop_v_h.Hook(dsPlay_iloop_v_r);
		dsPlay_iloop_tp_h.Hook(dsPlay_iloop_tp_r);
		dsStop_num_h.Hook(dsStop_num_r);
		dsStop_id_h.Hook(dsStop_id_r);
		dsStop_tone_id_h.Hook(dsStop_tone_id_r);
		dsStop_all_h.Hook(dsStop_all_r);
		dsSoundServer_h.Hook(dsSoundServer_r);

		// The game uses 3 regions of the sound queue (0-31 32-37 38-42)
		// The code below remove these regions.
		WriteData((int16_t*)0x436B20, (int16_t)0xE990);
		WriteData((int8_t*)0x436BFA, (int8_t)0xEB);
		WriteData((int32_t*)0x436C8F, 43);
		WriteData((int8_t*)0x436CC4, (int8_t)43);
		WriteData((int8_t*)0x436CE7, (int8_t)43);
		WriteData((int8_t*)0x4387CB, (int8_t)43);
	}
}