#include "pch.h"
#include "CKoTSpearAttack1.h"

#include <Engine\CAssetMgr.h>
#include <Engine\CTransform.h>

#include <Scripts\CMonsterScript.h>
#include <Scripts\CKoTSpear_ProjScript.h>

CKoTSpearAttack1::CKoTSpearAttack1()
	: CState(KOTSPEARATTACK1)
	, m_SoundPlay{false,}
	, m_bThrow(false)
{
	m_pProj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"KoTSpear_Proj");
	m_pProj = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KoTSpear_Proj.pref");
}

CKoTSpearAttack1::~CKoTSpearAttack1()
{
}

void CKoTSpearAttack1::finaltick()
{
	int CurFrame = GetOwner()->Animator2D()->GetCurAnimFrmIdx();

	if (CurFrame == 16 && !m_bThrow)
	{
		// @TODO 터지면 여기... bThrow값을 공유하고 있음
		InstanceProj();
		m_bThrow = true;

		if (!m_SoundPlay[1])
		{
			GamePlayStatic::Play2DSound(L"sound\\npc_time_spear\\NPC_TimeSpear_Atk_Throw_Whsh_01.wav", 1, 0.25f);
			m_SoundPlay[1] = true;
		}
	}
	else if (CurFrame == 16)
	{
		if (!m_SoundPlay[0])
		{
			GamePlayStatic::Play2DSound(L"sound\\npc_time_spear\\NPC_TimeSpear_Vo_Atk_Lgt_03.wav", 1, 0.25f);
			m_SoundPlay[0] = true;
		}
	}

	// playing anim
	if (GetOwner()->Animator2D()->IsPlaying())
	{
		return;
	}

	// change state
	ChangeState(L"Idle");
}

void CKoTSpearAttack1::Enter()
{
	GetOwner()->Animator2D()->Play(L"Attack1", false);

	for (bool& iter : m_SoundPlay)
		iter = false;
}

void CKoTSpearAttack1::Exit()
{
	MONSTERSCRIPT->StartAttackCool();
	m_bThrow = false;
}

void CKoTSpearAttack1::InstanceProj()
{
	//(Ptr<CPrefab> _Prefab, Vec3 _vWorldPos, int _layerIdx)
	if (not m_pProj.Get())
		return;

	Vec3 SpawnPos = GetOwner()->Transform()->GetWorldPos();
	CGameObject* pNewObj = m_pProj->Instantiate();
	CKoTSpear_ProjScript* pProjScript = pNewObj->GetScriptByType<CKoTSpear_ProjScript>();

	if (MONSTERSCRIPT->GetDir() == UNIT_DIRX::LEFT)
	{
		SpawnPos.x -= GetOwner()->Transform()->GetRelativeScale().x / 2.f;
		pProjScript->SetDir(UNIT_DIRX::LEFT);
	}
	else
	{
		SpawnPos.x += GetOwner()->Transform()->GetRelativeScale().x / 2.f;
		pProjScript->SetDir(UNIT_DIRX::RIGHT);
	}

	pNewObj->Transform()->SetRelativePos(SpawnPos);
	GamePlayStatic::SpawnGameObject(pNewObj, 4);
}
