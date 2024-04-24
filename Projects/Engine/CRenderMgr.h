#pragma once
#include "singleton.h"
#include "CTexture.h"
#include "CDownSampling.h"
#include "CUpsampling.h" 
#include "CBlurX.h"   
#include "CBlurY.h"     
#include "CCombine.h"

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
    Vec2                    m_WinResol;
    Vec2                    m_WinScale;

    // camera ---------------------------------
    CCamera*                m_EditorCam;
    vector<CCamera*>        m_vecCam;       // cam은 componets라 여러개 있을 수 있음

    // light ----------------------------------
    vector<CLight2D*>       m_vecLight2D;
    CStructuredBuffer*      m_Light2DBuffer;

    // debuging draw --------------------------
    list<FDebugShapeInfo>   m_DbgShapeInfo;
    CGameObject*            m_pDbgObj;
    bool                    m_DebugPosition;
    bool                    m_isEditorMode;

    // render texture --------------------------
    Ptr<CTexture>           m_PostProcessTex;
    Ptr<CTexture>           m_RTCopyTex;
    vector<Ptr<CTexture>>   m_vecNoiseTex;

    // render texture - Bloom Texture
    int                     m_BloomLevel;
    Ptr<CTexture>           m_BloomRTTex;
    Ptr<CTexture>           m_BloomRTCopyTex;
    vector<Ptr<CTexture>>   m_vecBlurXTex;
    vector<Ptr<CTexture>>   m_vecBlurXYTex;
    Ptr<CDownSampling>      m_CSBloomDownScaling;
    Ptr<CUpsampling>        m_CSBloomUpScaling;
    Ptr<CBlurX>             m_CSBloomBluringX;
    Ptr<CBlurY>             m_CSBloomBluringY;
    Ptr<CCombine>           m_CSBloomCombine;

    // render function --------------------------
    // - 게임/에디터 별로 render를 다르게 돌려야 하기 때문
    typedef void(CRenderMgr::* RENDER_FUNC)(void);
    RENDER_FUNC             m_RenderFunc;

public:
    void init();
    void tick();

    void CopyRenderTargetToPostProcessTarget();
    void CopyRenderTargetToImGuiRenderTexture();
    void CopyRenderTargetToBloomTarget();
    void Execute_Bloom();

private:
    void CreateDebugObj();
    void CreateBloomCS();
    void CreatePostprocessTexture(Vec2 _vResolution);
    void CreateRTCopyTexture(Vec2 _vResolution);
    void CreateBloomTextures(Vec2 _vResolution);

    void UpdatePipeline();
    void Clear();
    void render_play();
    void render_editor();
    void render_debug();

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
    Ptr<CTexture> GetBloomRTTex() { return m_BloomRTTex; }
    const vector<CCamera*>& GetCameras() { return m_vecCam; }
    CCamera* GetEditorCamera() { return m_EditorCam; }
    Vec2 GetWinResol() { return m_WinResol; }
    Vec2 GetWinScale() { return m_WinScale; }
    bool IsDebugPosition() { return m_DebugPosition; }
    bool IsEditorMode() { return m_isEditorMode; }
};