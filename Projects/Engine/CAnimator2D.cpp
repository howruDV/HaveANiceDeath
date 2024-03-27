#include "pch.h"
#include "CAnimator2D.h"
#include "CLevelMgr.h"
#include "CAnim.h"
#include "CTransform.h"
#include "CLevel.h"

CAnimator2D::CAnimator2D()
    : CComponent(COMPONENT_TYPE::ANIMATOR2D)
    , m_CurAnim(nullptr)
    , m_bRepeat(false)
    , m_bFlipX(false)
{
}

CAnimator2D::CAnimator2D(const CAnimator2D& _OriginAnimator)
    : CComponent(_OriginAnimator)
    , m_CurAnim(nullptr)
    , m_bRepeat(_OriginAnimator.m_bRepeat)
    , m_bFlipX(_OriginAnimator.m_bFlipX)
    , m_bFlipY(_OriginAnimator.m_bFlipY)
{
    unordered_map<wstring, CAnim*>::const_iterator iter = _OriginAnimator.m_mapAnim.begin();
    for (; iter != _OriginAnimator.m_mapAnim.end(); ++iter)
    {
        CAnim* pCloneAnim = iter->second->Clone();

        pCloneAnim->m_Animator = this;
        m_mapAnim.insert(make_pair(iter->first, pCloneAnim));
    }

    if (nullptr != _OriginAnimator.m_CurAnim)
    {
        m_CurAnim = FindAnim(_OriginAnimator.m_CurAnim->GetName());
    }
}

CAnimator2D::~CAnimator2D()
{
    Delete_Map(m_mapAnim);
}

void CAnimator2D::finaltick()
{
    if (not m_CurAnim)
    {
        if (m_listNextAnim.empty())
            return;
        else
        {
            m_CurAnim = m_listNextAnim.front().pAnim;
            m_bRepeat = m_listNextAnim.front().bRepeat;
            m_listNextAnim.pop_front();
        }
    }

    if (m_CurAnim->IsFinish())
    {
        if (m_bRepeat)
            m_CurAnim->Reset();
        else if (!m_bRepeat && !m_listNextAnim.empty())
        {
            m_CurAnim = m_listNextAnim.front().pAnim;
            m_bRepeat = m_listNextAnim.front().bRepeat;
            m_listNextAnim.pop_front();
        }
    }

    m_CurAnim->finaltick();
}

void CAnimator2D::UpdatePipeline()
{
    if (not m_CurAnim)
        return;

    m_CurAnim->UpdatePipeline();
}

void CAnimator2D::Clear()
{
    CAnim::Clear();
}

void CAnimator2D::Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vOffset, Vec2 _vCutSize, Vec2 _vBackgroundSize, int _FrmCount, float _FPS)
{
    CAnim* pAnim = FindAnim(_strKey);
    assert(!pAnim);

    pAnim = new CAnim;
    pAnim->Create(this, _AtlasTex, _vLeftTop, _vOffset, _vCutSize, _vBackgroundSize, _FrmCount, _FPS);
    pAnim->SetName(_strKey);
    m_mapAnim.insert({ _strKey, pAnim });
}

void CAnimator2D::Create(const wstring& _strKey, Ptr<CTexture> _AltasTex, const vector<FAnimFrm>& _vecFrm)
{
    CAnim* pAnim = FindAnim(_strKey);
    if (pAnim && _strKey == L"PreviewAnim")
    {
        m_mapAnim.erase(m_mapAnim.find(_strKey));
        pAnim->Clear();
    }
    else
        assert(!pAnim);

    pAnim = new CAnim;
    pAnim->Create(this, _AltasTex, _vecFrm);
    pAnim->SetName(_strKey);
    m_mapAnim.insert(make_pair(_strKey, pAnim));
}

void CAnimator2D::Create(FILE* _File)
{
    if (not _File)
    {
        MessageBoxA(nullptr, "Can't open file or Animation file path is wrong!", "Failed To Open Create Animation", MB_OK);
    }

    CAnim* pAnim = new CAnim;
    pAnim->LoadFromFile(_File);
    pAnim->m_Animator = this;
    m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
}

void CAnimator2D::Create(CAnim* _Anim, const wstring& _strKey)
{
    wstring key = (_strKey == "")? _Anim->GetName() : _strKey;
    if (not _Anim || key == "" || FindAnim(key))
    {
        MessageBoxA(nullptr, "No animation key specified.", "Failed to Create Animation", MB_OK);
        return;
    }

    CAnim* pAnim = _Anim->Clone();
    pAnim->SetName(key);
    pAnim->m_Animator = this;
    m_mapAnim.insert(make_pair(key, pAnim));
}

CAnim* CAnimator2D::FindAnim(const wstring& _strAnimName)
{
    unordered_map<wstring, CAnim*>::iterator iter = m_mapAnim.find(_strAnimName);
    if (iter != m_mapAnim.end())
        return iter->second;
    return nullptr;
}

int CAnimator2D::DeleteAnim(const wstring& _AnimationKey)
{
    CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    if (LEVEL_STATE::PLAY == CurLevel->GetState())
    {
        return E_FAIL;
    }

    CAnim* SelectedAnim = FindAnim(_AnimationKey);
    if (nullptr == SelectedAnim)
    {
        MessageBox(nullptr, L"애니메이션이 존재하지 않습니다.", L"Animation Delete Failed", MB_OK);
        return E_FAIL;
    }

    delete SelectedAnim;
    m_mapAnim.erase(_AnimationKey);

    return S_OK;
}

void CAnimator2D::Play(const wstring& _strAnimName, bool _bRepeat)
{
    CAnim* pAnim = FindAnim(_strAnimName);
    if (!pAnim)
        return;

    // set background size
    Vec3 newBgSize = Vec3(pAnim->m_vecFrm[0].vBackgroundSize.x, pAnim->m_vecFrm[0].vBackgroundSize.y, 1.f);
    newBgSize.x *= pAnim->m_AtlasTex->GetWidth();
    newBgSize.y *= pAnim->m_AtlasTex->GetHeight();
    if (Transform()->GetRelativeScale() != newBgSize)
    {
        Transform()->SetRelativeScale(newBgSize);
    }

    m_bRepeat = _bRepeat;
    m_CurAnim = pAnim;
    m_CurAnim->Reset();
}

void CAnimator2D::PushNextAnim(const wstring& _strAnimName, bool _bRepeat)
{
    CAnim* pAnim = FindAnim(_strAnimName);
    if (!pAnim)
        return;

    NextAnimInfo next = { pAnim, _bRepeat };
    m_listNextAnim.push_back(next);
}

void CAnimator2D::SaveToFile(FILE* _File)
{
    // 애니메이션 개수 저장
    size_t AnimCount = m_mapAnim.size();
    fwrite(&AnimCount, sizeof(size_t), 1, _File);

    for (const auto& pair : m_mapAnim)
    {
        pair.second->SaveToFile(_File);
    }

    // 플레이 중이던 애니메이션의 키를 저장한다.
    wstring PlayAnimName;
    if (nullptr != m_CurAnim)
    {
        PlayAnimName = m_CurAnim->GetName();
    }

    SaveWString(PlayAnimName, _File);
    fwrite(&m_bRepeat, sizeof(bool), 1, _File);
}

void CAnimator2D::LoadFromFile(FILE* _File)
{
    // 애니메이션 개수 로드
    size_t AnimCount = 0;
    fread(&AnimCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < AnimCount; ++i)
    {
        CAnim* pAnim = new CAnim;
        pAnim->LoadFromFile(_File);

        pAnim->m_Animator = this;
        m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
    }

    // 플레이 중이던 애니메이션의 키를 불러온다
    wstring PlayAnimName;
    LoadWString(PlayAnimName, _File);

    if (!PlayAnimName.empty())
    {
        m_CurAnim = FindAnim(PlayAnimName);
    }

    fread(&m_bRepeat, sizeof(bool), 1, _File);
}