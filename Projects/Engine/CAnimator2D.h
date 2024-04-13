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
    unordered_map<wstring, CAnim*>  m_mapAnim;  // map의 key는 anim의 name과 같음
    list<NextAnimInfo>              m_listNextAnim;
    CAnim*                          m_CurAnim;
    UNIT_DIRX                       m_bFlipX;
    UNIT_DIRY                       m_bFlipY;
    bool                            m_bRepeat;

public:
    virtual void finaltick() override;
    void UpdatePipeline();

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    static void Clear();    // Animator가 없어도 동작해야하는 함수
    void Play(const wstring& _strAnimName, bool _bRepeat = true, bool _bReverse = false);
    void Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vOffset, Vec2 _vCutSize, Vec2 _vBackgroundSize, int _FrmCount, float _FPS);
    void Create(const wstring& _strKey, Ptr<CTexture> _AltasTex, const vector<FAnimFrm>& _vecFrm);
    void Create(FILE* _File);
    void Create(CAnim* _Anim, const wstring& _strKey = L"");
    CAnim* FindAnim(const wstring& _strAnimName);
    int DeleteAnim(const wstring& _AnimationKey);

public:
    void SetFlipX(UNIT_DIRX _bool) { m_bFlipX = _bool; }
    void SetFlipY(UNIT_DIRY _bool) { m_bFlipY = _bool; }
    void ClearNextAnim() { m_listNextAnim.clear(); }
    void PushNextAnim(const wstring& _strAnimName, bool _bRepeat = false);

    const unordered_map<wstring, CAnim*>& GetAnimations() { return m_mapAnim; }
    const wstring& GetCurAnimName() { if (m_CurAnim) { return m_CurAnim->GetName(); } return L""; }
    UNIT_DIRX GetFlipX() { return m_bFlipX; }
    UNIT_DIRY GetFlipY() { return m_bFlipY; }
    int GetCurAnimFrmIdx() { return m_CurAnim->GetCurFrmIdx(); }
    FAnimFrm GetCurAnimFrm() { return m_CurAnim->GetCurFrm(); }
    bool IsPlaying() { if (m_CurAnim) { return !m_CurAnim->IsFinish(); } return false; }

public:
    CLONE(CAnimator2D);
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _OriginAnimator);
    ~CAnimator2D();
};

