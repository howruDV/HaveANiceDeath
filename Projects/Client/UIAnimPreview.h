#pragma once
#include "UI.h"
#include <Engine/CGameObject.h>
// =======================================
// UIAnimPreview: 제작 중인 Animation 미리 재생
// =======================================

class UIAnimPannel;
class UIAnimDetail;

class UIAnimPreview :
    public UI
{
private:
    UIAnimPannel*       m_MainPannel;
    UIAnimDetail*       m_DetailPannel;

    Ptr<CTexture>       m_AtlasTex;
    FAnimFrm*           m_CurFrm;
    int                 m_FrmSize;
    int                 m_CurFrmIdx;
    float               m_AccTime;
    bool                m_bPrevActive;

public:
    virtual void tick() override;
    virtual void begin() override;
    virtual void render_update() override;

    void Clear();

public:
    void SetAtlas(Ptr<CTexture> _atlas) { m_AtlasTex = _atlas; }
    void SetFrmSize(int _FrmSize) { m_FrmSize = _FrmSize; }
    void Init(FAnimFrm& _Frm) { m_CurFrm = &_Frm; m_CurFrmIdx = 0; m_AccTime = 0; }

public:
    UIAnimPreview();
    ~UIAnimPreview();
};

