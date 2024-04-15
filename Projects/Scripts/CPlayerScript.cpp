#include "pch.h"
#include "CPlayerScript.h"
#include "CPlayerMgr.h"
#include "CInvenMgr.h"
#include "CProgressBarScript.h"
#include "CGaugeScript.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CMovement.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CTransform.h>

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

void CPlayerScript::init()
{
	// Mgr 등록
	//CPlayerMgr::PlayerMgr()->SetPlayer(GetOwner());

	// Create Player's Component : Player Script에서는 생성자에서는 owner 모르고, begin은 이미 실행된 이후라 애매;
	Collider2D()->SetAbsolute(true);
	Collider2D()->SetOffsetScale(Vec3(50.f, 110.f, 1.f));
	Collider2D()->SetOffsetPos(Vec3(0.f, -10.f, 0.f));

	/*MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BloomMat"));*/
	// dynamic material
	MeshRender()->GetDynamicMaterial();
	MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(0.5f, 0.1f, 1.f, 1.f));
	MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.8f);

	Movement()->UseGravity(true);
	Movement()->SetInitSpeed(m_fSpeed);
	Movement()->SetInitSpeed_InAir(300.f);
	Movement()->SetMaxSpeed_Ground(m_fSpeed);
	Movement()->SetMaxSpeed_InAir(5000.f);
	Movement()->SetGravityForce(Vec3(0.f, -4000.f, 0.f));

	// Player Animation
	FILE* pFile = nullptr;
	CAnim* pAnim = new CAnim;

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Idle.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Idle");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_IdleToRun.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Idle_ToRun");
	//Animator2D()->Create(pFile);
	//fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_IdleUturn.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Idle_UTurn");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Jump_Falling.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Jump_Falling");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Jump_Landing.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Jump_Landing");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Jump_Start.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Jump_Start");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Run.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Run");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_RunToIdle.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Run_ToIdle");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_RunUturn.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Run_UTurn");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Dash.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Dash");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Concentrate.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Concentrate");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Concentrate_Start.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Concentrate_Start");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_PowerUp1.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"PowerUp");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Hit02.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Hit");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Disappear1.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Die");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Combo2a.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"ScytheDiss_ComboA");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Combo2b.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"ScytheDiss_ComboB");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Combo2c.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"ScytheDiss_ComboC");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Combo2d.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"ScytheDiss_ComboD");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Combo2aerial01.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"ScytheDiss_Aerial");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Combo2_Up.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"ScytheDiss_Up");

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Combo2_Crush.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"ScytheDiss_Crush");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Combo2_Special01.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"ScytheDiss_Special");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Attack_B_Rest.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"ScytheDiss_Rest");
	fclose(pFile);

	delete pAnim;
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

		if (StateMachine()->GetFSM().Get())
		{
			StateMachine()->GetFSM()->ChangeState(L"Idle");
		}
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
	CUnitScript::HitDamage(_Damage);  
	m_iHPActive -= _Damage.iActiveHPDamage;

	if (m_HPbar_Cur)
		m_HPbar_Cur->Decrease(_Damage.iCurHPDamage);
	if (m_HPbar_Active)
		m_HPbar_Active->SetProgress((float)m_iHPActive / (float)m_iHPMax);
}

void CPlayerScript::Attack()
{
	m_iRestCur += 5;
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