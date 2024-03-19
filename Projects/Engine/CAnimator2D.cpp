#include "pch.h"
#include "CAnimator2D.h"
#include "CAnim.h"

CAnimator2D::CAnimator2D()
    : CComponent(COMPONENT_TYPE::ANIMATOR2D)
    , m_CurAnim(nullptr)
    , m_bRepeat(false)
{
}

CAnimator2D::CAnimator2D(const CAnimator2D& _OriginAnimator)
    : CComponent(_OriginAnimator)
    , m_CurAnim(nullptr)
    , m_bRepeat(_OriginAnimator.m_bRepeat)
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
        return;

    if (m_CurAnim->IsFinish() && m_bRepeat)
        m_CurAnim->Reset();
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
    m_mapAnim.insert({ _strKey, pAnim });
}

CAnim* CAnimator2D::FindAnim(const wstring& _strAnimName)
{
    unordered_map<wstring, CAnim*>::iterator iter = m_mapAnim.find(_strAnimName);
    if (iter != m_mapAnim.end())
        return iter->second;
    return nullptr;
}

void CAnimator2D::Play(const wstring& _strAnimName, bool _bRepeat)
{
    CAnim* pAnim = FindAnim(_strAnimName);
    if (!pAnim)
        return;

    m_bRepeat = _bRepeat;
    m_CurAnim = pAnim;
    m_CurAnim->Reset();
}

void CAnimator2D::SaveToFile(FILE* _File)
{
    // �ִϸ��̼� ���� ����
    size_t AnimCount = m_mapAnim.size();
    fwrite(&AnimCount, sizeof(size_t), 1, _File);

    for (const auto& pair : m_mapAnim)
    {
        pair.second->SaveToFile(_File);
    }

    // �÷��� ���̴� �ִϸ��̼��� Ű�� �����Ѵ�.
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
    // �ִϸ��̼� ���� �ε�
    size_t AnimCount = 0;
    fread(&AnimCount, sizeof(size_t), 1, _File);

    for (size_t i = 0; i < AnimCount; ++i)
    {
        CAnim* pAnim = new CAnim;
        pAnim->LoadFromFile(_File);

        pAnim->m_Animator = this;
        m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
    }

    // �÷��� ���̴� �ִϸ��̼��� Ű�� �ҷ��´�
    wstring PlayAnimName;
    LoadWString(PlayAnimName, _File);

    if (!PlayAnimName.empty())
    {
        m_CurAnim = FindAnim(PlayAnimName);
    }

    fread(&m_bRepeat, sizeof(bool), 1, _File);
}