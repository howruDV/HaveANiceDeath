#include "pch.h"
#include "CElevatorScript.h"
#include "CPlayerMgr.h"

#include <Engine\CKeyMgr.h>
#include <Engine\CAnimator2D.h>
#include <Engine\CTransform.h>

CElevatorScript::CElevatorScript()
	: CScript(ELEVATORSCRIPT)
{
}

CElevatorScript::~CElevatorScript()
{
}

void CElevatorScript::begin()
{
	Animator2D()->Play(L"Open", false);
}

void CElevatorScript::tick()
{
	float fDist = (CPlayerMgr::GetPlayer()->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Length();
	if (fDist > 100.f)
		return;

	// @TODO UI Àç»ý

	if (KEY_TAP(KEY::F))
	{
		// Level Control
		Animator2D()->Play(L"Close", false);
	}
}

void CElevatorScript::SaveToFile(FILE* _File)
{
}

void CElevatorScript::LoadFromFile(FILE* _File)
{
}