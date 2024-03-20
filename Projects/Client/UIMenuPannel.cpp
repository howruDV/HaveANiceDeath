#include "pch.h"
#include "UIMenuPannel.h"
#include "CImGuiMgr.h"
#include "UIInspectorPannel.h"
#include "UIAnimPannel.h"
#include "UIAnimDetail.h"
#include "CLevelSaveLoad.h"

#include <Engine/CPathMgr.h>
#include <Engine/CTaskMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CScript.h>
#include <Scripts/CScriptMgr.h>

UIMenuPannel::UIMenuPannel()
	: UI("Menu", "##Menu")
{
}

UIMenuPannel::~UIMenuPannel()
{
}

void UIMenuPannel::render()
{
    if (ImGui::BeginMainMenuBar())
    {
        render_update();

        ImGui::EndMainMenuBar();
    }
}

void UIMenuPannel::render_update()
{
    File();
    Tool();
    Level();
    GameObject();
    Asset();
}

void UIMenuPannel::File()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Save Level", ""))
        {
            wchar_t szSelect[256] = {};
            OPENFILENAME ofn = {};
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = nullptr;
            ofn.lpstrFile = szSelect;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szSelect);
            ofn.lpstrFilter = L"ALL\0*.*\0Level\0*.lv";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;

            // 탐색창 초기 위치 지정
            wstring strInitPath = CPathMgr::GetContentPath();
            strInitPath += L"level\\";
            ofn.lpstrInitialDir = strInitPath.c_str();

            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
            if (GetSaveFileName(&ofn))
            {
                CLevelSaveLoad::SaveLevel(CLevelMgr::GetInst()->GetCurrentLevel(), CPathMgr::GetRelativePath(szSelect));
            }
        }

        if (ImGui::MenuItem("Load Level", ""))
        {
            wchar_t szSelect[256] = {};
            OPENFILENAME ofn = {};
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = nullptr;
            ofn.lpstrFile = szSelect;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szSelect);
            ofn.lpstrFilter = L"ALL\0*.*\0Level\0*.lv";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;

            // 탐색창 초기 위치 지정
            wstring strInitPath = CPathMgr::GetContentPath();
            strInitPath += L"level\\";
            ofn.lpstrInitialDir = strInitPath.c_str();

            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetOpenFileName(&ofn))
            {
                CLevel* pLevel = CLevelSaveLoad::LoadLevel(CPathMgr::GetRelativePath(szSelect));
                CLevelMgr::GetInst()->ChangeLevel(pLevel, LEVEL_STATE::STOP);

                // Inspector 의 타겟정보를 nullptr 로 되돌리기
                UIInspectorPannel* pInspector = (UIInspectorPannel*)CImGuiMgr::GetInst()->FindUI("##Inspector");
                pInspector->SetTargetObject(nullptr);
            }
        }

        ImGui::EndMenu();
    }
}

void UIMenuPannel::Tool()
{
    if (ImGui::BeginMenu("Tool"))
    {
        if (ImGui::MenuItem("Create Animation"))
        {
            UIAnimPannel* pAnimToolMain = (UIAnimPannel*)CImGuiMgr::GetInst()->FindUI("##Animation Tool_Main");
            pAnimToolMain->Activate();  // UIAnimPannel가 관련된 창 모두 같이 끄고 닫음 처리
        }

        ImGui::EndMenu();
    }
}

void UIMenuPannel::Level()
{
    if (ImGui::BeginMenu("Level"))
    {
        CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
        LEVEL_STATE State = pCurLevel->GetState();
        bool PlayEnable = false;
        bool PauseEnable = false;
        bool StopEnable = false;

        if (LEVEL_STATE::STOP == State || LEVEL_STATE::PAUSE == State)
            PlayEnable = true;
        if (LEVEL_STATE::PLAY == State)
            PauseEnable = true;
        if (LEVEL_STATE::PLAY == State || LEVEL_STATE::PAUSE == State)
            StopEnable = true;

        if (ImGui::MenuItem("Play", nullptr, nullptr, PlayEnable))
        {
            if (LEVEL_STATE::STOP == pCurLevel->GetState())
            {
                CLevelSaveLoad::SaveLevel(pCurLevel, L"level//temp.lv");
            }

            CLevelMgr::GetInst()->ChangeLevelState(LEVEL_STATE::PLAY);
        }

        if (ImGui::MenuItem("Pause", nullptr, nullptr, PauseEnable))
        {
            CLevelMgr::GetInst()->ChangeLevelState(LEVEL_STATE::PAUSE);
        }

        if (ImGui::MenuItem("Stop", nullptr, nullptr, StopEnable))
        {
            CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(L"level//temp.lv");
            CLevelMgr::GetInst()->ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

            // Inspector 의 타겟정보를 nullptr 로 되돌리기
            UIInspectorPannel* pInspector = (UIInspectorPannel*)CImGuiMgr::GetInst()->FindUI("##Inspector");
            pInspector->SetTargetObject(nullptr);
        }

        ImGui::EndMenu();
    }
}

void UIMenuPannel::GameObject()
{
    if (ImGui::BeginMenu("GameObject"))
    {
        if (ImGui::MenuItem("Create Empty Object", ""))
        {
            CGameObject* pNewObj = new CGameObject;
            pNewObj->SetName(L"New GameObject");
            pNewObj->AddComponent(new CTransform);
            GamePlayStatic::SpawnGameObject(pNewObj, 0);
        }
        ImGui::Separator();

        if (ImGui::BeginMenu("Component", ""))
        {
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}

            ImGui::EndMenu();
        }


        if (ImGui::BeginMenu("Script", ""))
        {
            vector<wstring> vecScriptName;
            CScriptMgr::GetScriptInfo(vecScriptName);

            for (size_t i = 0; i < vecScriptName.size(); ++i)
            {
                if (ImGui::MenuItem(WstrToStr(vecScriptName[i]).c_str()))
                {
                    UIInspectorPannel* inspector = (UIInspectorPannel*)CImGuiMgr::GetInst()->FindUI("##Inspector");
                    if (nullptr != inspector->GetTargetObject())
                    {
                        inspector->GetTargetObject()->AddComponent(CScriptMgr::GetScript(vecScriptName[i]));
                    }
                }
            }

            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
}

void UIMenuPannel::Asset()
{
    if (ImGui::BeginMenu("Asset"))
    {
        if (ImGui::MenuItem("Create Empty Material"))
        {
            wchar_t szPath[255] = {};
            wstring FilePath = CPathMgr::GetContentPath();

            // 중복 시 처리
            int num = 0;
            while (true)
            {
                swprintf_s(szPath, L"Material//New Material_%d.mat", num);
                if (!exists(FilePath + szPath))
                    break;
                ++num;
            }

            CMaterial* pMat = new CMaterial;
            pMat->SetName(szPath);
            pMat->Save(szPath);
            GamePlayStatic::AddAsset(pMat);
        }

        ImGui::EndMenu();
    }
}