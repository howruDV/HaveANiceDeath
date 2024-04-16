#include "pch.h"
#include "CCameraEffect.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CTransform.h>

CCameraEffect::CCameraEffect()
	: CScript(CAMERAEFFECT)
{
}

CCameraEffect::~CCameraEffect()
{
}

void CCameraEffect::tick()
{
	//if (m_queueEffect.empty())
	//	return;

	//FCamEffect& CurEffect = m_queueEffect.front();
	//CurEffect.fAccTime += DT;

	//switch (CurEffect.Type)
	//{
	//case CAMEFFECT_TYPE::SHAKE:
	//{
	//	float Strength = CurEffect.fVar;
	//	float deltaX = sinf(CTimeMgr::GetInst()->GetGameTime()) * Strength;
	//	float deltaY = sinf(CTimeMgr::GetInst()->GetGameTime()) * Strength;
	//	Vec3 Pos = Transform()->GetRelativePos();
	//	Pos.x += deltaX;
	//	Pos.y += deltaY;

	//	Transform()->SetRelativePos(Pos);
	//}
	//	break;

	//case CAMEFFECT_TYPE::TRANSITION_ON:
	//{

	//}
	//	break;

	//case CAMEFFECT_TYPE::TRANSITION_OFF:
	//{

	//}
	//	break;
	//}

	//if (CurEffect.fAccTime > CurEffect.fPlayTime)
	//	m_queueEffect.pop_front();
}

void CCameraEffect::SaveToFile(FILE* _File)
{
}

void CCameraEffect::LoadFromFile(FILE* _File)
{
}
