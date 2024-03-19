#pragma once
#include "UI.h"

class TreeNode;
class UITree;
class UIOutlinerPannel :
    public UI
{
private:
    UITree* m_Tree;

public:
    virtual void render_update() override;
    void ReloadCurrentLevel();

private:
    void AddObjectToTree(TreeNode* _Node, CGameObject* _Object);
    void SelectObject(DWORD_PTR _Node);
    void DragDropObject(DWORD_PTR _Dest, DWORD_PTR _Source);

public:
    UIOutlinerPannel();
    ~UIOutlinerPannel();
};

