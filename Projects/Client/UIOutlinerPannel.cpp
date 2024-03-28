#include "pch.h"
#include "UIOutlinerPannel.h"
#include "UITree.h"
#include "UIInspectorPannel.h"
#include "UIContentPannel.h"
#include "ObjectController.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTaskMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CPrefab.h>

UIOutlinerPannel::UIOutlinerPannel()
	: UI("Outliner", "##Outliner")
{
	m_Tree = new UITree("OutlinerTree");
	m_Tree->SetRootVisibility(false);
	m_Tree->UseDragDrop(true);
	m_Tree->AddSelectDelegate(this, (DELEGATE_1)&UIOutlinerPannel::SelectObject);
	m_Tree->AddRightClickDelegate(this, (DELEGATE_0)&UIOutlinerPannel::OpenRightClickMenu);
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
		DeleteObject();
	
	if (m_bRightClick)
	{
		DrawRightClickMenu();
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

void UIOutlinerPannel::DrawRightClickMenu()
{
	bool bHovered = false;

	if (ImGui::BeginPopupContextItem("##OutlinerRightClickPopup"))
	{
		if (ImGui::Selectable("Copy"))
		{
			TreeNode* pNode = m_Tree->GetSelectedNode();

			if (pNode)
			{
				CGameObject* pOrg = (CGameObject*)pNode->GetData();
				CGameObject* pCopy = pOrg->Clone();
				pCopy->SetName(pOrg->GetName() + L"_copy");

				GamePlayStatic::SpawnGameObject(pCopy, pOrg->GetLayerIdx());
			}

			m_bRightClick = false;
		}
		if (ImGui::IsItemHovered()) bHovered |= true;

		if (ImGui::Selectable("Delete"))
		{
			DeleteObject();
			m_bRightClick = false;
		}
		if (ImGui::IsItemHovered()) bHovered |= true;

		if (ImGui::Selectable("Create Prefab"))
		{
			TreeNode* pNode = m_Tree->GetSelectedNode();

			if (pNode)
			{
				// create prefab
				CGameObject* pSelectObj = (CGameObject*)pNode->GetData();
				Ptr<CPrefab> pPrefab = new CPrefab(pSelectObj->Clone(), false);
				wstring strPath = L"prefab\\" + pSelectObj->GetName() + L".pref";
				pPrefab->SetName(strPath);

				CAssetMgr::GetInst()->AddAsset(strPath, pPrefab.Get());
				UIContentPannel* pContent = (UIContentPannel*)CImGuiMgr::GetInst()->FindUI("##Content");
				pContent->ReloadContent();

				// save prefab
				pPrefab->Save(strPath);
			}

			m_bRightClick = false;
		}
		if (ImGui::IsItemHovered()) bHovered |= true;

		if (ImGui::Selectable("Close"))
		{
			ImGui::CloseCurrentPopup();
			m_bRightClick = false;
		}

		if (KEY_TAP(KEY::LBTN) && not bHovered)
			m_bRightClick = false;

		ImGui::EndPopup();
	}

	ImGui::OpenPopup("##OutlinerRightClickPopup", ImGuiPopupFlags_MouseButtonRight);
}

// Usage: 선택한 node를 Inspector의 target으로 바꿈
void UIOutlinerPannel::SelectObject(DWORD_PTR _Node)
{
	TreeNode* pNode = (TreeNode*)_Node;
	CGameObject* pObject = (CGameObject*)pNode->GetData();
	if (!pObject)
		return;

	UIInspectorPannel* pInspector = (UIInspectorPannel*)CImGuiMgr::GetInst()->FindUI("##Inspector");
	
	if (pInspector->GetTargetObject() == pObject)
		pInspector->GetObjController()->FocusObject(pObject);

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

	if (!pSourceNode) return;

	// exception: 부모 오브젝트가 자식오브젝트의 자식으로 들어가려는 경우
	if (pDestObj && pDestObj->IsAncestor(pSourceObj))
		return;

	// case: 최상위 gameObject로 만듦
	if (!pDestNode)
	{
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

void UIOutlinerPannel::DeleteObject()
{
	TreeNode* pNode = m_Tree->GetSelectedNode();

	if (pNode)
	{
		CGameObject* pSelectObj = (CGameObject*)pNode->GetData();
		GamePlayStatic::DestroyGameObject(pSelectObj);
	}
}

void UIOutlinerPannel::AddObjectToTree(TreeNode* _Node, CGameObject* _Object)
{
	TreeNode* pNode = m_Tree->CreateNode(_Node, WstrToStr(_Object->GetName()), (DWORD_PTR)_Object);
	const vector<CGameObject*>& vecChild = _Object->GetChild();

	for (size_t i = 0; i < vecChild.size(); ++i)
		AddObjectToTree(pNode, vecChild[i]);
}
