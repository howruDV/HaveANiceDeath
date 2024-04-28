#include "pch.h"
#include "CTaskMgr.h"
#include "CGameObject.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CAssetMgr.h"
#include "CRenderMgr.h"

#include "CCollider2D.h"
#include "CStateMachine.h"

CTaskMgr::CTaskMgr()
	: m_bCreateObject(false)
	, m_bDeleteObject(false)
	, m_bAssetChange(false)
	, m_DeleteFrameCount(0)
{

}

CTaskMgr::~CTaskMgr()
{

}

void CTaskMgr::tick()
{
	Clear();

	for (int i = 0; i < m_vecTask.size(); ++i)
	{
		switch (m_vecTask[i].Type)
		{
		case TASK_TYPE::CREATE_OBJECT:
		{
			// params
			int layerIdx = (int)m_vecTask[i].Param_1;
			CGameObject* pObj = (CGameObject*)m_vecTask[i].Param_2;

			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			pCurLevel->AddObject(pObj, layerIdx, false);
			m_bCreateObject = true;

			// @TODO 에러나면 여기
			if (pCurLevel->GetState() == LEVEL_STATE::PLAY)
				pObj->begin();
		}
			break;

		case TASK_TYPE::DELETE_OBJECT:
		{
			// params
			CGameObject* pDeadObj = (CGameObject*)m_vecTask[i].Param_1;

			list<CGameObject*>queue{ pDeadObj };
			while (!queue.empty())
			{
				CGameObject* pObj = queue.front();
				queue.pop_front();

				pObj->m_bDead = true;
				for (size_t i = 0; i < pObj->m_vecChild.size(); ++i)
					queue.push_back(pObj->m_vecChild[i]);	
			}

			if (m_DeleteFrameCount == 0)
				++m_DeleteFrameCount;
			else if (m_DeleteFrameCount == 2)
				m_DeleteFrameCount = 1;
		}
			break;

		case TASK_TYPE::ADD_ASSET:
		{
			CAsset* pAsset = (CAsset*)m_vecTask[i].Param_1;
			CAssetMgr::GetInst()->AddAsset(pAsset->GetName(), pAsset);
			m_bAssetChange = true;
		}
		break;

		case TASK_TYPE::DELETE_ASSET:
		{
			// Param1 : AssetType, Param2 : Asset Adress
			ASSET_TYPE Type = (ASSET_TYPE)m_vecTask[i].Param_1;
			CAsset* pAsset = (CAsset*)m_vecTask[i].Param_2;
			CAssetMgr::GetInst()->DeleteAsset(Type, pAsset->GetKey());
			m_bAssetChange = true;
		}

		break;

		case TASK_TYPE::CHANGE_LEVELSTATE:
		{
			CLevel* pLevel = (CLevel*)m_vecTask[i].Param_1;
			LEVEL_STATE NextState = (LEVEL_STATE)m_vecTask[i].Param_2;

			pLevel->ChangeState(NextState);
		}
		break;

		case TASK_TYPE::CHANGE_LEVEL:
		{
			CLevel* pNextLevel = (CLevel*)m_vecTask[i].Param_1;
			LEVEL_STATE State = (LEVEL_STATE)m_vecTask[i].Param_2;
			CRenderMgr::GetInst()->ClearCamera();
			CLevelMgr::GetInst()->ChangeLevel_proc(pNextLevel, State);
			m_bCreateObject = true;
			m_bChangeLevel = true;
		}
		break;

		case TASK_TYPE::ADD_CHILD:
		{
			CGameObject* pParent = (CGameObject*)m_vecTask[i].Param_1;
			CGameObject* pChild = (CGameObject*)m_vecTask[i].Param_2;
			pParent->AddChild(pChild);
			m_bCreateObject = true;
		}
			break;
		
		case TASK_TYPE::DISCONNECT_PARENT:
			break;

		case TASK_TYPE::COLLIDER2D_SEMI_DEACTIVE:
		{
			CCollider2D* pCollider = (CCollider2D*)m_vecTask[i].Param_1;
			pCollider->m_SemiDeactive = true;
		}
		break;

		case TASK_TYPE::COLLIDER2D_DEACTIVE:
		{
			CCollider2D* pCollider = (CCollider2D*)m_vecTask[i].Param_1;
			pCollider->m_SemiDeactive = false;
			pCollider->m_Active = false;
		}
		break;

		case TASK_TYPE::CHANGE_STATE:
		{
			// Param1: Parent Object    |   Param2: Next State
			CGameObject* pObj = (CGameObject*)m_vecTask[i].Param_1;
			CState* pNextState = (CState*)m_vecTask[i].Param_2;
			pObj->StateMachine()->GetFSM()->ChangeState_proc(pNextState);
		}

		default:
			break;
		}
	}

	m_vecTask.clear();
}

void CTaskMgr::Clear()
{
	m_bChangeLevel = false;
	m_bCreateObject = false;

	// 억지
	if (1 == m_DeleteFrameCount)
	{
		++m_DeleteFrameCount;
		m_bDeleteObject = true;
	}
	else if (2 <= m_DeleteFrameCount)
	{
		m_DeleteFrameCount = 0;
		m_bDeleteObject = false;
	}

	m_bAssetChange = false;
}