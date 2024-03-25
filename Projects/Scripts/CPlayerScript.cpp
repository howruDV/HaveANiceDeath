#include "pch.h"
#include "CPlayerScript.h"
#include "CPlayerMgr.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CMovement.h>

CPlayerScript::CPlayerScript()
	: CScript(PLAYERSCRIPT)
	, m_fSpeed(500.f)
	, m_fSpeedInAir(m_fSpeed)
	, m_fJumpVelocMax(1300.f)
	, m_iHPMax(65)
	, m_iHPActive(m_iHPMax)
	, m_iHPCur(m_iHPMax)
	, m_iMPMax(50)
	, m_iMPCur(m_iMPCur)
	, m_iIngot(0)
	, m_iSoulary(0)
	, m_iPrismium(0)
	, m_iAnimaMax(3)
	, m_iAnimaBlue(0)
	, m_iAnimaGold(0)
	, m_bLookLeft(false)
	, m_bLookLeft_Prev(m_bLookLeft)
{
}

CPlayerScript::CPlayerScript(CPlayerScript& _Origin)
	: CScript(_Origin)
	, m_fSpeed(_Origin.m_fSpeed)
	, m_fSpeedInAir(_Origin.m_fSpeedInAir)
	, m_fJumpVelocMax(_Origin.m_fJumpVelocMax)
	, m_iHPMax(65)
	, m_iHPActive(m_iHPMax)
	, m_iHPCur(m_iHPMax)
	, m_iMPMax(50)
	, m_iMPCur(m_iMPCur)
	, m_iIngot(0)
	, m_iSoulary(0)
	, m_iPrismium(0)
	, m_iAnimaMax(3)
	, m_iAnimaBlue(0)
	, m_iAnimaGold(0)
	, m_bLookLeft(_Origin.m_bLookLeft)
	, m_bLookLeft_Prev(m_bLookLeft)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
	// Mgr 등록
	CPlayerMgr::PlayerMgr()->SetPlayer(GetOwner());

	// Create Player's Component : Player Script에서는 생성자에서는 owner 모르고, begin은 이미 실행된 이후라 애매;
	Collider2D()->SetAbsolute(true);
	Collider2D()->SetOffsetScale(Vec3(70.f, 80.f, 1.f));
	Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));

	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMat"));
	MeshRender()->GetDynamicMaterial();

	Movement()->UseGravity(true);
	Movement()->SetInitSpeed(m_fSpeed);
	Movement()->SetMaxSpeed_Ground(m_fSpeed);
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
	fclose(pFile);

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
	Animator2D()->Create(pAnim, L"Run_Uturn");
	fclose(pFile);

	delete pAnim;

	// StateMachine
	if (StateMachine())
	{
		StateMachine()->AddBlackboardData(L"fSpeed", BB_DATA::FLOAT, &m_fSpeed);
		StateMachine()->AddBlackboardData(L"fSpeedInAir", BB_DATA::FLOAT, &m_fSpeedInAir);
		StateMachine()->AddBlackboardData(L"fJumpVelocMax", BB_DATA::FLOAT, &m_fJumpVelocMax);

		if (StateMachine()->GetFSM().Get())
		{
			StateMachine()->GetFSM()->ChangeState(L"Idle");
		}
	}
}

void CPlayerScript::tick()
{
	if (m_bLookLeft != m_bLookLeft_Prev)
	{
		if (GetOwner()->Animator2D())
		{
			GetOwner()->Animator2D()->SetFlipX(m_bLookLeft);
		}
		else
		{
			// @TODO 아 ㅋㅋ 쓸때추가해ㅋㅋ
			//GetOwner()->GetRenderComponent()->GetDynamicMaterial
		}
	}
	
	m_bLookLeft_Prev = m_bLookLeft;
}

void CPlayerScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	if (_OtherObj->GetLayerIdx() == 6)
		Movement()->SetGround(true);
}

void CPlayerScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	if (_OtherObj->GetLayerIdx() == 6)
		Movement()->SetGround(false);
}