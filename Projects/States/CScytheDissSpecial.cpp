#include "pch.h"
#include "CScytheDissSpecial.h"

#include <Engine/CTransform.h>

CScytheDissSpecial::CScytheDissSpecial()
	: CState(SCYTHEDISSSPECIAL)
{
}

CScytheDissSpecial::~CScytheDissSpecial()
{
}

void CScytheDissSpecial::finaltick()
{
	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		return;
	}

	ChangeState(L"Idle");
}

void CScytheDissSpecial::Enter()
{
	GetOwner()->Animator2D()->Play(L"ScytheDiss_Special", false);
	m_OrginScale = GetOwner()->Transform()->GetRelativeScale();
	Vec3 Scale = m_OrginScale * 1.2f;
	GetOwner()->Transform()->SetRelativeScale(Scale);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);
}

void CScytheDissSpecial::Exit()
{
	GetOwner()->Transform()->SetRelativeScale(m_OrginScale);
	GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
}