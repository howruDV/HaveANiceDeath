#include "pch.h"
#include "UIOutlinerPannel.h"
#include "UITree.h"
#include "UIInspectorPannel.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTaskMgr.h>

UIOutlinerPannel::UIOutlinerPannel()
	: UI("Outliner", "##Outliner")
{
	m_Tree = new UITree("OutlinerTree");
	m_Tree->SetRootVisibility(false);
	m_Tree->UseDragDrop(true);
	m_Tree->AddSelectDelegate(this, (DELEGATE_1)&UIOutlinerPannel::SelectObject);
	m_Tree->AddDragDropDelegate(this, (DELEGATE_2)&UIOutlinerPannel::DragDropObject);

	AddChild(m_Tree);

	ReloadCurrentLevel();
}

UIOutlinerPannel::~UIOutlinerPannel()
{
}

void UIOutlinerPannel::render_update()
{
	if (CTaskMgr::GetInst()->GetObjectEvent())
		ReloadCurrentLevel();

	if (KEY_TAP(KEY::DEL))
	{
		TreeNode* pNode = m_Tree->GetSelectedNode();
		if (pNode)
		{
			CGameObject* pSelectObj = (CGameObject*)pNode->GetData();
			GamePlayStatic::DestroyGameObject(pSelectObj);
		}
	}
}

void UIOutlinerPannel::ReloadCurrentLevel()
{
	// reset tree
	m_Tree->Clear();
	TreeNode* pRootNode = m_Tree->CreateNode(nullptr, "DummyRoot", 0);

	// load current level
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pCurLevel)
		return;

	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		CLayer* pLayer = pCurLevel->GetLayer(i);
		const vector<CGameObject*>& vecParent = pLayer->GetParentObjects();

		for (size_t i = 0; i < vecParent.size(); ++i)
			AddObjectToTree(pRootNode, vecParent[i]);
	}
}

// Usage: 선택한 node를 Inspector의 target으로 바꿈
void UIOutlinerPannel::SelectObject(DWORD_PTR _Node)
{
	TreeNode* pNode = (TreeNode*)_Node;
	CGameObject* pObject = (CGameObject*)pNode->GetData();
	if (!pObject)
		return;

	UIInspectorPannel* pInspector = (UIInspectorPannel*)CImGuiMgr::GetInst()->FindUI("##Inspector");
	pInspector->SetTargetObject(pObject);
}

// Usage: 계층구조 조작, source node를 dest node의 child로 만듦
void UIOutlinerPannel::DragDropObject(DWORD_PTR _Dest, DWORD_PTR _Source)
{
	TreeNode* pDestNode = (TreeNode*)_Dest;
	TreeNode* pSourceNode = (TreeNode*)_Source;
	CGameObject* pDestObj = nullptr;
	CGameObject* pSourceObj = nullptr;
	if (pDestNode) pDestObj = (CGameObject*)pDestNode->GetData();
	if (pSourceNode) pSourceObj = (CGameObject*)pSourceNode->GetData();

	// exception: 부모 오브젝트가 자식오브젝트의 자식으로 들어가려는 경우
	if (pDestObj && pDestObj->IsAncestor(pSourceObj))
		return;

	// case: 최상위 gameObject로 만듦
	if (!pDestNode)
	{
		CGameObject* pSourceObj = (CGameObject*)pSourceNode->GetData();
		int LayerIdx = pSourceObj->DisconnectWithParent();
		if (LayerIdx == -1)
			return;

		CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		pCurLevel->AddObject(pSourceObj, LayerIdx, false);
	}
	// case: 누군가의 child로 만듦
	else
	{
		pDestObj->AddChild(pSourceObj);
	}

	ReloadCurrentLevel();
}

void UIOutlinerPannel::AddObjectToTree(TreeNode* _Node, CGameObject* _Object)
{
	TreeNode* pNode = m_Tree->CreateNode(_Node, WstrToStr(_Object->GetName()), (DWORD_PTR)_Object);
	const vector<CGameObject*>& vecChild = _Object->GetChild();

	for (size_t i = 0; i < vecChild.size(); ++i)
		AddObjectToTree(pNode, vecChild[i]);
}
