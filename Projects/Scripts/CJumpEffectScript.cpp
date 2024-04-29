#include "pch.h"
#include "CJumpEffectScript.h"

#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

CJumpEffectScript::CJumpEffectScript()
	: CScript(JUMPEFFECTSCRIPT)
{
}

CJumpEffectScript::~CJumpEffectScript()
{
}

void CJumpEffectScript::begin()
{
	for (CGameObject* it : GetOwner()->GetChild())
	{
		// play anim
		it->Animator2D()->Play(L"Dust", false);
		it->Animator2D()->DestroyAfterPlay();

		// set transform
		if (it->GetName() == L"R")
			it->Transform()->SetRelativeScale(it->Transform()->GetRelativeScale() * 1.2f);
		else
			it->Transform()->SetRelativeScale(it->Transform()->GetRelativeScale() * 0.7f);

		Vec3 Pos = it->Transform()->GetRelativePos();
		Pos.y += it->Transform()->GetRelativeScale().y / 2.f;
		it->Transform()->SetRelativePos(Pos);
	}
}

void CJumpEffectScript::tick()
{
	if (GetOwner()->GetChild().empty())
		GamePlayStatic::DestroyGameObject(GetOwner());
}

void CJumpEffectScript::SaveToFile(FILE* _File)
{
}

void CJumpEffectScript::LoadFromFile(FILE* _File)
{
}