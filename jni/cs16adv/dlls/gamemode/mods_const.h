#ifndef BTE_MOD_CONST_H
#define BTE_MOD_CONST_H
#ifdef _WIN32
#pragma once
#endif

enum GameMode_e : byte
{
	MOD_SINGLEPLAY = 0, // backward supports
	MOD_MULTIPLAY = 1, // backward supports
	MOD_NONE = 2,
	MOD_DM = 3,
	MOD_TDM = 4,
	MOD_ZB1 = 5,
	MOD_ZB2 = 6,
	MOD_ZBU = 7,
	MOD_ZB3 = 8,
	MOD_ZBS = 9,
	MOD_ZE = 10,
};

enum DamageTrack_e : byte
{
	DT_NONE,
	DT_ZB,
	DT_ZBS,
	DT_ZSE
};

#endif