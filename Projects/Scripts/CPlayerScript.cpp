#include "pch.h"
#include "CPlayerScript.h"
#include "CPlayerMgr.h"
#include "CScytheDissScript.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CMovement.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CTransform.h>

CPlayerScript::CPlayerScript()
	: CUnitScript(PLAYERSCRIPT)
	, m_fJumpVelocMax(1300.f)
	, m_fSpeedDash(1800.f)
	, m_iMPMax(50)
	, m_iMPCur(m_iMPMax)
	, m_iIngot(0)
	, m_iSoulary(0)
	, m_iPrismium(0)
	, m_iAnimaMax(3)
	, m_iAnimaBlue(0)
	, m_iAnimaGold(0)
	, m_fDashCoolTime(1.f)
	, m_fDashAccTime(0.f)
	, m_bDashCan(true)
	, m_AirColPlatform(nullptr)
	, m_bAirCol(false)
	, m_ColWall(nullptr)
	, m_bWallCol(false)
	, m_CurScythe(nullptr)
	, m_fComboCoolTime(0.3f)
	, m_fComboAccTime(0.f)
	, m_NextComboIdx(0)
	, m_bAerialCan(true)
{
	m_fSpeed = 500.f;
	m_fSpeedInAir = m_fSpeed;
	m_iHPMax = 65;
	m_iHPActive = m_iHPMax;
	m_iHPCur = m_iHPMax;
	m_CurScythe = new CScytheDissScript();
}

CPlayerScript::~CPlayerScript()
{
	if (m_CurScythe)
		delete m_CurScythe;
}

void CPlayerScript::begin()
{
	// Mgr 등록
	CPlayerMgr::PlayerMgr()->SetPlayer(GetOwner());

	// Create Player's Component : Player Script에서는 생성자에서는 owner 모르고, begin은 이미 실행된 이후라 애매;
	Collider2D()->SetAbsolute(true);
	Collider2D()->SetOffsetScale(Vec3(50.f, 110.f, 1.f));
	Collider2D()->SetOffsetPos(Vec3(0.f, -10.f, 0.f));

	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMat"));
	MeshRender()->GetDynamicMaterial();

	Movement()->UseGravity(true);
	Movement()->SetInitSpeed(m_fSpeed);
	Movement()->SetMaxSpeed_Ground(m_fSpeedDash);
	Movement()->SetMaxSpeed_InAir(5000.f);
	Movement()->SetGravityForce(Vec3(0.f,-4000.f,0.f));

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
}

void CPlayerScript::tick()
{
	// set direction
	if ((KEY_TAP(KEY::A) || KEY_PRESSED(KEY::A)) && (KEY_NONE(KEY::D) || KEY_RELEASED(KEY::D)))
		SetDir(UNIT_DIRX::LEFT);

	if ((KEY_TAP(KEY::D) || KEY_PRESSED(KEY::D)) && (KEY_NONE(KEY::A) || KEY_RELEASED(KEY::A)))
		SetDir(UNIT_DIRX::RIGHT);

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

	CUnitScript::tick();
}

void CPlayerScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	// Platform
	if (_OtherObj->GetLayerIdx() == 6)
	{
		Movement()->SetGround(true);
		m_bAerialCan = true;

		// @TODO 오류나면 여기
 		Vec3 vPos = _Collider->GetFinalPos();
		vPos.y = _OtherCollider->GetFinalPos().y + _OtherCollider->GetFinalScale().y / 2.f + _Collider->GetFinalScale().y / 2.f - _Collider->GetOffsetPos().y;
		Transform()->SetRelativePos(vPos);
	}

	// Aerial Platform
	if (_OtherObj->GetLayerIdx() == 7)
	{
		m_AirColPlatform = _OtherObj;
		m_bAirCol = true;
	}
	
	// Wall
	if (_OtherObj->GetLayerIdx() == 8)
	{
		m_ColWall = _OtherObj;
		m_bWallCol = true;
	}
}

void CPlayerScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	if (_OtherObj->GetLayerIdx() == 6)
	{
		Movement()->SetGround(false);
	}

	if (_OtherObj->GetLayerIdx() == 7)
	{
		if (m_bAirCol)
		{
			m_AirColPlatform = nullptr;
			m_bAirCol = false;
		}
	}

	// Wall
	if (_OtherObj->GetLayerIdx() == 8)
	{
		m_ColWall = nullptr;
		m_bWallCol = false;
	}
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
