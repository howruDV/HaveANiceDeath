#include "pch.h"
#include "CWallScript.h"
#include "CUnitScript.h"

#include <Engine/CCollider2D.h>
#include <Engine/CTransform.h>

CWallScript::CWallScript()
	: CScript(WALLSCRIPT)
	, m_bUpdownCollision(true)
	, m_bSideCollision(false)
{
	AddScriptParam(SCRIPT_PARAM::BOOL, "UpDown Collision", &m_bUpdownCollision);
	AddScriptParam(SCRIPT_PARAM::BOOL, "Side Collision", &m_bSideCollision);
}

CWallScript::CWallScript(const CWallScript& _Origin)
	: CScript(_Origin)
	, m_bUpdownCollision(_Origin.m_bUpdownCollision)
	, m_bSideCollision(_Origin.m_bSideCollision)
{
	AddScriptParam(SCRIPT_PARAM::BOOL, "UpDown Collision", &m_bUpdownCollision);
	AddScriptParam(SCRIPT_PARAM::BOOL, "Side Collision", &m_bSideCollision);
}

CWallScript::~CWallScript()
{
}

void CWallScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	if (!_OtherObj->GetScriptByType<CUnitScript>() || !_OtherObj->Collider2D())
		return;

	Vec3 vOtherRetPos = _OtherObj->Transform()->GetRelativePos();
	const Vec3 vMoveDir = _OtherObj->Transform()->GetRelativePos() - _OtherObj->Transform()->GetRelativePos_Prev();
	const Vec3 vThisColPos = _Collider->GetFinalPos();
	const Vec3 vOtherColPos = _OtherCollider->GetFinalPos();
	const Vec3 vThisColScale = _Collider->GetFinalScale();
	const Vec3 vOtherColScale = _OtherCollider->GetFinalScale();

	// case: updown collison
	if (vMoveDir.y != 0.f && fabs(vOtherColPos.y - vThisColPos.y) <= (vOtherColScale.y + vThisColScale.y) / 2.f)
	{
		if (m_bUpdownCollision)
		{
			// Push Up
			if (vMoveDir.y < 0.f && vOtherColPos.y > vThisColPos.y)
			{
				float ThisColTop = vThisColPos.y + vThisColScale.y / 2.f;
				vOtherRetPos.y = ThisColTop + vOtherColScale.y / 2.f - _OtherCollider->GetOffsetPos().y;

				if (_OtherObj->Movement())
					_OtherObj->Movement()->SetGround(true);
			}

			// Push Down
			else if (vMoveDir.y > 0.f && vOtherColPos.y < vThisColPos.y)
			{
				float ThisColBottom = vThisColPos.y - vThisColScale.y / 2.f;
				vOtherRetPos.y = ThisColBottom - vOtherColScale.y / 2.f - _OtherCollider->GetOffsetPos().y;
			}
		}
	}

	// case: side collision
	else if (vMoveDir.x != 0.f && fabs(vOtherColPos.x - vThisColPos.x) <= (vOtherColScale.x + vThisColScale.x) / 2.f)
	{
		if (m_bSideCollision)
		{
			float fPrevOtherL = vOtherColPos.x - vOtherColScale.x / 2.f - vMoveDir.x;
			float fPrevOtherR = vOtherColPos.x + vOtherColScale.x / 2.f - vMoveDir.x;

			// Push Left
			if (vMoveDir.x > 0.f && fPrevOtherR <= vThisColPos.x - vThisColScale.x / 2.f)
			{
				float ThisColLeft = vThisColPos.x - vThisColScale.x / 2.f;
				vOtherRetPos.x = ThisColLeft - vOtherColScale.x / 2.f - _OtherCollider->GetOffsetPos().x;
			}

			// Push Right
			else if (vMoveDir.x < 0.f && fPrevOtherL >= vThisColPos.x + vThisColScale.x / 2.f)
			{
				float ThisColRight = vThisColPos.x + vThisColScale.x / 2.f;
				vOtherRetPos.x = ThisColRight + vOtherColScale.x / 2.f - _OtherCollider->GetOffsetPos().x;
			}
		}
	}

	_OtherObj->Transform()->SetRelativePos(vOtherRetPos);
}

void CWallScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	if (!_OtherObj->GetScriptByType<CUnitScript>() || !_OtherObj->Collider2D())
		return;

	Vec3 vOtherRetPos = _OtherObj->Transform()->GetRelativePos();
	const Vec3 vMoveDir = _OtherObj->Transform()->GetRelativePos() - _OtherObj->Transform()->GetRelativePos_Prev();
	const Vec3 vThisColPos = _Collider->GetFinalPos();
	const Vec3 vOtherColPos = _OtherCollider->GetFinalPos();
	const Vec3 vThisColScale = _Collider->GetFinalScale();
	const Vec3 vOtherColScale = _OtherCollider->GetFinalScale();

	// case: updown collison
	if (vMoveDir.y != 0.f && fabs(vOtherColPos.y - vThisColPos.y) <= (vOtherColScale.y + vThisColScale.y) / 2.f)
	{
		if (m_bUpdownCollision)
		{
			// Push Up
			if (vMoveDir.y < 0.f && vOtherColPos.y > vThisColPos.y)
			{
				float ThisColTop = vThisColPos.y + vThisColScale.y / 2.f;
				vOtherRetPos.y = ThisColTop + vOtherColScale.y / 2.f - _OtherCollider->GetOffsetPos().y;

				if (_OtherObj->Movement())
					_OtherObj->Movement()->SetGround(true);
			}

			// Push Down
			else if (vMoveDir.y > 0.f && vOtherColPos.y < vThisColPos.y)
			{
				float ThisColBottom = vThisColPos.y - vThisColScale.y / 2.f;
				vOtherRetPos.y = ThisColBottom - vOtherColScale.y / 2.f - _OtherCollider->GetOffsetPos().y;
			}
		}
	}

	// case: side collision
	else if (vMoveDir.x != 0.f && fabs(vOtherColPos.x - vThisColPos.x) <= (vOtherColScale.x + vThisColScale.x) / 2.f)
	{
		if (m_bSideCollision)
		{
			float fPrevOtherL = vOtherColPos.x - vOtherColScale.x / 2.f - vMoveDir.x;
			float fPrevOtherR = vOtherColPos.x + vOtherColScale.x / 2.f - vMoveDir.x;

			// Push Left
			if (vMoveDir.x > 0.f && fPrevOtherR <= vThisColPos.x - vThisColScale.x / 2.f)
			{
 				float ThisColLeft = vThisColPos.x - vThisColScale.x / 2.f;
				vOtherRetPos.x = ThisColLeft - vOtherColScale.x / 2.f - _OtherCollider->GetOffsetPos().x;
			}

			// Push Right
			else if (vMoveDir.x < 0.f && fPrevOtherL >= vThisColPos.x + vThisColScale.x / 2.f)
			{
				float ThisColRight = vThisColPos.x + vThisColScale.x / 2.f;
				vOtherRetPos.x = ThisColRight + vOtherColScale.x / 2.f - _OtherCollider->GetOffsetPos().x;
			}
		}
	}

	_OtherObj->Transform()->SetRelativePos(vOtherRetPos);
}

void CWallScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}