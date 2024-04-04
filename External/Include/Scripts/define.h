#pragma once
#include "struct.h"

enum class SCYTHE_TYPE
{
	DISS,

	END
};
extern wstring SCYTHE_NAME[(int)SCYTHE_TYPE::END];

extern FScytheDamage SCYTHE_DISS_DAMAGE;