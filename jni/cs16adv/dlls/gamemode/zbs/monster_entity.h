
#ifndef MONSTER_ENTITY_H
#define MONSTER_ENTITY_H
#ifdef _WIN32
#pragma once
#endif

#include "hostage/hostage.h"

enum MonsterAnim
{
	MONSTERANIM_IDLE,
	MONSTERANIM_WALK,
	MONSTERANIM_JUMP,
	MONSTERANIM_DIE,
	
	MONSTERANIM_FLINCH,
	MONSTERANIM_LARGE_FLINCH,
	MONSTERANIM_ATTACK,
	MONSTERANIM_SKILL,
};

class CMonster : public CHostage
{
public:
	virtual void Spawn();
	virtual void Precache();
	virtual int ObjectCaps() override { return (CBaseMonster::ObjectCaps() | FCAP_MUST_SPAWN); }
	virtual int Classify() { return CLASS_PLAYER_ALLY; }
	virtual int TakeDamage(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType);
	virtual void BecomeDead(void);
	virtual void Killed(entvars_t *pevAttacker, int iGib);
	virtual int BloodColor() { return BLOOD_COLOR_RED; }
	virtual void Touch(CBaseEntity *pOther);
	virtual void Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);

public:
	void EXPORT IdleThink();
	void Remove();

	CBaseEntity *FindTarget();
	void Wander();
	CBaseEntity *CheckAttack();
	CBaseEntity *CheckTraceHullAttack(float flDist, int iDamage, int iDmgType);
	bool ShouldAttack(CBaseEntity *target);

	void PlayDeathSound();
	void SetAnimation(MonsterAnim anim);

	void KillBouns(CBasePlayer *player);

public:
	float m_flAttackDamage;
};

#endif
