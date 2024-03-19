#pragma once
#include "CComponent.h"
#include "CTexture.h"
// =======================================
// CAnimtor2D: Animtion 할당 및 상태 관리 
// =======================================
class CAnim;

class CAnimator2D :
    public CComponent
{
private:
    unordered_map<wstring, CAnim*>  m_mapAnim;
    CAnim*  m_CurAnim;
    bool    m_bRepeat;

public:
    virtual void finaltick() override;
    void UpdatePipeline();

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    static void Clear();    // Animator가 없어도 동작해야하는 함수
    void Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vOffset, Vec2 _vCutSize, Vec2 _vBackgroundSize, int _FrmCount, float _FPS);
    CAnim* FindAnim(const wstring& _strAnimName);
    void Play(const wstring& _strAnimName, bool _bRepeat = true);

public:
    CLONE(CAnimator2D);
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _OriginAnimator);
    ~CAnimator2D();
};

