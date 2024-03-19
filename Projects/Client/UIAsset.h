#pragma once
#include "UI.h"
#include <Engine/Ptr.h>
#include <Engine/CAsset.h>

// =======================================
// UIAsset: Asset UI들이 상속하는 class
// =======================================
class UIAsset :
    public UI
{
private:
    Ptr<CAsset> m_Asset;
    ASSET_TYPE  m_Type;

public:
    virtual void render() override;
    virtual void render_update() override;

public:
    void SetAsset(Ptr<CAsset> _Asset);
    Ptr<CAsset> GetAsset() { return m_Asset; }
    ASSET_TYPE GetType() { return m_Type; }

public:
    UIAsset(const string& _strName, const string& _ID, ASSET_TYPE _Type);
    ~UIAsset();
};

