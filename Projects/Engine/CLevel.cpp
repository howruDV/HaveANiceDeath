#include "pch.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CTimeMgr.h"
#include "CRenderMgr.h"

CLevel::CLevel()
	: m_arrLayer{}
	, m_State(LEVEL_STATE::NONE)
{
	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		m_arrLayer[i] = new CLayer;
		m_arrLayer[i]->m_iLayerIdx = i;
	}
}

CLevel::CLevel(const CLevel& _OriginLevel)
	: CEntity(_OriginLevel)
	, m_arrLayer{}
	, m_State(_OriginLevel.m_State)
{
	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		m_arrLayer[i] = _OriginLevel.m_arrLayer[i]->Clone();
	}
}

CLevel::~CLevel()
{
	Delete_Array(m_arrLayer);
}

void CLevel::clear()
{
	for (UINT i = 0; i < LAYER_MAX; ++i)
		m_arrLayer[i]->m_vecObjects.clear();
}

void CLevel::begin()
{
	for (size_t i = 0; i < LAYER_MAX; ++i)
		m_arrLayer[i]->begin();
}

void CLevel::tick()
{
	for (size_t i = 0; i < LAYER_MAX; ++i)
		m_arrLayer[i]->tick();
}

void CLevel::finaltick()
{
	for (size_t i = 0; i < LAYER_MAX; ++i)
		m_arrLayer[i]->finaltick();
}

void CLevel::AddObject(CGameObject* _Object, int _LayerIdx, bool _bChildMove)
{
	m_arrLayer[_LayerIdx]->AddObject(_Object, _bChildMove);
}

void CLevel::AddObject(CGameObject* _Object, const wstring& _LayerName, bool _bChildMove)
{
	CLayer* pLayer = GetLayer(_LayerName);
	if (!pLayer)
		return;

	pLayer->AddObject(_Object, _bChildMove);
}

void CLevel::ChangeState(LEVEL_STATE _NextState)
{
	if (m_State == _NextState)
		return;

	// case: 에디터 -> 게임 (정지 -> 플레이)
	if ((m_State == LEVEL_STATE::STOP || m_State == LEVEL_STATE::PAUSE || m_State == LEVEL_STATE::NONE)
		&& _NextState == LEVEL_STATE::PLAY)
	{
		CTimeMgr::GetInst()->PauseTime(false);
		CRenderMgr::GetInst()->ActiveEditorMode(false); // 레벨 카메라

		if (LEVEL_STATE::STOP == m_State || LEVEL_STATE::NONE == m_State)
		{
			m_State = _NextState;
			begin();
		}
	}

	// case: 게임 -> 에디터 (플레이 -> 정지)
	else if ((m_State == LEVEL_STATE::PLAY || m_State == LEVEL_STATE::NONE ) &&
		(_NextState == LEVEL_STATE::STOP || _NextState == LEVEL_STATE::PAUSE || _NextState == LEVEL_STATE::NONE))
	{
		CTimeMgr::GetInst()->PauseTime(true);
		CRenderMgr::GetInst()->ActiveEditorMode(true); // 에디터 카메라
	}

	m_State = _NextState;
}

CLayer* CLevel::GetLayer(const wstring& _strLayerName)
{
	for (int i = 0; i < LAYER_MAX; ++i)
	{
		if (m_arrLayer[i]->GetName() == _strLayerName)
			return m_arrLayer[i];
	}
	return nullptr;
}

CGameObject* CLevel::FindObjectByName(const wstring& _strName)
{
	for (size_t i = 0; i < LAYER_MAX; ++i)
	{
		const vector<CGameObject*>& vecObject = m_arrLayer[i]->GetLayerObjects();
		for (size_t j = 0; j < vecObject.size(); ++j)
		{
			if (vecObject[j]->GetName() == _strName)
				return vecObject[j];
		}
	}
	return nullptr;
}

void CLevel::FindObjectsByName(const wstring& _strName, vector<CGameObject*>& _out)
{
	for (size_t i = 0; i < LAYER_MAX; ++i)
	{
		const vector<CGameObject*>& vecObject = m_arrLayer[i]->GetLayerObjects();
		for (size_t j = 0; j < vecObject.size(); ++j)
		{
			if (vecObject[j]->GetName() == _strName)
				_out.push_back(vecObject[j]);
		}
	}
}
