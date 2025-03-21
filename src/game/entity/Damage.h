//
// Created by EmsiaetKadosh on 25-3-21.
//

#pragma once

#include "..\..\def.h"

class [[carlbeks::predecl, carlbeks::defineat("Entity.h")]] Entity;

struct DamageTypeEnum {
	enum : unsigned char {
		NONE, // 无伤害
		SYSTEMATIC_DAMAGE, // 系统伤害
		PHYSICAL_DAMAGE, // 物理伤害
		MAGICAL_DAMAGE, // 魔法伤害
		TRUE_DAMAGE, // 真实伤害
		DENY_DAMAGE, // 否定伤害
		RETURN_DAMAGE, // 回敬伤害
		BLOOD_PRESSURE_DAMAGE, // 降压伤害
	} value;

	static constexpr QWORD elementCount() { return BLOOD_PRESSURE_DAMAGE + 1; }
};

struct DamageFormEnum {
	enum : unsigned char {
		NONE,
		MELEE_DAMAGE, // 近战伤害
		REMOTE_DAMAGE, // 远程伤害
		PROJECTILE_DAMAGE, // 弹射物伤害
		EXECUTE_DAMAGE, // 处决伤害
		FINAL_DAMAGE,
	} value;

	static constexpr QWORD elementCount() { return FINAL_DAMAGE + 1; }
};

struct Damage {
	double damages[DamageTypeEnum::elementCount()];
	DamageFormEnum form;

};
