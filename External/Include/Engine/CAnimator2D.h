#pragma once
#include "CComponent.h"
#include "CTexture.h"
#include "CAnim.h"
// =======================================
// CAnimtor2D: Animtion 할당 및 상태 관리 
// =======================================
class CAnim;

struct NextAnimInfo
{
    CAnim*  pAnim;
    bool    bRepeat;
};

class CAnimator2D :
    public CComponent
{
private:
    unordered_map<wstring, CAnim*>  m_mapAnim;
    list<NextAnimInfo>  m_listNextAnim;
    CAnim*  m_CurAnim;
    bool    m_bFlipX;
    bool    m_bFlipY;
    bool    m_bRepeat;

public:
    virtual void finaltick() override;
    void UpdatePipeline();

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    static void Clear();    // Animator가 없어도 동작해야하는 함수
    CAnim* FindAnim(const wstring& _strAnimName);
    void Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vOffset, Vec2 _vCutSize, Vec2 _vBackgroundSize, int _FrmCount, float _FPS);
    void Create(const wstring& _strKey, Ptr<CTexture> _AltasTex, const vector<FAnimFrm>& _vecFrm);
    void Create(FILE* _File);
    void Create(CAnim* _Anim, const wstring& _strKey = L"");
    void Play(const wstring& _strAnimName, bool _bRepeat = true);

public:
    void SetFlipX(bool _bool) { m_bFlipX = _bool; }
    void SetFlipY(bool _bool) { m_bFlipY = _bool; }
    void ClearNextAnim() { m_listNextAnim.clear(); }
    void PushNextAnim(const wstring& _strAnimName, bool _bRepeat = false);
    
    bool GetFlipX() { return m_bFlipX; }
    bool GetFlipY() { return m_bFlipY; }

public:
    CLONE(CAnimator2D);
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _OriginAnimator);
    ~CAnimator2D();
};

