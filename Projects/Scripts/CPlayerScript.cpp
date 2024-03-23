#include "pch.h"
#include "CPlayerScript.h"

#include <Engine/components.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CMovement.h>

CPlayerScript::CPlayerScript()
	: CScript(PLAYERSCRIPT)
	, m_fSpeed(200.f)
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
{
}

CPlayerScript::CPlayerScript(CPlayerScript& _Origin)
	: CScript(_Origin)
	, m_fSpeed(_Origin.m_fSpeed)
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
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::init()
{
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

	// Player Animation
	FILE* pFile = nullptr;
	CAnim* pAnim = new CAnim;

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Idle.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"IDLE");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_IdleToRun.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"IDLE_ToRun");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_IdleUturn.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"IDLE_UTurn");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Run.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Run");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_RunToIdle.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Run_ToIdle");
	fclose(pFile);

	Animator2D()->Play(L"IDLE");
	delete pAnim;
}

void CPlayerScript::begin()
{
	init();
}

void CPlayerScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRotation();

	if (KEY_PRESSED(KEY::A))
	{
		Movement()->AddForce(Vec3(-m_fSpeed, 0, 0));
	}
	if (KEY_TAP(KEY::A))
		Animator2D()->Play(L"Run");
	if (KEY_RELEASED(KEY::A))
	{
		Animator2D()->Play(L"IDLE");
		Movement()->SetVelocity(Vec3());
	}

	if (KEY_PRESSED(KEY::D))
		Movement()->AddForce(Vec3(m_fSpeed, 0, 0));
	if (KEY_TAP(KEY::D))
		Animator2D()->Play(L"Run");
	if (KEY_RELEASED(KEY::D))
	{
		Animator2D()->Play(L"IDLE");
		Movement()->SetVelocity(Vec3());
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRotation(vRot);
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