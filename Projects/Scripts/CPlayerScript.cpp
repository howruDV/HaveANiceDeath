#include "pch.h"
#include "CPlayerScript.h"
#include "CPlayerMgr.h"
#include "CInvenMgr.h"
#include "CProgressBarScript.h"
#include "CGaugeScript.h"
#include "CCamCtrlScript.h"
#include "CGameMgr.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CPlayerScript::CPlayerScript()
	: CUnitScript(PLAYERSCRIPT)
	, m_fJumpVelocMax(1300.f)
	, m_fSpeedDash(1800.f)
	, m_iMPMax(50)
	, m_iMPCur(m_iMPMax)
	, m_iRestMax(100)
	, m_iRestCur(0)
	, m_fDashCoolTime(1.f)
	, m_fDashAccTime(0.f)
	, m_bDashCan(true)
	, m_AirColPlatform(nullptr)
	, m_bAirCol(false)
	, m_fComboCoolTime(0.3f)
	, m_fComboAccTime(0.f)
	, m_NextComboIdx(0)
	, m_bAerialCan(true)
	, m_bComboCan(false)
{
	m_fSpeed = 500.f;
	m_fSpeedInAir = m_fSpeed;
	m_iHPMax = 65.f;
	m_iHPCur = m_iHPMax;
	m_iHPActive = m_iHPMax;

	AddScriptParam(SCRIPT_PARAM::INT, "HP Active", &m_iHPActive);
}

CPlayerScript::CPlayerScript(const CPlayerScript& _Origin)
	: CUnitScript(_Origin)
	, m_fJumpVelocMax(_Origin.m_fJumpVelocMax)
	, m_fSpeedDash(_Origin.m_fSpeedDash)
	, m_iMPMax(_Origin.m_iMPMax)
	, m_iMPCur(_Origin.m_iMPCur)
	, m_fDashCoolTime(_Origin.m_fDashCoolTime)
	, m_fDashAccTime(_Origin.m_fDashAccTime)
	, m_bDashCan(_Origin.m_bDashCan)
	, m_AirColPlatform(_Origin.m_AirColPlatform)
	, m_bAirCol(_Origin.m_bAirCol)
	, m_fComboCoolTime(_Origin.m_fComboCoolTime)
	, m_fComboAccTime(_Origin.m_fComboAccTime)
	, m_NextComboIdx(_Origin.m_NextComboIdx)
	, m_bAerialCan(_Origin.m_bAerialCan)
{
	AddScriptParam(SCRIPT_PARAM::INT, "HP Active", &m_iHPActive);
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
	CUnitScript::begin();

	MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(0.5f, 0.1f, 1.f, 1.f));
	MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.8f);

	// StateMachine
	if (StateMachine())
	{
		StateMachine()->AddBlackboardData(L"fSpeed", BB_DATA::FLOAT, &m_fSpeed);
		StateMachine()->AddBlackboardData(L"fSpeedInAir", BB_DATA::FLOAT, &m_fSpeedInAir);
		StateMachine()->AddBlackboardData(L"fSpeedDash", BB_DATA::FLOAT, &m_fSpeedDash);
		StateMachine()->AddBlackboardData(L"fJumpVelocMax", BB_DATA::FLOAT, &m_fJumpVelocMax);
		
		StateMachine()->GetFSM()->ChangeState(L"Idle");
	}

	// Find Level Obj
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (pLevel)
	{
		int LayerIdx = pLevel->GetLayerIdxByName(L"UI");

		CGameObject* pObj = pLevel->FindObjectByName(L"HUD_lifebar", LayerIdx);
		if (pObj)
			m_HPbar_Cur = pObj->GetScriptByType<CProgressBarScript>();

		pObj = pLevel->FindObjectByName(L"HUD_lifebar_active", LayerIdx);
		if (pObj)
			m_HPbar_Active = pObj->GetScriptByType<CProgressBarScript>();

		pObj = pLevel->FindObjectByName(L"HUD_RestGauge", LayerIdx);
		if (pObj)
			m_RestGauge = pObj->GetScriptByType<CGaugeScript>();
	}

	m_pEffectDash = GetOwner()->GetChildByName(L"EffectDash");
	if (m_pEffectDash)
		OffEffectDash();
}

void CPlayerScript::tick()
{
	// ----------------------------
	// set direction
	// ----------------------------
	if ((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && (KEY_NONE(KEY::D) || KEY_RELEASED(KEY::D)))
		SetDir(UNIT_DIRX::LEFT);

	if ((KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)) && (KEY_NONE(KEY::A) || KEY_RELEASED(KEY::A)))
		SetDir(UNIT_DIRX::RIGHT);

	if ((KEY_TAP(KEY::E)))
	{
		CPlayerMgr::GetInventory()->UseAnima();
	}
	if ((KEY_TAP(KEY::R)))
	{
		CPlayerMgr::GetInventory()->AddAnima();
	}

	CUnitScript::tick();

	// ----------------------------
	// check state
	// ----------------------------
	
	// check coolTime
	if (!m_bDashCan)
	{
		m_fDashAccTime += DT;

		if (m_fDashAccTime > m_fDashCoolTime)
		{
			m_bDashCan = true;
			m_fDashAccTime = 0.f;
		}
	}

	if (m_bComboCan)
	{
		m_fComboAccTime += DT;

		if (m_fComboAccTime > m_fComboCoolTime)
		{
			m_bComboCan = false;
			m_fComboAccTime = 0.f;
			m_NextComboIdx = 0;
		}
	}

	// HP - Active
	if (m_iHPActive > m_iHPMax)
		m_iHPActive = m_iHPMax;
	else if (m_iHPActive <= 0)
		m_iHPActive = 0;

	if (m_iHPCur > m_iHPActive)
		m_iHPCur = m_iHPActive;
	else if (m_iHPCur <= 0)
		m_iHPCur = 0;
		// ChangeState

	// MP
	if (m_iMPCur > m_iMPMax)
		m_iMPCur = m_iMPMax;
	else if (m_iMPCur <= 0)
		m_iMPCur = 0;

	// Rest
	if (m_iRestCur > m_iRestMax)
		m_iRestCur = m_iRestMax;
}

void CPlayerScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	if (CLevelMgr::GetInst()->GetCurrentLevel()->GetState() != LEVEL_STATE::PLAY)
		return;

	// Platform
	if (_OtherObj->GetLayerIdx() == 6)
	{
		wstring strState = StateMachine()->GetFSM()->GetCurState()->GetName();

		// case: upto platform
		if (strState.find(L"_Crush") != std::string::npos)
		{
			Vec3 GroundPos = Transform()->GetRelativePos();
			GroundPos.y = _OtherCollider->GetFinalPos().y + _OtherCollider->GetFinalScale().y / 2.f;
			GroundPos.y += _Collider->GetFinalScale().y / 2.f - _Collider->GetOffsetPos().y;
			Transform()->SetRelativePos(GroundPos);

			Movement()->SetGround(true);
		}

		// case: aireal collision
		if ( strState == L"PowerUp" || strState.find(L"_Up") != std::string::npos)
		{
			m_AirColPlatform = _OtherObj;
			m_bAirCol = true;
		}
	}
}


void CPlayerScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{

}

void CPlayerScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	CUnitScript::EndOverlap(_Collider, _OtherObj, _OtherCollider);

	if (_OtherObj == m_AirColPlatform)
	{
		m_AirColPlatform = nullptr;
		m_bAirCol = false;
	}
}

void CPlayerScript::BeginPushUp(CGameObject* _OtherObj)
{
	m_bAerialCan = true;
}

void CPlayerScript::BeginPushDown(CGameObject* _OtherObj)
{
	//m_AirColPlatform = _OtherObj;
	//m_bAirCol = true;
}

void CPlayerScript::EndPushUp(CGameObject* _OtherObj)
{
}

void CPlayerScript::EndPushDown(CGameObject* _OtherObj)
{
	/*m_AirColPlatform = nullptr;
	m_bAirCol = false;*/
}

void CPlayerScript::SaveToFile(FILE* _File)
{
	CUnitScript::SaveToFile(_File);

	fwrite(&m_fSpeedInAir, sizeof(float), 1, _File);
	fwrite(&m_fSpeedDash, sizeof(float), 1, _File);
	fwrite(&m_fJumpVelocMax, sizeof(float), 1, _File);
	fwrite(&m_iHPActive, sizeof(int), 1, _File);
	fwrite(&m_iMPMax, sizeof(int), 1, _File);
	fwrite(&m_iMPCur, sizeof(int), 1, _File);
	fwrite(&m_iRestMax, sizeof(int), 1, _File);
	fwrite(&m_iRestCur, sizeof(int), 1, _File);
	fwrite(&m_fDashCoolTime, sizeof(int), 1, _File);
	fwrite(&m_fComboCoolTime, sizeof(int), 1, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	CUnitScript::LoadFromFile(_File);

	fread(&m_fSpeedInAir, sizeof(float), 1, _File);
	fread(&m_fSpeedDash, sizeof(float), 1, _File);
	fread(&m_fJumpVelocMax, sizeof(float), 1, _File);
	fread(&m_iHPActive, sizeof(int), 1, _File);
	fread(&m_iMPMax, sizeof(int), 1, _File);
	fread(&m_iMPCur, sizeof(int), 1, _File);
	fread(&m_iRestMax, sizeof(int), 1, _File);
	fread(&m_iRestCur, sizeof(int), 1, _File);
	fread(&m_fDashCoolTime, sizeof(int), 1, _File);
	fread(&m_fComboCoolTime, sizeof(int), 1, _File);
}

void CPlayerScript::HitDamage(FDamage _Damage)
{
	if (StateMachine()->GetFSM()->GetCurState()->GetName() == L"Dash")
		return;

	CUnitScript::HitDamage(_Damage);  
	m_iHPActive -= _Damage.iActiveHPDamage;

	if (m_HPbar_Cur)
		m_HPbar_Cur->Decrease(_Damage.iCurHPDamage);
	if (m_HPbar_Active)
		m_HPbar_Active->SetProgress((float)m_iHPActive / (float)m_iHPMax);
}

void CPlayerScript::Attack()
{
	// camera shake
	FCamEffect Shake{};
	Shake.Type = CAMEFFECT_TYPE::SHAKE;
	Shake.fPlayTime = 0.3f;
	Shake.fAccTime = 0.f;
	Shake.fVar = 5.f;
	CGameMgr::GetMainCamera()->GetScriptByType<CCamCtrlScript>()->SetEffect(Shake);

	// add Rest
	if (m_iRestCur >= m_iRestMax)
		return;

	m_iRestCur += 5;

	if (m_iRestCur > m_iRestMax)
		m_iRestCur = m_iRestMax;
	
	m_RestGauge->Increase((float)m_iRestCur/(float)m_iRestMax);
}

void CPlayerScript::AddHPCur(int _Add)
{
	m_iHPCur += _Add;
	m_HPbar_Cur->Increase(_Add);
}

void CPlayerScript::AddHPActive(int _Add)
{
	m_iHPActive += _Add;
	m_HPbar_Active->SetProgress((float)m_iHPActive / (float)m_iHPMax);
}

void CPlayerScript::StartDashCoolTime(bool _bDashCan)
{
	if (!_bDashCan)
	{ 
		m_fDashAccTime = 0.f;
	}
	
	m_bDashCan = _bDashCan;
}

void CPlayerScript::StartCombo(int _ComboIdx)
{
	m_fComboAccTime = 0.f;
	m_NextComboIdx = _ComboIdx + 1;
	m_bComboCan = true;
}

void CPlayerScript::ResetRest()
{
	m_iRestCur = 0.f;
	m_RestGauge->Reset();
}

void CPlayerScript::OnEffectDash()
{
	if (!m_pEffectDash)
		return;

	m_pEffectDash->ParticleSystem()->SpawnOn();
}

void CPlayerScript::OffEffectDash()
{
	if (!m_pEffectDash)
		return;

	m_pEffectDash->ParticleSystem()->SpawnOff();
}
