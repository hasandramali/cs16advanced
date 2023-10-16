/*
wpn_mp7a1p.cpp
Copyright (C) 2019 Moemod Hyakuya

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "weapons.h"

#include "weapons/WeaponTemplate.hpp"

class CMP7A1_Pistol : public LinkWeaponTemplate< CMP7A1_Pistol,
	TGeneralData,
	TDoubleModeType,
	TDeployDoubleMode,
	TReloadDefault,
	TWeaponIdleDefault,
	BuildTGetItemInfoFromCSW<WEAPON_MP5N>::template type,
	TFireRifle,
	TRecoilKickBack,
	TPrimaryAttackRifle,
	TPrecacheEvent,
	TGetDamageDefault
>
{
private:
	enum mp7a1_e
	{
		MP7A1_PISTOL_IDLE,
		MP7A1_PISTOL_RELOAD,
		MP7A1_PISTOL_DRAW,
		MP7A1_PISTOL_SHOOT1,
		MP7A1_PISTOL_SHOOT2,
		MP7A1_CHANGE_CARBINE,
		MP7A1_CARBINE_IDLE1,
		MP7A1_CARBINE_RELOAD,
		MP7A1_CARBINE_DRAW,
		MP7A1_CARBINE_SHOOT1,
		MP7A1_CARBINE_SHOOT2,
		MP7A1_CHANGE_PISTOL,
	};

public:
	static constexpr auto MaxClip = 20;
	static constexpr auto DefaultReloadTime = 3.6;
	static constexpr auto DefaultAccuracy = 0.2;
	static constexpr int ZoomFOV = 50;

	static constexpr const char *V_Model = "models/v_mp7a1.mdl";
	static constexpr const char *P_Model = "models/p_mp7a1.mdl";
	static constexpr const char *W_Model = "models/w_mp7a1.mdl";
	static constexpr const char *EventFile = "events/mp7a1p.sc";
	static constexpr float ArmorRatioModifier = 1.0f;
	static constexpr InventorySlotType ItemSlot = PRIMARY_WEAPON_SLOT;
	static constexpr const char *ClassName = "weapon_mp7a1p";
	static constexpr const char *AnimExtension = "pistol";

	static constexpr const auto & KnockBack = KnockbackData{ 250.0f, 200.0f, 250.0f, 90.0f, 0.7f };
	static constexpr float MaxSpeed = 250;

	enum
	{
		ANIM_IDLE1 = MP7A1_PISTOL_IDLE,
		ANIM_RELOAD = MP7A1_PISTOL_RELOAD,
		ANIM_DRAW = MP7A1_PISTOL_DRAW,
		ANIM_CHANGE = MP7A1_CHANGE_PISTOL
	};

	// change ammo to 46mm
	struct ItemInfoData_t : Base::ItemInfoData_t
	{
		static constexpr const char *szAmmo1 = "46MM";
		static constexpr int iMaxAmmo1 = MAX_AMMO_46MM;
	};
#ifndef CLIENT_DLL
	WeaponBuyAmmoConfig GetBuyAmmoConfig() override { return { "ammo_46mm" , AMMO_46MM_PRICE }; }
#endif

	static constexpr const auto & SpreadCalcNotOnGround = 0.375 * A;
	static constexpr const auto & SpreadCalcDefault = 0.035 * A;

	static constexpr float CycleTime = 0.07;

	static constexpr int DamageDefault = 29;
	static constexpr int DamageZB = 37;
	static constexpr int DamageZBS = 37;

	static constexpr const auto & AccuracyCalc = (N * N / 190.0) + 0.6;
	static constexpr float AccuracyMax = 1.5f;
	static constexpr float RangeModifier = 0.83;
	static constexpr auto BulletType = BULLET_PLAYER_57MM;
	static constexpr int Penetration = 1;
	static constexpr int Distance = 8192;

	static constexpr const auto & KickBackWalking = KickBackData{ 0.6, 0.3, 0.25, 0.035, 3.5, 2.75, (signed int)10 };
	static constexpr const auto & KickBackNotOnGround = KickBackData{ 1, 0.55, 0.4, 0.05, 5, 3.25, (signed int)9 };
	static constexpr const auto & KickBackDucking = KickBackData{ 0.25, 0.175, 0.125, 0.02, 2.25, 1.2, (signed int)10 };
	static constexpr const auto & KickBackDefault = KickBackData{ 0.275, 0.2, 0.15, 0.02, 3, 1.75, (signed int)9 };

	static constexpr const auto DoubleMode_LinkedWeaponClassName = "weapon_mp7a1c";
	static constexpr bool DoubleMode_SyncAmmo = true;
	static constexpr float DoubleMode_ChangeTime = 6.0f;
};

LINK_ENTITY_TO_CLASS(weapon_mp7a1p, CMP7A1_Pistol)