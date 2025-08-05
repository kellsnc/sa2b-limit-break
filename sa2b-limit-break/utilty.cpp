#include "stdafx.h"
#include "SA2ModLoader.h"

// Check if a task is within vanilla clip distance
Bool CheckRangeWithR(task* tp, Float fRange)
{
	if (tp->ocp && (tp->ocp->ssCondition & 8))
	{
		return 0;
	}

	if (fRange == 0.0f)
	{
		return 0;
	}

	taskwk* twp = tp->Data1.twp;

	if (!*(int*)0x174AFC0 && CameraData1)
	{
		Float x = CameraData1->Position.x - twp->pos.x;
		Float y = CameraData1->Position.y - twp->pos.y;
		Float z = CameraData1->Position.z - twp->pos.z;

		if (x * x + y * y + z * z < fRange)
		{
			return 0;
		}
	}

	for (int i = 0; i < 2; ++i)
	{
		if (playertwp[i])
		{
			Float x = playertwp[i]->pos.x - twp->pos.x;
			Float y = playertwp[i]->pos.y - twp->pos.y;
			Float z = playertwp[i]->pos.z - twp->pos.z;

			if (x * x + y * y + z * z < fRange)
			{
				return 0;
			}
		}
	}

	return 1;
}

// Check if a task is within vanilla clip distance
Bool CheckRange(task* tp)
{
	return CheckRangeWithR(tp, (tp->ocp ? tp->ocp->fRangeOut : *(float*)0x905BA0));
}