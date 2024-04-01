#include "pch.h"
#include "CScytheDissRest.h"

CScytheDissRest::CScytheDissRest()
	: CState(SCYTHEDISSREST)
{
}

CScytheDissRest::~CScytheDissRest()
{
}

void CScytheDissRest::finaltick()
{
	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		return;
	}

	// change state
	ChangeState(L"Idle");
}

void CScytheDissRest::Enter()
{
	GetOwner()->Animator2D()->Play(L"ScytheDiss_Rest", false);

	// 순서대로 스폰
}

void CScytheDissRest::Exit()
{
}