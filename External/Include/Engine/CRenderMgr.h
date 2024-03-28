#pragma once
#include "singleton.h"
#include "CTexture.h"
// =======================================
// CRenderMgr: Render 전담
// =======================================

class CCamera;
class CLight2D;
class CGameObject;
class CStructuredBuffer;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);

private:
    CCamera*                m_EditorCam;
    vector<CCamera*>        m_vecCam;       // cam은 componets라 여러개 있을 수 있음

    vector<CLight2D*>       m_vecLight2D;
    CStructuredBuffer*      m_Light2DBuffer;

    list<FDebugShapeInfo>   m_DbgShapeInfo;
    CGameObject*            m_pDbgObj;
    bool                    m_DebugPosition;
    bool                    m_isEditorMode;

    Ptr<CTexture>           m_PostProcessTex;
    Ptr<CTexture>           m_RTCopyTex;
    vector<Ptr<CTexture>>   m_vecNoiseTex;

    // render function: 게임/에디터 별로 render를 다르게 돌려야 하기 때문
    typedef void(CRenderMgr::* RENDER_FUNC)(void);
    RENDER_FUNC             m_RenderFunc;

public:
    void init();
    void tick();

    void CopyRenderTargetToPostProcessTarget();
    void CopyRenderTargetToImGuiRenderTexture();

public:
    void RegisterCamera(CCamera* _Cam, int _idx);
    void RegisterEditorCamera(CCamera* _Cam) { m_EditorCam = _Cam; }
    void ClearCamera() { m_vecCam.clear(); }
    void RegisterLight2D(CLight2D* _Light) { m_vecLight2D.push_back(_Light); }
    void AddDebugShapeInfo(const FDebugShapeInfo& _info) { m_DbgShapeInfo.push_back(_info); }
    void SetDebugPosition(bool _OnOff) { m_DebugPosition = _OnOff; }
    void ActiveEditorMode(bool _bActive);

    Ptr<CTexture> GetRTCopyTex() { return m_RTCopyTex; }
    Ptr<CTexture> GetPostProcessTex() { return m_PostProcessTex; }
    const vector<CCamera*>& GetCameras() { return m_vecCam; }
    CCamera* GetEditorCamera() { return m_EditorCam; }
    bool IsDebugPosition() { return m_DebugPosition; }
    bool IsEditorMode() { return m_isEditorMode; }

private:
    void render_debug();

    void UpdatePipeline();
    void Clear();
    void render_play();
    void render_editor();
};

