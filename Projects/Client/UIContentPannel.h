#pragma once
#include "UI.h"
#include <Engine/CPrefab.h>
// =======================================
// UIContentPannel: 현재 메모리에 load된 asset
// =======================================

class UITree;

class UIContentPannel :
    public UI
{
private:
    UITree* m_Tree;
    vector<wstring>     m_vecAssetFileName;

public:
    virtual void render_update() override;

    void SelectAsset(DWORD_PTR _Node);
    void ResetContent();
    void ReloadContent();

private:
    void FindFileName(const wstring& _Directory);
    ASSET_TYPE GetAssetTypeByExt(const path& _relativePath);

public:
    UIContentPannel();
    ~UIContentPannel();
};

