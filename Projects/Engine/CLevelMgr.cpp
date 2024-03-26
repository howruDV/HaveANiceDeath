#include "pch.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CAssetMgr.h"
#include "CTaskMgr.h"
#include "CTimeMgr.h"
#include "CCollisionMgr.h"
#include "CDevice.h"
#include "CLevel.h"
#include "CLayer.h"

CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{

}

CLevelMgr::~CLevelMgr()
{
	if (m_CurLevel)
		delete m_CurLevel;
}

void CLevelMgr::init()
{

}

void CLevelMgr::tick()
{
	if (!m_CurLevel)
		return;

	// ���� �����ӿ� ��ϵ� m_vecObjects clear (���� ���� ������ �� �����Ӹ��� ���� ���)
	m_CurLevel->clear(); 

	// ������ Play ������ ��쿡�� tick() ȣ��
	if (m_CurLevel->GetState() == LEVEL_STATE::PLAY)
		m_CurLevel->tick();
	m_CurLevel->finaltick();

	CCollisionMgr::GetInst()->tick();
	CRenderMgr::GetInst()->tick();
	CTimeMgr::GetInst()->render();

	CRenderMgr::GetInst()->CopyRenderTargetToImGuiRenderTexture();
}

// Usage: Level Change ��û (ó��: TaskMgr)
void CLevelMgr::ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextStartState)
{
	FTask task = {};

	task.Type = TASK_TYPE::CHANGE_LEVEL;
	task.Param_1 = (DWORD_PTR)_NextLevel;
	task.Param_2 = (DWORD_PTR)_NextStartState;

	CTaskMgr::GetInst()->AddTask(task);
}

void CLevelMgr::ChangeLevelState(LEVEL_STATE _State)
{
	FTask task = {};

	task.Type = TASK_TYPE::CHANGE_LEVELSTATE;
	task.Param_1 = (DWORD_PTR)m_CurLevel;
	task.Param_2 = (DWORD_PTR)_State;

	CTaskMgr::GetInst()->AddTask(task);
}

// Usage: Process Level Change
void CLevelMgr::ChangeLevel_proc(CLevel* _NextLevel, LEVEL_STATE _NextLevelState)
{
	assert(m_CurLevel != _NextLevel);

	if (m_CurLevel)
		delete m_CurLevel;

	m_CurLevel = _NextLevel;

	if (m_CurLevel)
		m_CurLevel->ChangeState(_NextLevelState);
}
