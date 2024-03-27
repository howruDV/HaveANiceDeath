#pragma once
#include "UI.h"

class TreeNode;
class UITree;
class UIOutlinerPannel :
    public UI
{
private:
    UITree* m_Tree;
    bool    m_bRightClick;

public:
    virtual void render_update() override;
    void ReloadCurrentLevel();
    void OpenRightClickMenu() { m_bRightClick = true; };

private:
    void AddObjectToTree(TreeNode* _Node, CGameObject* _Object);
    void SelectObject(DWORD_PTR _Node);
    void DragDropObject(DWORD_PTR _Dest, DWORD_PTR _Source);
    void DrawRightClickMenu();
    void DeleteObject();

public:
    UIOutlinerPannel();
    ~UIOutlinerPannel();
};

