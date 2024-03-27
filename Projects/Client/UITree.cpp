#include "pch.h"
#include "UITree.h"
#include <Engine/CKeyMgr.h>

// ---------------------------------------------------
// TreeNode
// ---------------------------------------------------

TreeNode::TreeNode()
	: m_bFrame(false)
{
}

TreeNode::~TreeNode()
{
	Delete_Vec(m_vecChildNode);
}

void TreeNode::render_update()
{
	string strID = m_Name + m_ID;
	UINT Flag = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

	// flag
	if (m_bFrame)
		Flag |= ImGuiTreeNodeFlags_Framed;
	if (m_vecChildNode.empty())
		Flag |= ImGuiTreeNodeFlags_Leaf;
	if (m_bSelected)
		Flag |= ImGuiTreeNodeFlags_Selected;
	if (m_bFrame && m_vecChildNode.empty())
		strID = "   " + strID;

	// open tree node
	if (ImGui::TreeNodeEx(strID.c_str(), Flag))
	{
		// event
		// case: drag
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(m_Owner->GetID().c_str(), &m_Data, sizeof(DWORD_PTR));
			ImGui::Text(m_Name.c_str());
			ImGui::EndDragDropSource();

			// 소속 Tree에 Drag 발생한 Node임을 알림
			m_Owner->SetDragNode(this);
		}
		// case: drop
		else if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_Owner->GetID().c_str());
			if (payload)
				m_Owner->SetDropNode(this);

			ImGui::EndDragDropTarget();
		}
		// case: click
		else if (KEY_RELEASED(KEY::LBTN) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			m_Owner->SetSelectedNode(this);
		else if (KEY_RELEASED(KEY::RBTN) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			m_Owner->SetRightClickedNode(this);

		// render
		for (size_t i = 0; i < m_vecChildNode.size(); ++i)
			m_vecChildNode[i]->render_update();

		ImGui::TreePop();
	}
	// closed tree node
	else
	{
		// event
		// case: drag
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload(m_Owner->GetID().c_str(), &m_Data, sizeof(DWORD_PTR));
			ImGui::Text(m_Name.c_str());
			ImGui::EndDragDropSource();

			// 소속 Tree에 Drag 발생한 Node임을 알림
			m_Owner->SetDragNode(this);
		}
		// case: click
		else if (KEY_RELEASED(KEY::LBTN) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
			m_Owner->SetSelectedNode(this);
	}
}



// ---------------------------------------------------
// UITree
// ---------------------------------------------------
UINT UITree::NodeID = 0;

UITree::UITree(const string& _ID)
	: UI("##UITree", _ID)
	, m_bRootVisiblity(true)
	, m_bDragDrop(false)
{
}

UITree::~UITree()
{
	delete m_Root;
}

void UITree::render_update()
{
	if (!m_Root)
		return;

	// render
	if (m_bRootVisiblity)
		m_Root->render_update();
	else
	{
		for (size_t i = 0; i < m_Root->m_vecChildNode.size(); ++i)
			m_Root->m_vecChildNode[i]->render_update();
	}

	// ---------------
	// Delegate 호출
	// ---------------
	// case: left click
	if (m_bSelectEvent)
	{
		if (m_SelectInst && m_SelectFunc)
			(m_SelectInst->*m_SelectFunc)((DWORD_PTR)m_Selected);
	}

	// case: right click
	if (m_bRightClickEvent)
	{
		if (m_SelectInst && m_RightClickFunc)
			(m_SelectInst->*m_RightClickFunc)();
	}

	// case: Drop - target이 tree node가 아닌 경우(허공)
	if (KEY_RELEASED(KEY::LBTN) && m_DragNode && !m_DropNode)
	{
		if (m_DragDropInst && m_DragDropFunc)
			(m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_DropNode, (DWORD_PTR)m_DragNode);

		m_DragNode = nullptr;
	}
	else if (m_bDragDropEvent)
	{
		if (m_DragDropInst && m_DragDropFunc)
			(m_DragDropInst->*m_DragDropFunc)((DWORD_PTR)m_DropNode, (DWORD_PTR)m_DragNode);

		m_DropNode = nullptr;
		m_DragNode = nullptr;
	}

	m_bSelectEvent = false;
	m_bDragDropEvent = false;
	m_bRightClickEvent = false;
}

TreeNode* UITree::CreateNode(TreeNode* _Parent, string _strName, DWORD_PTR _data)
{
	TreeNode* pNewNode = new TreeNode;
	char buff[50] = {};
	sprintf_s(buff, "##TreeNode%d", NodeID++);
	pNewNode->SetID(buff);
	pNewNode->m_Name = _strName;
	pNewNode->m_Data = _data;
	pNewNode->m_Owner = this;

	if (_Parent)
		_Parent->AddChild(pNewNode);
	else
	{
		assert(!m_Root);
		m_Root = pNewNode;
	}

	return pNewNode;
}

void UITree::SetSelectedNode(TreeNode* _SelectNode)
{
	if (m_Selected)
		m_Selected->m_bSelected = false;

	m_Selected = _SelectNode;
	if (m_Selected)
		m_Selected->m_bSelected = true;

	m_bSelectEvent = true;
}

void UITree::SetRightClickedNode(TreeNode* _SelectNode)
{
	SetSelectedNode(_SelectNode);
	m_bRightClickEvent = true;
}

void UITree::SetDragNode(TreeNode* _DragNode)
{
	m_DragNode = _DragNode;
}

void UITree::SetDropNode(TreeNode* _DropNode)
{
	m_DropNode = _DropNode;
	m_bDragDropEvent = true;
}
