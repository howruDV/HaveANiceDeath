#include "pch.h"
#include "CMissileScript.h"
#include <Engine/CTransform.h>
#include <Engine/CTimeMgr.h>


CMissileScript::CMissileScript()
	: CScript(MISSILESCRIPT)
	, m_fSpeed(1500.f)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	vPos.y += m_fSpeed * DT;

	Transform()->SetRelativePos(vPos);	
}

void CMissileScript::SaveToFile(FILE* _File)
{
}

void CMissileScript::LoadFromFile(FILE* _File)
{
}