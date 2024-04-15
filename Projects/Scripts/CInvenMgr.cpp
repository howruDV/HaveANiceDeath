#include "pch.h"
#include "CInvenMgr.h"
#include "CPlayerMgr.h"
#include "CPlayerScript.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

#define ANIMA_BLUE 30.f
#define ANIMA_YELLOW 20.f

CInvenMgr::CInvenMgr()
    : CScript(INVENMGR)
    , m_iAnimaCount(0)
    , m_iIngot(0)
    , m_iSoulary(0)
    , m_iPrismium(0)
{
    FAnima tmp = { ANIMA_TYPE::BLUE, nullptr };
    FAnima tmp1 = { ANIMA_TYPE::YELLOW, nullptr };
    m_Anima[0] = tmp1;
    m_Anima[1] = tmp;
    m_Anima[2] = tmp;
}

CInvenMgr::~CInvenMgr()
{
}

void CInvenMgr::begin()
{
    // set anima material
    m_AnimaMat[0] = CAssetMgr::GetInst()->Load<CMaterial>(L"material\\HUD_lifeCharge.mat");
    m_AnimaMat[1] = CAssetMgr::GetInst()->Load<CMaterial>(L"material\\HUD_lifeCharge_on.mat");

    // find anima
    CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

    if (pLevel)
    {
        for (int i = 0; i < 3; ++i)
        {
            m_Anima[i].pObj = pLevel->FindObjectByName(L"HUD_Anima" + std::to_wstring(i), 31);
            if (!m_Anima[i].pObj)
                continue;

            if (m_Anima[i].Type == ANIMA_TYPE::BLUE)
            {
                m_iAnimaCount++;
                m_Anima[i].pObj->MeshRender()->SetMaterial(m_AnimaMat[1]);
                m_Anima[i].pObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(0, 1, 1, 1));
            }
            else if (m_Anima[i].Type == ANIMA_TYPE::YELLOW)
            {
                m_iAnimaCount++;
                m_Anima[i].pObj->MeshRender()->SetMaterial(m_AnimaMat[1]);
                m_Anima[i].pObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(1, 1, 0, 1));
            }
            else if (m_Anima[i].Type == ANIMA_TYPE::NONE)
            {
                m_Anima[i].pObj->MeshRender()->SetMaterial(m_AnimaMat[0]);
            }
        }
    }

}

void CInvenMgr::tick()
{
}

bool CInvenMgr::UseAnima()
{
    if (m_iAnimaCount <= 0)
        return false;

    int idx = m_iAnimaCount - 1;

    if (m_Anima[idx].Type == ANIMA_TYPE::BLUE)
    {
        m_iAnimaCount--;
        m_Anima[idx].pObj->MeshRender()->SetMaterial(m_AnimaMat[0]);
        m_Anima[idx].Type = ANIMA_TYPE::NONE;

        CPlayerMgr::GetPlayerScript()->AddHPCur(ANIMA_BLUE);
    }

    else if (m_Anima[idx].Type == ANIMA_TYPE::YELLOW)
    {
        m_iAnimaCount--;
        m_Anima[idx].pObj->MeshRender()->SetMaterial(m_AnimaMat[0]);
        m_Anima[idx].Type = ANIMA_TYPE::NONE;

        CPlayerMgr::GetPlayerScript()->AddHPCur(ANIMA_BLUE);
        CPlayerMgr::GetPlayerScript()->AddHPActive(ANIMA_YELLOW);
    }

    return true;
}
