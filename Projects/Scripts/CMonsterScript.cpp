#include "pch.h"
#include "CMonsterScript.h"
#include "CPlayerMgr.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CStateMachine.h>

CMonsterScript::CMonsterScript()
	: CUnitScript(MONSTERSCRIPT)
	, m_fDetectRange(400.f)
	, m_fAttackRange(300.f)
	, m_iAttackTypeCount(1)
	, m_bAppear(false)
	, m_fAttackCoolTime(2.f)
	, m_fAttackCoolAcc(0.f)
	, m_bAttackCool(false)
	, m_bFlying(false)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Detect Range", &m_fDetectRange);
	AddScriptParam(SCRIPT_PARAM::INT, "Attack Type Count", &m_iAttackTypeCount);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Attack Range", &m_fAttackRange);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Attack CoolTime", &m_bAppear);
	AddScriptParam(SCRIPT_PARAM::BOOL, "Start With Spawn", &m_bAppear);
	AddScriptParam(SCRIPT_PARAM::BOOL, "Flying", &m_bFlying);
}

CMonsterScript::CMonsterScript(const CMonsterScript& _Origin)
	: CUnitScript(MONSTERSCRIPT)
	, m_fDetectRange(_Origin.m_fDetectRange)
	, m_fAttackRange(_Origin.m_fAttackRange)
	, m_iAttackTypeCount(_Origin.m_iAttackTypeCount)
	, m_bAppear(_Origin.m_bAppear)
	, m_fAttackCoolTime(_Origin.m_fAttackCoolTime)
	, m_fAttackCoolAcc(_Origin.m_fAttackCoolAcc)
	, m_bAttackCool(false)
	, m_bFlying(false)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Detect Range", &m_fDetectRange);
	AddScriptParam(SCRIPT_PARAM::INT, "Attack Type Count", &m_iAttackTypeCount);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Attack Range", &m_fAttackRange);
	AddScriptParam(SCRIPT_PARAM::INT, "Attack Type Count", &m_iAttackTypeCount);
	AddScriptParam(SCRIPT_PARAM::BOOL, "Start With Spawn", &m_bAppear);
	AddScriptParam(SCRIPT_PARAM::BOOL, "Flying", &m_bFlying);
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::begin()
{
	// component
	MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BloomMat"));
	MeshRender()->GetDynamicMaterial();
	MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, 0);
	MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(1.f, 1.f, 1.f, 1.f));
	MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.8f);

	Movement()->UseGravity(true);
	Movement()->SetInitSpeed(m_fSpeed);
	Movement()->SetInitSpeed_InAir(300.f);
	Movement()->SetMaxSpeed_Ground(m_fSpeed);
	Movement()->SetMaxSpeed_InAir(300.f);
	Movement()->SetGravityForce(Vec3(0.f, -4000.f, 0.f));

	// animation

	// FSM
	if (StateMachine())
	{
		// blackboard
		StateMachine()->AddBlackboardData(L"fSpeed", BB_DATA::FLOAT, &m_fSpeed);
		StateMachine()->AddBlackboardData(L"fDetectRange", BB_DATA::FLOAT, &m_fDetectRange);
		StateMachine()->AddBlackboardData(L"fAttackRange", BB_DATA::FLOAT, &m_fAttackRange);
		StateMachine()->AddBlackboardData(L"iAttackTypeCount", BB_DATA::INT, &m_iAttackTypeCount);
		StateMachine()->AddBlackboardData(L"bFlying", BB_DATA::INT, &m_bFlying);
		CGameObject* pPlayer = CPlayerMgr::GetPlayer();

		if (pPlayer)
			StateMachine()->AddBlackboardData(L"pTarget", BB_DATA::OBJECT, pPlayer);

		// begin state
		if (StateMachine()->GetFSM().Get())
		{
			if (m_bAppear)
				StateMachine()->GetFSM()->ChangeState(L"Appear_Stand");
			else
				StateMachine()->GetFSM()->ChangeState(L"Idle");
		}
	}
}

void CMonsterScript::tick()
{
	// ----------------------------
	// set direction
	// ----------------------------
	CUnitScript::tick();

	// ----------------------------
	// check state
	// ----------------------------
	if (m_bAttackCool)
	{
		m_fAttackCoolAcc += DT;

		if (m_fAttackCoolAcc >= m_fAttackCoolTime)
		{
			m_fAttackCoolAcc = 0.f;
			m_bAttackCool = false;
		}
	}
}

void CMonsterScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CMonsterScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)
{
}

void CMonsterScript::SaveToFile(FILE* _File)
{
	fwrite(&m_fDetectRange, sizeof(float), 1, _File);
	fwrite(&m_fAttackRange, sizeof(float), 1, _File);
	fwrite(&m_fAttackCoolTime, sizeof(float), 1, _File);
	fwrite(&m_iAttackTypeCount, sizeof(int), 1, _File);
	fwrite(&m_bAppear, sizeof(bool), 1, _File);
	fwrite(&m_bFlying, sizeof(bool), 1, _File);
}

void CMonsterScript::LoadFromFile(FILE* _File)
{
	fread(&m_fDetectRange, sizeof(float), 1, _File);
	fread(&m_fAttackRange, sizeof(float), 1, _File);
	fread(&m_fAttackCoolTime, sizeof(float), 1, _File);
	fread(&m_iAttackTypeCount, sizeof(int), 1, _File);
	fread(&m_bAppear, sizeof(bool), 1, _File);
	fread(&m_bFlying, sizeof(bool), 1, _File);
}