/*
event_infinitys.cpp
Copyright (C) 2019 Moemod Yanase

This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
*/

#include "events.h"

enum
{
	ANIM_IDLE1,
	ANIM_SHOOT1,
	ANIM_SHOOT2,
	ANIM_SHOOT_EMPTY,
	ANIM_RELOAD,
	ANIM_DRAW
};

static const char *SOUNDS_NAME = "weapons/infi-1.wav";

void EV_FireInfinityS(event_args_t *args)
{
	vec3_t ShellVelocity;
	vec3_t ShellOrigin;
	vec3_t vecSrc, vecAiming;

	int idx = args->entindex;
	Vector origin( args->origin );
	Vector angles(
		args->iparam1 / 100.0f + args->angles[0],
		args->iparam2 / 100.0f + args->angles[1],
		args->angles[2]
		);
	Vector velocity( args->velocity );
	Vector forward, right, up;

	AngleVectors( angles, forward, right, up );

	if ( EV_IsLocal( idx ) )
	{
		++g_iShotsFired;
		EV_MuzzleFlash();
		if( args->bparam1 )
		{
			gEngfuncs.pEventAPI->EV_WeaponAnimation(Com_RandomLong(ANIM_SHOOT1, ANIM_SHOOT2), 2);
		}
		else
		{
			gEngfuncs.pEventAPI->EV_WeaponAnimation(ANIM_SHOOT_EMPTY, 2);
		}

		if( !gHUD.cl_righthand->value )
		{
			EV_GetDefaultShellInfo( args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 36.0, -14.0, -14.0, 0);
		}
		else
		{
			EV_GetDefaultShellInfo( args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 36.0, -14.0, 14.0, 0);
		}
	}
	else
	{
		EV_GetDefaultShellInfo( args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 20.0, -12.0, 4.0, 0);
	}

	EV_EjectBrass(ShellOrigin, ShellVelocity, angles[ YAW ], g_iPShell, TE_BOUNCE_SHELL);

	PLAY_EVENT_SOUND( SOUNDS_NAME );

	EV_GetGunPosition( args, vecSrc, origin );
	VectorCopy( forward, vecAiming );
	Vector vSpread;
	
	vSpread.x = args->fparam1;
	vSpread.y = args->fparam2;
	EV_HLDM_FireBullets( idx,
		forward, right,	up,
		1, vecSrc, vecAiming,
		vSpread, 8192.0, BULLET_PLAYER_45ACP,
		2 );
}
