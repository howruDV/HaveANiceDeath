#include "pch.h"
#include "CElevatorScript.h"
#include "CPlayerMgr.h"

#include <Engine\CKeyMgr.h>
#include <Engine\CAnimator2D.h>
#include <Engine\CTransform.h>

CElevatorScript::CElevatorScript()
	: CScript(ELEVATORSCRIPT)
	, m_bStartOpen(true)
{
	AddScriptParam(SCRIPT_PARAM::BOOL, "Start Open", &m_bStartOpen);
}

CElevatorScript::CElevatorScript(const CElevatorScript& _Origin)
	: CScript(ELEVATORSCRIPT)
	, m_bStartOpen(_Origin.m_bStartOpen)
{
	AddScriptParam(SCRIPT_PARAM::BOOL, "Start Open", &m_bStartOpen);
}

CElevatorScript::~CElevatorScript()
{
}

void CElevatorScript::begin()
{
	if (m_bStartOpen)
		Open();
}

void CElevatorScript::tick()
{
	// sound
	if (!m_vecSound.empty())
	{
		static Ptr<CSound> Prev = nullptr;
		
		if (Prev == nullptr || !Prev->IsPlaying())
		{
			Prev = m_vecSound.front();
			Prev->Play(1, 0.25f);
			m_vecSound.pop_front();
		}
	}

	// interaction
	float fDist = (CPlayerMgr::GetPlayer()->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Length();
	if (fDist > 300.f)
		return;

	if (KEY_TAP(KEY::F))
	{
		// @TODO Level Control
		Close();
	}
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
