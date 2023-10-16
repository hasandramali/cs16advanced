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
#include "wpn_mp7a1d.h"

enum elite_e
{
	ELITE_IDLE,
	ELITE_IDLE_LEFTEMPTY,
	ELITE_SHOOTLEFT1,
	ELITE_SHOOTLEFT2,
	ELITE_SHOOTLEFT3,
	ELITE_SHOOTLEFT4,
	ELITE_SHOOTLEFT5,
	ELITE_SHOOTLEFTLAST,
	ELITE_SHOOTRIGHT1,
	ELITE_SHOOTRIGHT2,
	ELITE_SHOOTRIGHT3,
	ELITE_SHOOTRIGHT4,
	ELITE_SHOOTRIGHT5,
	ELITE_SHOOTRIGHTLAST,
	ELITE_RELOAD,
	ELITE_DRAW
};

LINK_ENTITY_TO_CLASS(weapon_mp7a1d, CMP7A1D)

static const int WEAPON_MAXCLIP = 80;

void CMP7A1D::Spawn(void)
{
	pev->classname = MAKE_STRING("weapon_mp7a1d");

	Precache();
	m_iId = WEAPON_DEAGLE;
	SET_MODEL(ENT(pev), "models/w_dmp7a1.mdl");

	m_iDefaultAmmo = WEAPON_MAXCLIP;
	m_flAccuracy = 0.0;

	FallInit();
}

void CMP7A1D::Precache(void)
{
	PRECACHE_MODEL("models/v_dmp7a1.mdl");
	PRECACHE_MODEL("models/p_dmp7a1.mdl");
	PRECACHE_MODEL("models/w_dmp7a1.mdl");

	PRECACHE_SOUND("weapons/dde-1.wav");

	m_iShell = PRECACHE_MODEL("models/pshell.mdl");
	m_usFireELITE_LEFT = PRECACHE_EVENT(1, "events/mp7a1d_left.sc");
	m_usFireELITE_RIGHT = PRECACHE_EVENT(1, "events/mp7a1d_right.sc");
}

int CMP7A1D::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "46MM";
	p->iMaxAmmo1 = MAX_AMMO_46MM;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_MAXCLIP;
	p->iSlot = 0;
	p->iPosition = 8;
	p->iId = m_iId = WEAPON_P90;
	p->iFlags = 0;
	p->iWeight = P90_WEIGHT;

	return 1;
}

BOOL CMP7A1D::Deploy(void)
{
	m_flAccuracy = 0.0;

	if (!(m_iClip & 1))
		m_iWeaponState |= WPNSTATE_ELITE_LEFT;

	return DefaultDeploy("models/v_dmp7a1.mdl", "models/p_dmp7a1.mdl", ELITE_DRAW, "dualpistols", UseDecrement() != FALSE);
}

void CMP7A1D::PrimaryAttack(void)
{
	if (!FBitSet(m_pPlayer->pev->flags, FL_ONGROUND))
		ELITEFire((0.375) * m_flAccuracy, 0.07, FALSE);
	else
		ELITEFire((0.035) * m_flAccuracy, 0.07, FALSE);
}

void CMP7A1D::ELITEFire(float flSpread, float flCycleTime, BOOL fUseAutoAim)
{
	m_bDelayFire = true;
	m_iShotsFired++;
	m_flAccuracy = ((float)(m_iShotsFired * m_iShotsFired * m_iShotsFired) / 190) + 0.6;

	if (m_flAccuracy > 1.5)
		m_flAccuracy = 1.5;

	if (m_iClip <= 0)
	{
		if (m_fFireOnEmpty)
		{
			PlayEmptySound();
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.2;
		}

		return;
	}

	m_iClip--;
	m_flNextPrimaryAttack = m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + flCycleTime;
	m_pPlayer->m_iWeaponVolume = BIG_EXPLOSION_VOLUME;
	m_pPlayer->m_iWeaponFlash = DIM_GUN_FLASH;

	UTIL_MakeVectors(m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle);
	m_pPlayer->pev->effects |= EF_MUZZLEFLASH;

	Vector vecDir;
	int flags;
#ifdef CLIENT_WEAPONS
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	if (m_iWeaponState & WPNSTATE_ELITE_LEFT)
	{
#ifndef CLIENT_DLL
		m_pPlayer->SetAnimation(PLAYER_ATTACK1);
#endif
		m_iWeaponState &= ~WPNSTATE_ELITE_LEFT;

		vecDir = FireBullets3(m_pPlayer->GetGunPosition() - gpGlobals->v_right * 5, gpGlobals->v_forward, flSpread, 8192, 3, BULLET_PLAYER_57MM, 70, 0.83, m_pPlayer->pev, TRUE, m_pPlayer->random_seed);
		PLAYBACK_EVENT_FULL(flags, ENT(m_pPlayer->pev), m_usFireELITE_LEFT, 0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, (int)(m_pPlayer->pev->punchangle.y * 100), m_iClip, FALSE, FALSE);
	}
	else
	{
#ifndef CLIENT_DLL
		m_pPlayer->SetAnimation(PLAYER_ATTACK2);
#endif
		m_iWeaponState |= WPNSTATE_ELITE_LEFT;

		vecDir = FireBullets3(m_pPlayer->GetGunPosition() - gpGlobals->v_right * 5, gpGlobals->v_forward, flSpread, 8192, 3, BULLET_PLAYER_57MM, 70, 0.83, m_pPlayer->pev, TRUE, m_pPlayer->random_seed);
		PLAYBACK_EVENT_FULL(flags, ENT(m_pPlayer->pev), m_usFireELITE_RIGHT, 0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, (int)(m_pPlayer->pev->punchangle.y * 100), m_iClip, FALSE, FALSE);
	}

#ifndef CLIENT_DLL
	if (!m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);
#endif
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 2;

	if (!FBitSet(m_pPlayer->pev->flags, FL_ONGROUND))
		KickBack(1, 0.55, 0.4, 0.05, 5, 3.25, 9);
	else if (m_pPlayer->pev->velocity.Length2D() > 0)
		KickBack(0.6, 0.3, 0.25, 0.035, 3.5, 2.75, 7);
	else if (FBitSet(m_pPlayer->pev->flags, FL_DUCKING))
		KickBack(0.25, 0.175, 0.125, 0.02, 2.25, 1.2, 10);
	else
		KickBack(0.275, 0.2, 0.15, 0.02, 3, 1.75, 9);
}

void CMP7A1D::Reload(void)
{
	if (DefaultReload(WEAPON_MAXCLIP, ELITE_RELOAD, 3.6))
	{
#ifndef CLIENT_DLL
		m_pPlayer->SetAnimation(PLAYER_RELOAD);
#endif
		m_flAccuracy = 0.0;
	}
}

void CMP7A1D::WeaponIdle(void)
{
	ResetEmptySound();
	m_pPlayer->GetAutoaimVector(AUTOAIM_10DEGREES);

	if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
		return;

	if (m_iClip)
	{
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 60;

		if (m_iClip == 1)
			SendWeaponAnim(ELITE_IDLE_LEFTEMPTY, UseDecrement() != FALSE);
		else
			SendWeaponAnim(ELITE_IDLE, UseDecrement() != FALSE);
	}
}
