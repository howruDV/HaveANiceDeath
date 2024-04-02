#include "pch.h"
#include "CWallScript.h"
#include "CUnitScript.h"

#include <Engine/CCollider2D.h>
#include <Engine/CTransform.h>

CWallScript::CWallScript()
	: CScript(WALLSCRIPT)
	, m_bUpdownCollision(true)
	, m_bSideCollision(false)
	, m_fGroundAdjust(5.f)
	, m_fSideAdjust(5.f)
{
	AddScriptParam(SCRIPT_PARAM::BOOL, "UpDown Collision", &m_bUpdownCollision);
	AddScriptParam(SCRIPT_PARAM::BOOL, "Side Collision", &m_bSideCollision);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "UpDown Adjust", &m_fGroundAdjust);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Side Adjust", &m_fSideAdjust);
}

CWallScript::CWallScript(const CWallScript& _Origin)
	: CScript(_Origin)
	, m_bUpdownCollision(_Origin.m_bUpdownCollision)
	, m_bSideCollision(_Origin.m_bSideCollision)
	, m_fGroundAdjust(_Origin.m_fGroundAdjust)
	, m_fSideAdjust(_Origin.m_fSideAdjust)
{
	AddScriptParam(SCRIPT_PARAM::BOOL, "UpDown Collision", &m_bUpdownCollision);
	AddScriptParam(SCRIPT_PARAM::BOOL, "Side Collision", &m_bSideCollision);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "UpDown Adjust", &m_fGroundAdjust);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Side Adjust", &m_fSideAdjust);
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

	const Vec3 vThisColLeftBot = Vec3(vThisColPos) - Vec3(vThisColScale / 2.f);
	const Vec3 vThisColRightTop = Vec3(vThisColPos) + Vec3(vThisColScale / 2.f);

	// case: updown collison
	if (vMoveDir.y != 0.f && fabs(vOtherColPos.y - vThisColPos.y) <= (vOtherColScale.y + vThisColScale.y) / 2.f)
	{
		if (m_bUpdownCollision)
		{
			float fPrevOtherT = vOtherColPos.y + vOtherColScale.y / 2.f - vMoveDir.y;
			float fPrevOtherB = vOtherColPos.y - vOtherColScale.y / 2.f - vMoveDir.y;

			// Push Up
			//if (vMoveDir.y < 0.f && vOtherColPos.y + m_fGroundAdjust > vThisColPos.y)
			if (vMoveDir.y < 0.f && fPrevOtherB + m_fGroundAdjust >= vThisColRightTop.y)
			{
				float ThisColTop = vThisColPos.y + vThisColScale.y / 2.f;
				vOtherRetPos.y = ThisColTop + vOtherColScale.y / 2.f - _OtherCollider->GetOffsetPos().y;

				if (_OtherObj->Movement())
				{
					_OtherObj->Movement()->SetGround(true);
					_OtherObj->GetScriptByType<CUnitScript>()->PushGround(GetOwner());
				}
			}

			// Push Down
			//else if (vMoveDir.y > 0.f && vOtherColPos.y < vThisColPos.y)
			else if(vMoveDir.y > 0.f && fPrevOtherT - m_fGroundAdjust <= vThisColLeftBot.y)
			{
				float ThisColBottom = vThisColPos.y - vThisColScale.y / 2.f;
				vOtherRetPos.y = ThisColBottom - vOtherColScale.y / 2.f - _OtherCollider->GetOffsetPos().y;
			}
		}
	}

	// case: side collision
	if (vMoveDir.x != 0.f && fabs(vOtherColPos.x - vThisColPos.x) <= (vOtherColScale.x + vThisColScale.x) / 2.f)
	{
		if (m_bSideCollision)
		{
			float fPrevOtherL = vOtherColPos.x - vOtherColScale.x / 2.f - vMoveDir.x;
			float fPrevOtherR = vOtherColPos.x + vOtherColScale.x / 2.f - vMoveDir.x;

			// Push Left
			if (vMoveDir.x > 0.f && fPrevOtherR - m_fSideAdjust <= vThisColLeftBot.x)
			{
				float ThisColLeft = vThisColPos.x - vThisColScale.x / 2.f;
				vOtherRetPos.x = ThisColLeft - vOtherColScale.x / 2.f - _OtherCollider->GetOffsetPos().x;
			}

			// Push Right
			else if (vMoveDir.x < 0.f && fPrevOtherL + m_fSideAdjust >= vThisColRightTop.x)
			{
				float ThisColRight = vThisColPos.x + vThisColScale.x / 2.f;
				vOtherRetPos.x = ThisColRight + vOtherColScale.x / 2.f - _OtherCollider->GetOffsetPos().x;
			}
		}
	}

	// Adjust Beside Two Colliders
	//|| vThisColPos.y + vThisColScale.y + m_fGroundAdjust > vOtherColPos.y + vOtherColScale.y / 2.f)
	if (m_bUpdownCollision && (vThisColRightTop.y > vOtherColPos.y - vOtherColScale.y / 2.f)
		&& (vThisColRightTop.y - m_fGroundAdjust <= vOtherColPos.y - vOtherColScale.y / 2.f))
	{
		float fNewY = vThisColRightTop.y + vOtherColScale.y / 2.f - _OtherCollider->GetOffsetPos().y;

		if (vOtherRetPos.y < fNewY)
			vOtherRetPos.y = fNewY + 1.f;
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


	const Vec3 vThisColLeftBot = Vec3(vThisColPos) - Vec3(vThisColScale / 2.f);
	const Vec3 vThisColRightTop = Vec3(vThisColPos) + Vec3(vThisColScale / 2.f);

	// case: updown collison
	if (vMoveDir.y != 0.f && fabs(vOtherColPos.y - vThisColPos.y) <= (vOtherColScale.y + vThisColScale.y) / 2.f)
	{
		if (m_bUpdownCollision)
		{
			float fPrevOtherT = vOtherColPos.y + vOtherColScale.y / 2.f - vMoveDir.y;
			float fPrevOtherB = vOtherColPos.y - vOtherColScale.y / 2.f - vMoveDir.y;

			// Push Up
			if (vMoveDir.y < 0.f && fPrevOtherB + m_fGroundAdjust >= vThisColRightTop.y)
			{
				//float ThisColTop = vThisColPos.y + vThisColScale.y / 2.f;
				//vOtherRetPos.y = ThisColTop + vOtherColScale.y / 2.f - _OtherCollider->GetOffsetPos().y;
			}

			// Push Down
			else if (vMoveDir.y > 0.f && fPrevOtherT - m_fGroundAdjust <= vThisColLeftBot.y)
			{
				float ThisColBottom = vThisColPos.y - vThisColScale.y / 2.f;
				vOtherRetPos.y = ThisColBottom - vOtherColScale.y / 2.f - _OtherCollider->GetOffsetPos().y;
			}
		}
	}

	// case: side collision
	if (vMoveDir.x != 0.f && fabs(vOtherColPos.x - vThisColPos.x) <= (vOtherColScale.x + vThisColScale.x) / 2.f)
	{
		if (m_bSideCollision)
		{
			float fPrevOtherL = vOtherColPos.x - vOtherColScale.x / 2.f - vMoveDir.x;
			float fPrevOtherR = vOtherColPos.x + vOtherColScale.x / 2.f - vMoveDir.x;

			// Push Left
			if (vMoveDir.x > 0.f && fPrevOtherR - m_fSideAdjust <= vThisColPos.x - vThisColScale.x / 2.f)
			{
 				float ThisColLeft = vThisColPos.x - vThisColScale.x / 2.f;
				vOtherRetPos.x = ThisColLeft - vOtherColScale.x / 2.f - _OtherCollider->GetOffsetPos().x;
			}

			// Push Right
			else if (vMoveDir.x < 0.f && fPrevOtherL + m_fSideAdjust >= vThisColPos.x + vThisColScale.x / 2.f)
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
	if (!m_bUpdownCollision)
		return;

	CUnitScript* pUnit = _OtherObj->GetScriptByType<CUnitScript>();
	if (pUnit && pUnit->IsGround(GetOwner()))
	{
		pUnit->DeleteGround(GetOwner());

		if (pUnit->GetGroundCount() == 0)
			pUnit->Movement()->SetGround(false);
	}
}

void CWallScript::SaveToFile(FILE* _File)
{
	fwrite(&m_bUpdownCollision, sizeof(bool), 1, _File);
	fwrite(&m_bSideCollision, sizeof(bool), 1, _File);
	fwrite(&m_fGroundAdjust, sizeof(float), 1, _File);
	fwrite(&m_fSideAdjust, sizeof(float), 1, _File);
}

void CWallScript::LoadFromFile(FILE* _File)
{
	fread(&m_bUpdownCollision, sizeof(bool), 1, _File);
	fread(&m_bSideCollision, sizeof(bool), 1, _File);
	fread(&m_fGroundAdjust, sizeof(float), 1, _File);
	fread(&m_fSideAdjust, sizeof(float), 1, _File);
}