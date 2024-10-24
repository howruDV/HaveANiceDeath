#pragma once
#include "CEntity.h"
#include "CTexture.h"
// =======================================
// CAnim: Animation 단위 (Components가 소유하는 정보)
// =======================================

class CAnimator2D;

struct FAnimFrm
{
    Vec2    vLeftTopUV;
    Vec2    vCutSizeUV;
    Vec2    vOffsetUV;
    Vec2    vBackgroundSize;
    float   fDuration = 0.f;
};

class CAnim :
    public CEntity
{
private:
    CAnimator2D*        m_Animator;
    Ptr<CTexture>       m_AtlasTex;
    vector<FAnimFrm>    m_vecFrm;
    int                 m_CurFrmIdx;
    float               m_AccTime;
    bool                m_bFinish;
    bool                m_bReverse;

public:
    void finaltick();
    void UpdatePipeline();
    
    void SaveToFile(FILE* _File);
    void LoadFromFile(FILE* _File);
    
    static void Clear();
    void Create(CAnimator2D* _Animator, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vOffset, Vec2 _vCutSize, Vec2 _vBackground, int _FrmCount, float _FPS);
    void Create(CAnimator2D* _Animator, Ptr<CTexture> _Atlas, const vector<FAnimFrm> _vecFrm, bool aleardyUV = false);
    void SetReverse();

    int GetCurFrmIdx() { return m_CurFrmIdx; }
    int GetFrmSize() { return m_vecFrm.size(); }
    FAnimFrm GetCurFrm() { return m_vecFrm[m_CurFrmIdx]; }
    vector<Vec4> GetOffsetList();
    vector<float> GetDurationList();

public:
    bool IsFinish() { return m_bFinish; }
    void Reset()
    {
        m_bFinish = false;
        m_CurFrmIdx = 0;
        m_AccTime = 0.f;
        m_bReverse = false;
    }

public:
    CLONE(CAnim);
    CAnim();
    CAnim(const CAnim& _OriginAnim);
    ~CAnim();

    friend class CAnimator2D;
    friend class UIAnimPannel;
};

