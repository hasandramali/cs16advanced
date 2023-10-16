/*
*
*    This program is free software; you can redistribute it and/or modify it
*    under the terms of the GNU General Public License as published by the
*    Free Software Foundation; either version 2 of the License, or (at
*    your option) any later version.
*
*    This program is distributed in the hope that it will be useful, but
*    WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software Foundation,
*    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*    In addition, as a special exception, the author gives permission to
*    link the code of this program with the Half-Life Game Engine ("HL
*    Engine") and Modified Game Libraries ("MODs") developed by Valve,
*    L.L.C ("Valve").  You must obey the GNU General Public License in all
*    respects for all of the code used other than the HL Engine and MODs
*    from Valve.  If you modify this file, you may extend this exception
*    to your version of the file, but you are not obligated to do so.  If
*    you do not wish to do so, delete this exception statement from your
*    version.
*
*/
#include "events.h"

enum kriss_e
{
	KRISS_IDLE,
	KRISS_SHOOT1,
	KRISS_SHOOT2,
	KRISS_SHOOT3,
	KRISS_RELOAD,
	KRISS_DRAW,
	KRISS_ATTACH_SILENCER,
	KRISS_UNSIL_IDLE,
	KRISS_UNSIL_SHOOT1,
	KRISS_UNSIL_SHOOT2,
	KRISS_UNSIL_SHOOT3,
	KRISS_UNSIL_RELOAD,
	KRISS_UNSIL_DRAW,
	KRISS_DETACH_SILENCER
};

static const char *SOUNDS_NAME[] =
{
	"weapons/kriss-1.wav",
	"weapons/kriss_sil-1.wav"
};

void EV_FireKRISS(event_args_s *args)
{
	vec3_t ShellVelocity;
	vec3_t ShellOrigin;
	vec3_t vecSrc, vecAiming;
	int    sequence, idx = args->entindex;
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
		if (args->bparam1)
		{
			sequence = Com_RandomLong(KRISS_SHOOT1, KRISS_SHOOT3);
		}
		else
		{
			sequence = Com_RandomLong(KRISS_UNSIL_SHOOT1, KRISS_UNSIL_SHOOT3);
		}
		gEngfuncs.pEventAPI->EV_WeaponAnimation(sequence, 2);
		if( !gHUD.cl_righthand->value )
		{
			EV_GetDefaultShellInfo( args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 34.0, -10.0, -11.0, 0);
		}
		else
		{
			EV_GetDefaultShellInfo( args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 34.0, -10.0, 11.0, 0);
		}
	}
	else
	{
		EV_GetDefaultShellInfo( args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 20.0, -12.0, 4.0, 0);
	}

	EV_EjectBrass(ShellOrigin, ShellVelocity, angles[ YAW ], g_iPShell, TE_BOUNCE_SHELL);
	PLAY_EVENT_SOUND(args->bparam1 ? SOUNDS_NAME[1] : SOUNDS_NAME[0]);
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
