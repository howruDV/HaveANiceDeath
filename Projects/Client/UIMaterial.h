#pragma once
#include "UIAsset.h"
class UIMaterial :
    public UIAsset
{
private:
    TEX_PARAM       m_SelectTexParam;

public:
    virtual void render_update() override;

public:
    void SelectTexture(DWORD_PTR _dwData);
    static void render_UIMatParam(Ptr<CMaterial> pMat, Ptr<CGraphicsShader> pShader);

public:
    UIMaterial();
    ~UIMaterial();
};

