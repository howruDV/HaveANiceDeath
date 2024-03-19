#include "pch.h"
#include "UITexture.h"
#include <Engine/CTexture.h>

UITexture::UITexture()
	: UIAsset("Texture", "##Texture", ASSET_TYPE::TEXTURE)
{
}

UITexture::~UITexture()
{
}

void UITexture::render_update()
{
    if (not GetAsset().Get())
        return;

	UIAsset::render_update();

    Ptr<CTexture> pTex = (CTexture*)GetAsset().Get();
    string strPath = string(pTex->GetRelativePath().begin(), pTex->GetRelativePath().end());

    ImGui::Text("Texture");
    ImGui::SameLine();
    ImGui::InputText("##TexName", (char*)strPath.c_str(), strPath.length(), ImGuiInputTextFlags_ReadOnly);

    // resolution
    float Width = (float)pTex->GetWidth();
    float Height = (float)pTex->GetHeight();

    ImGui::Text("Width ");
    ImGui::SameLine();
    ImGui::InputFloat("##TextureWidth", &Width, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);

    ImGui::Text("Height");
    ImGui::SameLine();
    ImGui::InputFloat("##TextureHeight", &Height, 0.f, 0.f, "%.3f", ImGuiInputTextFlags_ReadOnly);
    
    // image
    static bool use_text_color_for_tint = false;
    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
    ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
    ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
    ImGui::Image(pTex->GetSRV().Get(), ImVec2(200, 200), uv_min, uv_max, tint_col, border_col);

}