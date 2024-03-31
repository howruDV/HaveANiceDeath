#include "pch.h"
#include "UIMaterial.h"
#include "UIParam.h"
#include "func_ImGUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CMaterial.h>
#include <Engine/CGraphicsShader.h>
#include <Engine/CTexture.h>

UIMaterial::UIMaterial()
	: UIAsset("Material", "##Material", ASSET_TYPE::MATERIAL)
{
}

UIMaterial::~UIMaterial()
{
}


void UIMaterial::render_update()
{
    if (not GetAsset().Get())
        return;

    UIAsset::render_update();

    // Texture Image
    Ptr<CMaterial> pMat = (CMaterial*)GetAsset().Get();
    Ptr<CGraphicsShader> pShader = pMat->GetShader();
    string strPath = string(pMat->GetRelativePath().begin(), pMat->GetRelativePath().end());
    string strShaderName;
    if (pShader.Get())
        strShaderName = string(pShader->GetKey().begin(), pShader->GetKey().end());

    TextBox("Material");
    ImGui::SameLine();
    ImGui::InputText("##TexName", (char*)strPath.c_str(), strPath.length(), ImGuiInputTextFlags_ReadOnly);

    TextBox("Shader  ");
    ImGui::SameLine();
    ImGui::InputText("##ShaderName", (char*)strShaderName.c_str(), strShaderName.length(), ImGuiInputTextFlags_ReadOnly);

    // Shader Parameter
    
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
    TextBox("Material Parameter");
    ImGui::Spacing(); ImGui::Spacing();
    
    if (!pShader.Get())
        return;

    const vector<FScalarParam>& vecScalarParam = pShader->GetScalarParam();
    for (size_t i = 0; i < vecScalarParam.size(); ++i)
    {
        switch (vecScalarParam[i].Type)
        {
        case SCALAR_PARAM::INT_0:
        case SCALAR_PARAM::INT_1:
        case SCALAR_PARAM::INT_2:
        case SCALAR_PARAM::INT_3:
            UIParam::Param_INT((int*)pMat->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc);
            break;
        case SCALAR_PARAM::FLOAT_0:
        case SCALAR_PARAM::FLOAT_1:
        case SCALAR_PARAM::FLOAT_2:
        case SCALAR_PARAM::FLOAT_3:
            UIParam::Param_FLOAT((float*)pMat->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc);
            break;
        case SCALAR_PARAM::VEC2_0:
        case SCALAR_PARAM::VEC2_1:
        case SCALAR_PARAM::VEC2_2:
        case SCALAR_PARAM::VEC2_3:
            UIParam::Param_VEC2((Vec2*)pMat->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc);
            break;
        case SCALAR_PARAM::VEC4_0:
        case SCALAR_PARAM::VEC4_1:
        case SCALAR_PARAM::VEC4_2:
        case SCALAR_PARAM::VEC4_3:
            UIParam::Param_VEC4((Vec4*)pMat->GetScalarParam(vecScalarParam[i].Type), vecScalarParam[i].Desc);
            break;
        case SCALAR_PARAM::MAT_0:
        case SCALAR_PARAM::MAT_1:
        case SCALAR_PARAM::MAT_2:
        case SCALAR_PARAM::MAT_3:
            break;
        }
    }

    const vector<FTexParam>& vecTexParam = pShader->GetTexParam();
    for (size_t i = 0; i < vecTexParam.size(); ++i)
    {
        Ptr<CTexture> pTex = pMat->GetTexParam(vecTexParam[i].Type);
        
        // case: 리스트 버튼을 누름
        if (UIParam::Param_TEXTURE(pTex, vecTexParam[i].Desc, this, (DELEGATE_1)&UIMaterial::SelectTexture))
            m_SelectTexParam = vecTexParam[i].Type;
        pMat->SetTexParam(vecTexParam[i].Type, pTex);
    }
}

void UIMaterial::SelectTexture(DWORD_PTR _dwData)
{
    string strTex = (char*)_dwData;
    wstring strTexName = StrToWstr(strTex);

    Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strTexName);
    Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
    pMtrl->SetTexParam(m_SelectTexParam, pTex);
}