#include "pch.h"
#include "CCreateTempLevel.h"
#include "CLevelSaveLoad.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CCollisionMgr.h>

#include <Engine/components.h>
#include <Engine/CGameObject.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CTexture.h>
#include <Engine/CPrefab.h>
#include <Engine/CFSM.h>
#include <Engine/CAnim.h>

#include <Engine/CSetColorShader.h>

#include <Scripts/CGameMgr.h>
#include <Scripts/CPlayerMgr.h>
#include <Scripts/CPlayerScript_Test.h>
#include <Scripts/CMissileScript_Test.h>
#include <Scripts/CMonsterScript_Test.h>
#include <Scripts/CPlayerScript.h>
#include <Scripts/CWallScript.h>

#include <States/CIdleState.h>
#include <States/CTraceState.h>
#include <States/CPlayerIdle.h>
#include <States/CPlayerIdleToRun.h>
#include <States/CPlayerRun.h>
#include <States/CPlayerRunToIdle.h>
#include <States/CPlayerRunUTurn.h>
#include <States/CPlayerIdleUTurn.h>
#include <States/CPlayerJumpStart.h>
#include <States/CPlayerJumpFalling.h>
#include <States/CPlayerJumpLanding.h>
#include <States/CPlayerDash.h>
#include <States/CPlayerConcentrate.h>
#include <States/CPlayerConcentrate_Start.h>
#include <States/CPlayerPowerUp.h>
#include <States/CPlayerHit.h>
#include <States/CPlayerDie.h>
#include <States/CScytheDissComboA.h>
#include <States/CScytheDissComboB.h>
#include <States/CScytheDissComboC.h>
#include <States/CScytheDissComboD.h>
#include <States/CScytheDissAerial.h>
#include <States/CScytheDissSpecial.h>
#include <States/CScytheDissUp.h>
#include <States/CScytheDissCrush.h>
#include <States/CScytheDissRest.h>

void CCreateTempLevel::Init()
{
	////Missile Prefab 생성
	//CGameObject* pObj = nullptr;
	//pObj = new CGameObject;
	//pObj->SetName(L"Missile");
	//pObj->AddComponent(new CTransform);
	//pObj->AddComponent(new CMeshRender);
	////pObj->AddComponent(new CMissileScript);
	//pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));
	//pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMat"));

	//Ptr<CPrefab> pMissilePrefab = new CPrefab(pObj, false);
	//CAssetMgr::GetInst()->AddAsset<CPrefab>(L"MissilePrefab", pMissilePrefab.Get());
	//pMissilePrefab->Save(L"prefab\\missile.pref");

	//임시 FSM (AI Asset) 객체 생성
	Ptr<CFSM>	pFSM = new CFSM(nullptr, false);
	pFSM->AddState(L"IdleState", new CIdleState);
	pFSM->AddState(L"TraceState", new CTraceState);
	CAssetMgr::GetInst()->AddAsset<CFSM>(L"NormalMonsterFSM", pFSM.Get());

	pFSM = new CFSM(nullptr, false);
	pFSM->AddState(L"Idle", new CPlayerIdle);
	pFSM->AddState(L"Idle_ToRun", new CPlayerIdleToRun);
	pFSM->AddState(L"Idle_UTurn", new CPlayerIdleUTurn);
	pFSM->AddState(L"Run", new CPlayerRun);
	pFSM->AddState(L"Run_ToIdle", new CPlayerRunToIdle);
	pFSM->AddState(L"Run_UTurn", new CPlayerRunUTurn);
	pFSM->AddState(L"Jump_Start", new CPlayerJumpStart);
	pFSM->AddState(L"Jump_Falling", new CPlayerJumpFalling);
	pFSM->AddState(L"Jump_Landing", new CPlayerJumpLanding);
	pFSM->AddState(L"Dash", new CPlayerDash);
	pFSM->AddState(L"Concentrate", new CPlayerConcentrate);
	pFSM->AddState(L"Concentrate_Start", new CPlayerConcentrate_Start);
	pFSM->AddState(L"PowerUp", new CPlayerPowerUp);
	pFSM->AddState(L"Hit", new CPlayerHit);
	pFSM->AddState(L"Die", new CPlayerDie);
	pFSM->AddState(L"ScytheDiss_ComboA", new CScytheDissComboA);
	pFSM->AddState(L"ScytheDiss_ComboB", new CScytheDissComboB);
	pFSM->AddState(L"ScytheDiss_ComboC", new CScytheDissComboC);
	pFSM->AddState(L"ScytheDiss_ComboD", new CScytheDissComboD);
	pFSM->AddState(L"ScytheDiss_Aerial", new CScytheDissAerial);
	pFSM->AddState(L"ScytheDiss_Special", new CScytheDissSpecial);
	pFSM->AddState(L"ScytheDiss_Up", new CScytheDissUp);
	pFSM->AddState(L"ScytheDiss_Crush", new CScytheDissCrush);
	pFSM->AddState(L"ScytheDiss_Rest", new CScytheDissRest);
	CAssetMgr::GetInst()->AddAsset<CFSM>(L"FSM\\PlayerFSM.fsm", pFSM.Get());

	//pFSM = new CFSM(nullptr, false);
	//pFSM->AddState(L"Idle", new CMonsterIdle);
	//pFSM->AddState(L"Attack2", new CKoTBigAttack2);
	//CAssetMgr::GetInst()->AddAsset<CFSM>(L"FSM\\KoTBigFSM.fsm", pFSM.Get());

	// -----------------------------------------------FSM CODEGEN TEST
	//CFSM* pFSM = new CFSM(nullptr, true);
	//pFSM->AddState(L"IdleState", new CIdleState);
	////pFSM->AddState(L"TraceState", new CTraceState);

	//wchar_t szPath[255] = {};
	//wstring FilePath = CPathMgr::GetContentPath();
	//swprintf_s(szPath, L"FSM//TestFSM.fsm");
	//pFSM->SetName(szPath);
	//pFSM->Save(szPath);
	//CAssetMgr::GetInst()->AddAsset<CFSM>(szPath, pFSM.Get());
}

void CCreateTempLevel::CreateTempLevel()
{	
	// @TODO 삭제 - Material Load
	// CAssetMgr::GetInst()->Load<CMaterial>(L"TestMat", L"material\\testStd2D.mat");

	//Ptr<CMaterial> pBackgroudMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BackgroundMat");
	//Ptr<CMaterial> pStd2DMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMat");

	//pBackgroudMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Background.png"));
	//pStd2DMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Fighter.bmp"));

	// @TODO 삭제 - Level Load
	CLevel* pLevel = CLevelSaveLoad::LoadLevel(L"level\\WorkSpace.lv");
	CLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::STOP);
	CCollisionMgr::GetInst()->LayerCheck(3, 4);
	CCollisionMgr::GetInst()->LayerCheck(3, 6);
	CCollisionMgr::GetInst()->LayerCheck(4, 6);
	return;

	CCreateTempLevel::Init();

	CLevel* pTempLevel = new CLevel;
	pTempLevel->GetLayer(0)->SetName(L"Default");
	pTempLevel->GetLayer(1)->SetName(L"Background");
	pTempLevel->GetLayer(2)->SetName(L"Tile");
	pTempLevel->GetLayer(3)->SetName(L"Player");
	pTempLevel->GetLayer(4)->SetName(L"Monster");
	pTempLevel->GetLayer(5)->SetName(L"Light");
	pTempLevel->GetLayer(6)->SetName(L"Platform");
	pTempLevel->GetLayer(31)->SetName(L"UI");

	// Computer Shader Test
	/*Ptr<CTexture> pTestTex = CAssetMgr::GetInst()->CreateTexture(L"texture\\TestTex.png", 1600, 900
		, DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
	Ptr<CSetColorShader> pCS = (CSetColorShader*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"SetColorShader").Get();
	pCS->SetColor(Vec3(1.f, 1.f, 1.f));
	pCS->SetTargetTexture(pTestTex);
	pCS->Execute();

	FPixel* pPixel = pTestTex->GetPixels();
	FPixel pixel = pPixel[0];*/

	// set collision
	//CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster"); //이름으로 검색시 curLevel 없어서 오류ㅋ
	//CCollisionMgr::GetInst()->LayerCheck(L"Monster", L"Monster");
	CCollisionMgr::GetInst()->LayerCheck(3, 4);
	CCollisionMgr::GetInst()->LayerCheck(4, 4);
	CCollisionMgr::GetInst()->LayerCheck(3, 6);
	CCollisionMgr::GetInst()->LayerCheck(4, 6);

	// Create Main Camera
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	//pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetCameraPriority(0);
	pCamObj->Camera()->LayerCheckAll();
	pCamObj->Camera()->LayerCheck(31, false);
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	pTempLevel->AddObject(pCamObj, L"Default", false);

	// Create UI Camera
	pCamObj = new CGameObject;
	pCamObj->SetName(L"UICamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3());

	pCamObj->Camera()->SetCameraPriority(1);
	pCamObj->Camera()->LayerCheck(31, true);
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	pTempLevel->AddObject(pCamObj, L"Default", false);

	// Create Light
	CGameObject* pLight = new CGameObject;
	pLight->SetName(L"Light");
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CMeshRender);
	pLight->AddComponent(new CLight2D);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight->Light2D()->SetLightColor(Vec3(1.f));
	pLight->Light2D()->SetAmbient(Vec3(1.f));

	pLight->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
	pTempLevel->AddObject(pLight, L"Light");

	// Light2
	/*pLight = new CGameObject;
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CMeshRender);
	pLight->AddComponent(new CLight2D);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pLight->Light2D()->SetLightColor(Vec3(1.f, 0.3f, 0.3f));
	pLight->Light2D()->SetRadius(300.f);

	pLight->Transform()->SetRelativePos(Vec3(-200.f, 0.f, 200.f));
	m_CurLevel->AddObject(pLight, L"Light");*/

	// Light3
	/*pLight = new CGameObject;
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CMeshRender);
	pLight->AddComponent(new CLight2D);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pLight->Light2D()->SetLightColor(Vec3(0.3f, 1.f, 0.3f));
	pLight->Light2D()->SetRadius(300.f);

	pLight->Transform()->SetRelativePos(Vec3(200.f, 0.f, 200.f));
	m_CurLevel->AddObject(pLight, L"Light");*/

	// Light4
	/*pLight = new CGameObject;
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CMeshRender);
	pLight->AddComponent(new CLight2D);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	pLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pLight->Light2D()->SetAngle(XM_PI / 4);	// 부채꼴 각 설정
	pLight->Light2D()->SetRadius(500.f);

	pLight->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
	pLight->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, XM_PI / 2.f));
	m_CurLevel->AddObject(pLight, L"Light");*/

	// Create Background
	CGameObject* pObj = new CGameObject;
	Ptr<CTexture> pTex = nullptr;

	pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Background.png");
	pObj->SetName(L"Background");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 300.f));
	pObj->Transform()->SetRelativeScale(Vec3(1600.f, 900.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BackgroundMat"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);

	pTempLevel->AddObject(pObj, L"Background", false);

	// Create Tilemap
	/*pObj = new CGameObject();
	pObj->SetName(L"TileMap");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CTileMap);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 299.f));

	Ptr<CTexture> pTileAtlas = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\TILE.bmp");
	pObj->TileMap()->SetAtlas(pTileAtlas, Vec2(64.f, 64.f));
	pObj->TileMap()->SetMap(1, 1);
	pObj->TileMap()->SetTileImgIdx(0, 0, 0);

	m_CurLevel->AddObject(pObj, L"Tile", false);*/


	CGameObject* pMgrObj = new CGameObject;
	pMgrObj->SetName(L"Manager");
	pMgrObj->AddComponent(new CTransform);
	pMgrObj->AddComponent(new CPlayerMgr);
	pMgrObj->AddComponent(new CGameMgr);
	pTempLevel->AddObject(pMgrObj, L"Default", false);

	// Create Player
	{
		//pObj = new CGameObject;
		//pObj->SetName(L"Player");
		//pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\Fighter.bmp");

		//pObj->AddComponent(new CTransform);
		//pObj->AddComponent(new CMeshRender);
		//pObj->AddComponent(new CPlayerScript_Test);
		//pObj->AddComponent(new CCollider2D);
		//pObj->AddComponent(new CAnimator2D);

		//pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
		//pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

		//pObj->Collider2D()->SetAbsolute(false);
		//pObj->Collider2D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
		//pObj->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));

		//pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		//pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMat"));
		//pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
		//pObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 1);


		// child
		///*CGameObject* pChild = new CGameObject;
		//pChild->SetName(L"Child");

		//pChild->AddComponent(new CTransform);
		//pChild->AddComponent(new CMeshRender);

		//pChild->Transform()->SetAbsolute(true);
		//pChild->Transform()->SetRelativePos(Vec3(100.f, 0.f, 0.f));
		//pChild->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));

		//pChild->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		//pChild->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMat"));

		//pObj->AddChild(pChild);*/
		//pTempLevel->AddObject(pObj, L"Player", false);

		//pObj = pObj->Clone();
		//pObj->SetName(L"PlayerClone");
		//pObj->Transform()->SetRelativePos(Vec3(-300.f, 0.f, 100.f));
		//pTempLevel->AddObject(pObj, L"Player", false);
	}
	pObj = new CGameObject;
	pObj->SetName(L"Death");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CAnimator2D);
	pObj->AddComponent(new CStateMachine);
	pObj->AddComponent(new CMovement);
	pObj->AddComponent(new CPlayerScript);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
	pObj->StateMachine()->SetFSM(CAssetMgr::GetInst()->FindAsset<CFSM>(L"FSM\\PlayerFSM.fsm"));
	pObj->GetScriptByType<CPlayerScript>()->init();

	//pMgrObj->GetScriptByType<CPlayerMgr>()->SetPlayer(pObj);
	pTempLevel->AddObject(pObj, L"Player", false);

	pObj = new CGameObject;
	pObj->SetName(L"KoTBig");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CAnimator2D);
	//pObj->AddComponent(new CStateMachine);
	pObj->AddComponent(new CMovement);
	//pObj->AddComponent(new CKoTBigScript);

	//pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	//pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));
	//pObj->StateMachine()->SetFSM(CAssetMgr::GetInst()->FindAsset<CFSM>(L"FSM\\KoTBigFSM.fsm"));
	pTempLevel->AddObject(pObj, L"Monster", false);


	// Platform
	pObj = new CGameObject;
	pObj->SetName(L"Platform2");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CWallScript);
	pObj->Transform()->SetRelativePos(Vec3(0.f, -160.f, 100.f));
	pObj->Transform()->SetRelativeScale(Vec3(500.f, 100.f, 1.f));
	pObj->GetScriptByType<CWallScript>()->SetSideCollision(true);
	pTempLevel->AddObject(pObj, L"Platform", false);

	// Platform
	pObj = new CGameObject;
	pObj->SetName(L"Platform");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CWallScript);
	pObj->Transform()->SetRelativePos(Vec3(0.f, -500.f, 100.f));
	pObj->Transform()->SetRelativeScale(Vec3(1500.f, 200.f, 1.f));
	pTempLevel->AddObject(pObj, L"Platform", false);

	// Monster
	pObj = new CGameObject;
	pObj->SetName(L"Monster");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CCollider2D);
	//pObj->AddComponent(new CStateMachine);
	//pObj->AddComponent(new CMonsterScript_Test);

	pObj->Transform()->SetRelativePos(Vec3(200.f, 0.f, 100.f));
	pObj->Transform()->SetRelativeScale(Vec3(600.f, 600.f, 1.f));

	pObj->Collider2D()->SetAbsolute(true);
	pObj->Collider2D()->SetOffsetScale(Vec3(50.f, 50.f, 1.f));
	pObj->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"EffetMat"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\test1.png"));

	//pObj->StateMachine()->SetFSM(CAssetMgr::GetInst()->FindAsset<CFSM>(L"NormalMonsterFSM"));

	//wchar_t szPath[255] = {};
	//wstring FilePath = CPathMgr::GetContentPath();
	//swprintf_s(szPath, L"FSM\\TestFSM.fsm");
	//pObj->StateMachine()->SetFSM(CAssetMgr::GetInst()->Load<CFSM>(szPath));

	pTempLevel->AddObject(pObj, L"Monster", false);

	// UI Object Create
	pObj = new CGameObject;
	pObj->SetName(L"UI");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(-590, 310.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));
	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UIMat"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);

	pTempLevel->AddObject(pObj, L"UI", false);

	// UI GameEnding Object
	/*pObj = new CGameObject;
	pObj->AddComponent(new CTransform);
	pObj->SetName(L"GameEnding_Fail");
	pObj->Transform()->SetRelativePos(Vec3(0, 0, 500.f));
	
	{
		CGameObject* pChildObj;

		pChildObj = new CGameObject();
		pChildObj->SetName(L"Background");
		pChildObj->AddComponent(new CTransform);
		pChildObj->AddComponent(new CMeshRender);
		pChildObj->Transform()->SetAbsolute(true);
		pChildObj->Transform()->SetRelativeScale(Vec3(3000, 2000, 0));
		pChildObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pChildObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UIMat"));
		Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\UI\\background_black.jpg");
		pChildObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
		pObj->AddChild(pChildObj);

		pChildObj = new CGameObject();
		pChildObj->SetName(L"Dead_Screen");
		pChildObj->AddComponent(new CTransform);
		pChildObj->AddComponent(new CMeshRender);
		pChildObj->AddComponent(new CAnimator2D);
		pChildObj->Transform()->SetAbsolute(true);
		pChildObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		pChildObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UIMat"));

		FILE* pFile = nullptr;
		CAnim* pAnim = new CAnim;
		_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\UI\\LD_Dead_Screen.anim").c_str(), L"rb");
		pAnim->LoadFromFile(pFile);
		pChildObj->Animator2D()->Create(pAnim, L"Dead_Screen");
		fclose(pFile);
		delete pAnim;

		pObj->AddChild(pChildObj);
	}

	pMgrObj->GetScriptByType<CGameMgr>()->SetGameEndingFail(pObj);
	pTempLevel->AddObject(pObj, L"UI", false);
	pObj->Deactivate();*/

	// PostProcess Object Create
	/*pObj = new CGameObject;
	pObj->SetName(L"Distortion");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
	pObj->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));

	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DistortionMat"));
	pObj->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"texture\\noise\\noise_03.jpg"));

	m_CurLevel->AddObject(pObj, L"Default", false);*/

	// Particle object
	pObj = new CGameObject;
	pObj->SetName(L"Particle");

	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CParticleSystem);
	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 99.f));

	pTempLevel->AddObject(pObj, L"Default", false);

	// @TODO 터지면 여기임
	CLevelMgr::GetInst()->ChangeLevel(pTempLevel, LEVEL_STATE::STOP);
	CLevelSaveLoad::SaveLevel(pTempLevel, L"level\\temp.lv");
}

void CCreateTempLevel::CreateGameEndig_Fail()
{
	CLevel* pEndingLevel = new CLevel;
	CGameObject* pObj;

	// -------------------------------------------
	// Layer
	// -------------------------------------------
	pEndingLevel->GetLayer(0)->SetName(L"Default");
	pEndingLevel->GetLayer(1)->SetName(L"Background");
	pEndingLevel->GetLayer(2)->SetName(L"Tile");
	pEndingLevel->GetLayer(3)->SetName(L"Player");
	pEndingLevel->GetLayer(4)->SetName(L"Monster");
	pEndingLevel->GetLayer(5)->SetName(L"Light");
	pEndingLevel->GetLayer(6)->SetName(L"Platform");
	pEndingLevel->GetLayer(31)->SetName(L"UI");

	// -------------------------------------------
	// Camera
	// -------------------------------------------
	
	// Create Main Camera
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	//pCamObj->AddComponent(new CCameraMoveScript);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));

	pCamObj->Camera()->SetCameraPriority(0);
	pCamObj->Camera()->LayerCheckAll();
	pCamObj->Camera()->LayerCheck(31, false);
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	pEndingLevel->AddObject(pCamObj, L"Default", false);

	// Create UI Camera
	pCamObj = new CGameObject;
	pCamObj->SetName(L"UICamera");
	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);

	pCamObj->Transform()->SetRelativePos(Vec3(0.5f, 0.f, 0.f));
	pCamObj->Transform()->SetRelativeRotation(Vec3());

	pCamObj->Camera()->SetCameraPriority(1);
	pCamObj->Camera()->LayerCheck(31, true);
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	pEndingLevel->AddObject(pCamObj, L"Default", false);

	// -------------------------------------------
	// Create Light
	// -------------------------------------------
	CGameObject* pLight = new CGameObject;
	pLight->SetName(L"Light");
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CMeshRender);
	pLight->AddComponent(new CLight2D);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLight->Light2D()->SetLightColor(Vec3(1.f));
	pLight->Light2D()->SetAmbient(Vec3(1.f));

	pLight->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
	pEndingLevel->AddObject(pLight, L"Light");

	// Light2
	pLight = new CGameObject;
	pLight->SetName(L"PointLight");
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CMeshRender);
	pLight->AddComponent(new CLight2D);

	pLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pLight->Light2D()->SetLightColor(Vec3(0.15f, 0.15f, 0.15f));
	pLight->Light2D()->SetRadius(800.f);

	pLight->Transform()->SetRelativePos(Vec3(0.f, 400.f, 200.f));
	pEndingLevel->AddObject(pLight, L"Light");

	// -------------------------------------------
	// Create objects
	// -------------------------------------------
	pObj = new CGameObject();
	pObj->SetName(L"Background");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);

	pObj->Transform()->SetAbsolute(true);
	pObj->Transform()->SetRelativeScale(Vec3(3000, 2000, 0));
	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\GameEnd_Background.mat"));

	pEndingLevel->AddObject(pObj, L"UI");

	pObj = new CGameObject();
	pObj->SetName(L"Dead_Screen");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CMeshRender);
	pObj->AddComponent(new CAnimator2D);

	pObj->Transform()->SetAbsolute(true);
	pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UIMat"));

	FILE* pFile = nullptr;
	CAnim* pAnim = new CAnim;
	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\UI\\LD_Dead_Screen.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	pObj->Animator2D()->Create(pAnim, L"Dead_Screen");
	fclose(pFile);
	delete pAnim;

	pObj->Animator2D()->Play(L"Dead_Screen", false);
	pEndingLevel->AddObject(pObj, L"UI");

	// -------------------------------------------
	// Save Level
	// -------------------------------------------
	CLevelMgr::GetInst()->ChangeLevel(pEndingLevel, LEVEL_STATE::STOP);
	CLevelSaveLoad::SaveLevel(pEndingLevel, L"level\\Ending_tmp.lv");
}