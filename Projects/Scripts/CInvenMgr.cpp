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
    m_Anima[0] = { ANIMA_TYPE::YELLOW, nullptr };
    m_Anima[1] = { ANIMA_TYPE::BLUE, nullptr };
    m_Anima[2] = { ANIMA_TYPE::NONE, nullptr };

    m_Weapon[0] = { (UINT)SCYTHE_TYPE::DISS, nullptr };
    m_Weapon[1] = { -1, nullptr };
    m_Weapon[2] = { -1, nullptr };
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
            // anima
            m_Anima[i].pObj = pLevel->FindObjectByName(L"HUD_Anima" + std::to_wstring(i), 31);
            if (m_Anima[i].pObj)
            {
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

            //weapon
            m_Weapon[i].pObj = pLevel->FindObjectByName(L"Icon_Weapon" + std::to_wstring(i), 31);
            if (m_Weapon[i].pObj)
            {
                if (m_Weapon[i].Type != -1)
                {
                    wstring strName = L"material\\Icon_" + GetScytheName() + L".mat";
                    Ptr<CMaterial> mat = CAssetMgr::GetInst()->Load<CMaterial>(strName);
                    m_Weapon[i].pObj->GetRenderComponent()->SetMaterial(mat);
                    m_Weapon[i].pObj->Activate();
                }
                else
                {
                    m_Weapon[i].pObj->Deactivate();
                }
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

void CInvenMgr::AddAnima()
{
    // case: anima isnt' full
    if (m_iAnimaCount < 3)
    {
        m_Anima[m_iAnimaCount].Type = ANIMA_TYPE::BLUE;
        m_Anima[m_iAnimaCount].pObj->MeshRender()->SetMaterial(m_AnimaMat[1]);
        m_Anima[m_iAnimaCount].pObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(0, 1, 1, 1));
        m_iAnimaCount++;
    }

    // case: anima is full
    else
    {
        for (int i = 0; i < 3; ++i)
        {
            if (m_Anima[i].Type == ANIMA_TYPE::YELLOW)
                continue;

            m_Anima[i].Type = ANIMA_TYPE::YELLOW;
            m_Anima[i].pObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(1, 1, 0, 1));
            break;
        }
    }
}

FScytheDamage CInvenMgr::GetScytheDamage()
{
    if (m_Weapon[0].Type == (int)SCYTHE_TYPE::DISS)
        return SCYTHE_DISS_DAMAGE;

    return FScytheDamage{};
}