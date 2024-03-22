#include "pch.h"
#include "CMissileScript_Test.h"
#include <Engine/CTransform.h>
#include <Engine/CTimeMgr.h>


CMissileScript_Test::CMissileScript_Test()
	: CScript(MISSILESCRIPT_TEST)
	, m_fSpeed(1500.f)
{
}

CMissileScript_Test::~CMissileScript_Test()
{
}

void CMissileScript_Test::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	vPos.y += m_fSpeed * DT;

	Transform()->SetRelativePos(vPos);	
}

void CMissileScript_Test::SaveToFile(FILE* _File)
{
}

void CMissileScript_Test::LoadFromFile(FILE* _File)
{
}