#include "pch.h"
#include "CPlayerScript_Test.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CAnimator2D.h>
#include <Engine/CMaterial.h>
#include <Engine/CRenderComponent.h>

#include <Scripts/CMissileScript_Test.h>

CPlayerScript_Test::CPlayerScript_Test()
	: CScript(PLAYERSCRIPT_TEST)
	, m_Speed(100.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Player Speed", &m_Speed);
}

CPlayerScript_Test::~CPlayerScript_Test()
{
}

void CPlayerScript_Test::begin()
{
	Ptr<CTexture> pAltasTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\link.png");
	Animator2D()->Create(L"IDLE_UP", pAltasTex, Vec2(0.f, 260.f), Vec2(0.f, 60.f), Vec2(120.f, 130.f), Vec2(120.f, 130.f), 1, 10);
	Animator2D()->Create(L"IDLE_DOWN", pAltasTex, Vec2(0.f, 0.f), Vec2(0.f, 60.f), Vec2(120.f, 130.f), Vec2(120.f, 130.f), 3, 10);
	
	//FILE* pFile = nullptr;
	//_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\Untitled.anim").c_str(), L"rb");
	//CAnim* pAnim = new CAnim;
	//pAnim->LoadFromFile(pFile);
	//Animator2D()->Create(pAnim, L"IDLE_UP");
	//fclose(pFile);
	//delete pAnim;
	
	//pFile = nullptr;
	//_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\death\\LD_Idle.anim").c_str(), L"rb");
	//CAnim* pAnim = new CAnim;
	//pAnim->LoadFromFile(pFile);
	//Animator2D()->Create(pFile);
	//fclose(pFile);
	//delete pAnim;
	
	Animator2D()->Create(L"IDLE_LEFT", pAltasTex, Vec2(0.f, 130.f), Vec2(0.f, 60.f), Vec2(120.f, 130.f), Vec2(120.f, 130.f), 3, 10);
	Animator2D()->Create(L"IDLE_RIGHT", pAltasTex, Vec2(0.f, 390.f), Vec2(0.f, 60.f), Vec2(120.f, 130.f), Vec2(120.f, 130.f), 3, 10);

	Animator2D()->Create(L"MOVE_UP", pAltasTex, Vec2(0.f, 780.f), Vec2(0.f, 60.f), Vec2(120.f, 130.f), Vec2(120.f, 130.f), 10, 20);
	Animator2D()->Create(L"MOVE_DOWN", pAltasTex, Vec2(0.f, 520.f), Vec2(0.f, 60.f), Vec2(120.f, 130.f), Vec2(120.f, 130.f), 10, 20);
	Animator2D()->Create(L"MOVE_LEFT", pAltasTex, Vec2(0.f, 650.f), Vec2(0.f, 60.f), Vec2(120.f, 130.f), Vec2(120.f, 130.f), 10, 20);
	Animator2D()->Create(L"MOVE_RIGHT", pAltasTex, Vec2(0.f, 910.f), Vec2(0.f, 60.f), Vec2(120.f, 130.f), Vec2(120.f, 130.f), 10, 20);

	Animator2D()->Play(L"IDLE_DOWN");
	GetRenderComponent()->GetDynamicMaterial();

	//m_Missile = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"MissilePrefab");
	//m_Missile = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\missile.pref");

	MeshRender()->GetDynamicMaterial();
	MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\cine\\Cine_RitualBoss_Outro_LD.png"));

}

void CPlayerScript_Test::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRotation();

	if (KEY_PRESSED(KEY::A))
		vPos.x -= DT * m_Speed;
	if (KEY_TAP(KEY::A))
		Animator2D()->Play(L"MOVE_LEFT");
	if (KEY_RELEASED(KEY::A))
		Animator2D()->Play(L"IDLE_LEFT");

	if (KEY_PRESSED(KEY::D))
		vPos.x += DT * m_Speed;
	if (KEY_TAP(KEY::D))
		Animator2D()->Play(L"MOVE_RIGHT");
	if (KEY_RELEASED(KEY::D))
		Animator2D()->Play(L"IDLE_RIGHT");

	if (KEY_PRESSED(KEY::W))
		vPos.y += DT * m_Speed;
	if (KEY_TAP(KEY::W))
		Animator2D()->Play(L"MOVE_UP");
	if (KEY_RELEASED(KEY::W))
		Animator2D()->Play(L"IDLE_UP");

	if (KEY_PRESSED(KEY::S))
		vPos.y -= DT * m_Speed;
	if (KEY_TAP(KEY::S))
		Animator2D()->Play(L"MOVE_DOWN");
	if (KEY_RELEASED(KEY::S))
		Animator2D()->Play(L"IDLE_DOWN");

	if (KEY_PRESSED(KEY::X))
	{
		vRot.x += DT * XM_PI;
	}

	if (KEY_PRESSED(KEY::Y))
	{
		vRot.y += DT * XM_PI;
	}

	if (KEY_PRESSED(KEY::Z))
	{
		vRot.z += DT * XM_PI;
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRotation(vRot);

	// GameObject 생성
	if (KEY_TAP(KEY::SPACE))
	{
		Instantiate(m_Missile, Transform()->GetWorldPos(), 0);
		//GamePlayStatic::Play2DSound(L"sound\\DM.wav", 1, 0.5f, false);
		GamePlayStatic::Play2DBGM(L"sound\\DM.wav", 0.5f);

		/*vector<CGameObject*> vecChild = GetOwner()->GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
			GamePlayStatic::DestroyGameObject(vecChild[i]);
		GetOwner()->Destroy();*/

		/*
		CGameObject* pObj = nullptr;

		pObj = new CGameObject;
		pObj->SetName(L"Missile");
		pObj->AddComponent(new CTransform);
		pObj->AddComponent(new CMeshRender);
		pObj->AddComponent(new CMissileScript);

		pObj->Transform()->SetRelativePos(Transform()->GetRelativePos());
		pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));
		pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pObj->MeshRender()->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"Std2DShader"));

		GamePlayStatic::SpawnGameObject(pObj, 0);
		*/
	}

	// paper burn: 시간이 지남에 따라 랜덤으로 날림
	// static float f = 0.f;
	// f += DT * 0.3f;
	// GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_1, f);

}

void CPlayerScript_Test::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
	Ptr<CMaterial> pMtrl = GetRenderComponent()->GetDynamicMaterial();
}

void CPlayerScript_Test::Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerScript_Test::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CPlayerScript_Test::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CPlayerScript_Test::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
}