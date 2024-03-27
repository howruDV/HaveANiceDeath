#pragma once
#include "UI.h"

// =======================================
// TreeNode: UITree를 구성하는 Node 하나 관리
// =======================================
class TreeNode
{
private:
    UITree*             m_Owner;        // 해당 Node를 소유한 Tree UI
    string              m_ID;
    string              m_Name;
    DWORD_PTR           m_Data;

    TreeNode*           m_ParentNode;   // 계층정보
    vector<TreeNode*>   m_vecChildNode;

    bool                m_bFrame;
    bool                m_bSelected;

public:
    void render_update();

public:
    void SetName(const string& _Name) { m_Name = _Name; }
    void SetFrame(bool _Frame) { m_bFrame = _Frame; }
    const string& GetName() { return m_Name; }
    DWORD_PTR GetData() { return m_Data; }

private:
    void SetID(const string& _ID) { m_ID = _ID; }
    void AddChild(TreeNode* _Node) { 
        m_vecChildNode.push_back(_Node);
        _Node->m_ParentNode = this;
    }

public:
    TreeNode();
    ~TreeNode();

    friend class UITree;
};



// =======================================
// TreeNode: Tree의 최상위 Node만 관리
// =======================================
class UITree :
    public UI
{
private:
    static UINT     NodeID;

private:
    TreeNode*       m_Root;
    bool            m_bRootVisiblity;

    TreeNode*       m_Selected;
    UI*             m_SelectInst;       // Delegate 호출시킬 객체
    DELEGATE_1      m_SelectFunc;       // 좌클릭
    DELEGATE_0      m_RightClickFunc;   // 우클릭
    bool            m_bSelectEvent;
    bool            m_bRightClickEvent;

    TreeNode*       m_DragNode;
    TreeNode*       m_DropNode;
    UI*             m_DragDropInst;
    DELEGATE_2      m_DragDropFunc;
    bool            m_bDragDrop;
    bool            m_bDragDropEvent;

public:
    virtual void render_update() override;
    TreeNode* CreateNode(TreeNode* _Parent, string _strName, DWORD_PTR _data);

    // getter & setter
public:
    void AddSelectDelegate(UI* _Inst, DELEGATE_1 _pFunc) { m_SelectInst = _Inst; m_SelectFunc = _pFunc; }
    void AddRightClickDelegate(UI* _Inst, DELEGATE_0 _pFunc) { m_SelectInst = _Inst; m_RightClickFunc = _pFunc; }
    void AddDragDropDelegate(UI* _Inst, DELEGATE_2 _pFunc) { m_DragDropInst = _Inst; m_DragDropFunc = _pFunc; }
    void UseDragDrop(bool _Use) { m_bDragDrop = _Use; }
    void SetRootVisibility(bool _bRootVisiblity) { m_bRootVisiblity = _bRootVisiblity; }
    void Clear() {
        if (m_Root)
        {
            delete m_Root;
            m_Root = nullptr;
        }
    }
    TreeNode* GetRootNode() { return m_Root; }
    TreeNode* GetSelectedNode() { return m_Selected; }

private:
    void SetSelectedNode(TreeNode* _SelectNode);
    void SetRightClickedNode(TreeNode* _SelectNode);
    void SetDragNode(TreeNode* _DragNode);
    void SetDropNode(TreeNode* _DropNode);

public:
    UITree(const string& _ID);
    ~UITree();

    friend class TreeNode;
};

