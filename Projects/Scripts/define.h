#pragma once
#include "struct.h"

enum class SCYTHE_TYPE
{
	DISS,

	END
};
extern wstring SCYTHE_NAME[(int)SCYTHE_TYPE::END];

enum class SCYTHE_DAMAGE_NAME
{
	ComboA,
	ComboB,
	ComboC,
	ComboD,
	Air,
	Up,
	Special,
	Crush,
	Concentrate1,
	Concentrate2,
	Rest,
};

extern FScytheDamage SCYTHE_DISS_DAMAGE;