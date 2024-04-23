#include "pch.h"
#include "CElevatorScript.h"
#include "CPlayerMgr.h"

#include "CGameMgr.h"
#include "CCamCtrlScript.h"

#include <Engine\CKeyMgr.h>
#include <Engine\CAnimator2D.h>
#include <Engine\CTransform.h>
#include <Engine\CStateMachine.h>

CElevatorScript::CElevatorScript()
	: CScript(ELEVATORSCRIPT)
	, m_bCanClose(false)
{
	AddScriptParam(SCRIPT_PARAM::BOOL, "Can Close", &m_bCanClose);
}

CElevatorScript::CElevatorScript(const CElevatorScript& _Origin)
	: CScript(ELEVATORSCRIPT)
	, m_bCanClose(_Origin.m_bCanClose)
{
	AddScriptParam(SCRIPT_PARAM::BOOL, "Can Close", &m_bCanClose);
}

CElevatorScript::~CElevatorScript()
{
}

void CElevatorScript::begin()
{
	//if (m_bStartOpen)
	//	Open();

	//GetOwner()->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(0.1f, 0.2f, 0.1f, 1.f));
	//GetOwner()->GetRenderComponent()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.8f);

	StateMachine()->AddBlackboardData(L"bCanClose", BB_DATA::INT, &m_bCanClose);

	if (!m_bCanClose)
	{
		StateMachine()->GetFSM()->ChangeState(L"Appear");
	}
	else
	{
		StateMachine()->GetFSM()->ChangeState(L"Wait");
	}
}

void CElevatorScript::tick()
{
	//// sound
	//if (!m_vecSound.empty())
	//{
	//	static Ptr<CSound> Prev = nullptr;
	//	
	//	if (Prev == nullptr || !Prev->IsPlaying())
	//	{
	//		Prev = m_vecSound.front();
	//		Prev->Play(1, 0.25f);
	//		m_vecSound.pop_front();
	//	}
	//}

	//// interaction
	//float fDist = (CPlayerMgr::GetPlayer()->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Length();
	//if (fDist > 300.f)
	//	return;

	//if (m_bGameEnd && !Animator2D()->IsPlaying())
	//	CGameMgr::GetMainCamera()->GetScriptByType<CCamCtrlScript>()->PushTransition(true);

	//if (KEY_TAP(KEY::F))
	//{
	//	// @TODO Level Control
	//	Close();
	//	m_bGameEnd = true;
	//}
}

void CElevatorScript::SaveToFile(FILE* _File)
{
	//fwrite(&m_bStartOpen, sizeof(bool), 1, _File);
}

void CElevatorScript::LoadFromFile(FILE* _File)
{
	//fread(&m_bStartOpen, sizeof(bool), 1, _File);
}

void CElevatorScript::Open()
{
	Animator2D()->Play(L"Open", false);
	m_vecSound.push_back(CAssetMgr::GetInst()->Load<CSound>(L"sound\\obj\\elev\\GPE_Elevator_Open_Up_01.wav"));
	m_vecSound.push_back(CAssetMgr::GetInst()->Load<CSound>(L"sound\\obj\\elev\\GPE_Elevator_Open_Door_01.wav"));
}

void CElevatorScript::Close()
{
	Animator2D()->Play(L"Open", false, true);
	m_vecSound.push_back(CAssetMgr::GetInst()->Load<CSound>(L"sound\\obj\\elev\\GPE_Elevator_Close_Door_01.wav"));
	m_vecSound.push_back(CAssetMgr::GetInst()->Load<CSound>(L"sound\\obj\\elev\\GPE_Elevator_Close_Down_01.wav"));
}
