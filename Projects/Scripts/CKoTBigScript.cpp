#include "pch.h"
#include "CKoTBigScript.h"
#include "CPlayerMgr.h"
#include "CHitBoxScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CTaskMgr.h>
#include <Engine/CStateMachine.h>
#include <Engine/components.h>

CKoTBigScript::CKoTBigScript()
	: CUnitScript(KOTBIGSCRIPT)
	, m_fDetectRange(400.f)
	, m_iAttack2Damage(10.f)
{
}

CKoTBigScript::~CKoTBigScript()
{
}

void CKoTBigScript::begin()
{	
	// components
	Collider2D()->SetAbsolute(true);
	Collider2D()->SetOffsetScale(Vec3(167.f, 180.f, 1.f));

	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMat"));
	MeshRender()->GetDynamicMaterial();

	Movement()->UseGravity(true);

	// animation
	FILE* pFile = nullptr;
	CAnim* pAnim = new CAnim;

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\Knight_Of_Time_Big\\Knight_Of_Time_Big_Idle.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Idle");
	fclose(pFile);

	_wfopen_s(&pFile, (CPathMgr::GetContentPath() + (wstring)L"animation\\Knight_Of_Time_Big\\Knight_Of_Time_Big_Attack02.anim").c_str(), L"rb");
	pAnim->LoadFromFile(pFile);
	Animator2D()->Create(pAnim, L"Attack2");
	fclose(pFile);

	delete pAnim;

	// FSM
	if (StateMachine())
	{
		// blackboard
		StateMachine()->AddBlackboardData(L"fDetectRange", BB_DATA::FLOAT, &m_fDetectRange);
		StateMachine()->AddBlackboardData(L"fAttack2_Damage", BB_DATA::FLOAT, &m_iAttack2Damage);
		CGameObject* pPlayer = CPlayerMgr::GetPlayer();

		if (pPlayer)
			StateMachine()->AddBlackboardData(L"pTarget", BB_DATA::OBJECT, pPlayer);

		// begin state
		if (StateMachine()->GetFSM().Get())
			StateMachine()->GetFSM()->ChangeState(L"Idle");
	}

	// Create Child
	CGameObject* pHitbox = new CGameObject;
	pHitbox->SetName(L"Attack2_Hitbox");
	pHitbox->AddComponent(new CTransform);
	pHitbox->AddComponent(new CCollider2D);
	pHitbox->AddComponent(new CHitboxScript);

	pHitbox->Transform()->SetAbsolute(true);
	pHitbox->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pHitbox->Transform()->SetRelativeScale(Vec3(550.f, 150.f, 1.f));

	CHitboxScript* pHitboxComp = pHitbox->GetScriptByType<CHitboxScript>();
	pHitboxComp->SetDamage(FDamage{10, 0});
	pHitboxComp->SetHostileLayer(3);
	pHitboxComp->SetRepeat(true);
	pHitboxComp->SetCoolTime(5.f);

	GetOwner()->AddChild(pHitbox);
	//pHitbox->Collider2D()->Deactivate();
	pHitbox->Deactivate();

	FTask task = {};
	task.Type = TASK_TYPE::CREATE_OBJECT; // Param1: Layer Index | Param2: Object Address
	task.Param_1 = GetOwner()->GetLayerIdx();
	task.Param_2 = (DWORD_PTR)pHitbox;
	CTaskMgr::GetInst()->AddTask(task);

	task = {};
	task.Type = TASK_TYPE::ADD_CHILD; // // Param1: Parent Object | Param2: Child Object
	task.Param_1 = (DWORD_PTR)GetOwner();
	task.Param_2 = (DWORD_PTR)pHitbox;
	CTaskMgr::GetInst()->AddTask(task);
}

void CKoTBigScript::tick()
{
}

void CKoTBigScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CKoTBigScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CKoTBigScript::SaveToFile(FILE* _File)
{
	fwrite(&m_fDetectRange, sizeof(float), 1, _File);
	fwrite(&m_iAttack2Damage, sizeof(float), 1, _File);
}

void CKoTBigScript::LoadFromFile(FILE* _File)
{
	fread(&m_fDetectRange, sizeof(float), 1, _File);
	fread(&m_iAttack2Damage, sizeof(float), 1, _File);
}