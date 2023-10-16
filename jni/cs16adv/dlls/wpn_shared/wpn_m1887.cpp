/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"
#include "wpn_m1887.h"

enum m1887_e
{
	M1887_IDLE,
	M1887_FIRE1,
	M1887_FIRE2,
	M1887_FIRE3,
	M1887_FIRE4,
	M1887_FIRE5,
	M1887_RELOAD,
	M1887_PUMP,
	M1887_START_RELOAD,
	M1887_DRAW
};

LINK_ENTITY_TO_CLASS(weapon_m1887, CM1887)

const int M1887_MAX_CLIP = 8;

void CM1887::Spawn(void)
{
	pev->classname = MAKE_STRING("weapon_m1887");

	Precache();
	m_iId = WEAPON_M3;
	SET_MODEL(ENT(pev), "models/w_m1887.mdl");
	m_iDefaultAmmo = M1887_MAX_CLIP;

	FallInit();
}

void CM1887::Precache(void)
{
	PRECACHE_MODEL("models/v_m1887.mdl");
	PRECACHE_MODEL("models/p_m1887.mdl");
	PRECACHE_MODEL("models/w_m1887.mdl");

	m_iShellId = m_iShell = PRECACHE_MODEL("models/shotgunshell.mdl");

	PRECACHE_SOUND("weapons/m1887-1.wav");
	PRECACHE_SOUND("weapons/m1887_insert.wav");
	PRECACHE_SOUND("weapons/m1887_reafter.wav");
	PRECACHE_SOUND("weapons/m1887_restart.wav");
	PRECACHE_SOUND("weapons/m1887_draw.wav");

	m_usFireM1887 = PRECACHE_EVENT(1, "events/m1887.sc");
}

int CM1887::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "buckshot";
	p->iMaxAmmo1 = MAX_AMMO_BUCKSHOT;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = M1887_MAX_CLIP;
	p->iSlot = 0;
	p->iPosition = 5;
	p->iId = m_iId = WEAPON_M3;
	p->iFlags = 0;
	p->iWeight = M3_WEIGHT;

	return 1;
}

BOOL CM1887::Deploy(void)
{
	return DefaultDeploy("models/v_m1887.mdl", "models/p_m1887.mdl", M1887_DRAW, "shotgun", UseDecrement() != FALSE);
}

void CM1887::PrimaryAttack(void)
{
	if (m_pPlayer->pev->waterlevel == 3)
	{
		PlayEmptySound();
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.15;
		return;
	}

	if (m_iClip <= 0)
	{
		Reload();

		if (m_iClip == 0)
			PlayEmptySound();

		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 1.0;
		return;
	}

	m_pPlayer->m_iWeaponVolume = LOUD_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = BRIGHT_GUN_FLASH;

	m_iClip--;
	m_pPlayer->pev->effects |= EF_MUZZLEFLASH;
#ifndef CLIENT_DLL
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);
#endif

	UTIL_MakeVectors(m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle);
#ifndef CLIENT_DLL
	m_pPlayer->FireBullets(8, m_pPlayer->GetGunPosition(), gpGlobals->v_forward, Vector(0.0675, 0.0675, 0), 3000, BULLET_PLAYER_BUCKSHOT, 0);
#endif

	int flags;
#ifdef CLIENT_WEAPONS
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	PLAYBACK_EVENT_FULL(flags, ENT(m_pPlayer->pev), m_usFireM1887, 0, (float *)&g_vecZero, (float *)&g_vecZero, 0, 0, 0, 0, FALSE, FALSE);

	if (m_iClip)
		m_flPumpTime = UTIL_WeaponTimeBase() + 0.5;

#ifndef CLIENT_DLL
	if (!m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
#endif
	if (m_iClip)
		m_flPumpTime = UTIL_WeaponTimeBase() + 0.5;

	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.7;
	m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.7;

	if (m_iClip)
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 2.5;
	else
		m_flTimeWeaponIdle = 0.7;

	m_fInSpecialReload = 0;

	if (m_pPlayer->pev->flags & FL_ONGROUND)
		m_pPlayer->pev->punchangle.x -= UTIL_SharedRandomLong(m_pPlayer->random_seed + 1, 6, 10);
	else
		m_pPlayer->pev->punchangle.x -= UTIL_SharedRandomLong(m_pPlayer->random_seed + 1, 10, 14);

	m_pPlayer->m_flEjectBrass = gpGlobals->time + 0.45;
}

void CM1887::Reload(void)
{
	if (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 || m_iClip == M1887_MAX_CLIP)
		return;

	if (m_flNextPrimaryAttack > UTIL_WeaponTimeBase())
		return;

	if (!m_fInSpecialReload)
	{
#ifndef CLIENT_DLL
		m_pPlayer->SetAnimation(PLAYER_RELOAD);
#endif
		SendWeaponAnim(M1887_START_RELOAD, UseDecrement() != FALSE);

		m_fInSpecialReload = 1;
		m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.55;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.55;
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.55;
		m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.55;
	}
	else if (m_fInSpecialReload == 1)
	{
		if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
			return;

		m_fInSpecialReload = 2;
		SendWeaponAnim(M1887_RELOAD, UseDecrement() != FALSE);

		m_flNextReload = UTIL_WeaponTimeBase() + 0.45;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.45;
	}
	else
	{
		m_iClip++;
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;
		m_fInSpecialReload = 1;
		m_pPlayer->ammo_buckshot--;
	}
}

void CM1887::WeaponIdle(void)
{
	ResetEmptySound();
	m_pPlayer->GetAutoaimVector(AUTOAIM_5DEGREES);

	if (m_flPumpTime && m_flPumpTime < UTIL_WeaponTimeBase())
		m_flPumpTime = 0;

	if (m_flTimeWeaponIdle < UTIL_WeaponTimeBase())
	{
		if (m_iClip == 0 && m_fInSpecialReload == 0 && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType])
		{
			Reload();
		}
		else if (m_fInSpecialReload != 0)
		{
			if (m_iClip != M1887_MAX_CLIP && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType])
			{
				Reload();
			}
			else
			{
				SendWeaponAnim(M1887_PUMP, UseDecrement() != FALSE);

				m_fInSpecialReload = 0;
				m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.5;
			}
		}
		else
			SendWeaponAnim(M1887_IDLE, UseDecrement() != FALSE);
	}
}
