#include "pch.h"
#include "UIParam.h"
#include "imgui.h"
#include "UIListPannel.h"
#include "func_ImGUI.h"
#include <Engine/CAssetMgr.h>

int UIParam::g_ID = 0;

bool UIParam::Param_INT(int* _Data, const string& _Desc)
{
	TextBox(_Desc.c_str());
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##int%d", g_ID++);
	if (ImGui::InputInt(szID, _Data))
	{
		return true;
	}
    return false;
}

bool UIParam::Param_FLOAT(float* _Data, const string& _Desc)
{
	TextBox(_Desc.c_str());
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##float%d", g_ID++);
	if (ImGui::InputFloat(szID, _Data))
	{
		return true;
	}

	return false;
}

bool UIParam::Param_VEC2(Vec2* _Data, const string& _Desc)
{
	TextBox(_Desc.c_str());
	ImGui::SameLine();

	float arrFloat[2] = { _Data->x, _Data->y };
	char szID[256] = {};
	sprintf_s(szID, "##Vec2%d", g_ID++);
	if (ImGui::InputFloat2(szID, arrFloat))
	{
		_Data->x = arrFloat[0];
		_Data->y = arrFloat[1];
		return true;
	}

	return false;
}

bool UIParam::Param_VEC3(Vec3* _Data, const string& _Desc)
{
	TextBox(_Desc.c_str());
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##Vec3%d", g_ID++);
	if (ImGui::InputFloat3(szID, *_Data))
	{
		return true;
	}

	return false;
}

bool UIParam::Param_VEC4(Vec4* _Data, const string& _Desc)
{
	TextBox(_Desc.c_str());
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##Vec4%d", g_ID++);
	if (ImGui::InputFloat4(szID, *_Data))
	{
		return true;
	}

	return false;
}

bool UIParam::Param_TEXTURE(Ptr<CTexture>& _Texture, const string& _Desc, UI* _Inst, DELEGATE_1 _Func)
{
	TextBox(_Desc.c_str());
	ImGui::SameLine();

	char szID[256] = {};
	sprintf_s(szID, "##Texture%d", g_ID++);
	ImTextureID texid = nullptr;
	string strTexKey;

	if (_Texture.Get())
	{
		texid = _Texture->GetSRV().Get();
		strTexKey = string(_Texture->GetKey().begin(), _Texture->GetKey().end());
	}

	// texture name
	ImGui::InputText(szID, (char*)strTexKey.c_str(), strTexKey.length(), ImGuiInputTextFlags_ReadOnly);

	// drop check
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			DWORD_PTR data = *((DWORD_PTR*)payload->Data);
			CAsset* pAsset = (CAsset*)data;
			if (ASSET_TYPE::TEXTURE == pAsset->GetType())
			{
				_Texture = (CTexture*)pAsset;
			}
		}

		ImGui::EndDragDropTarget();
	}

	// image
	static bool use_text_color_for_tint = false;
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);
	ImVec4 tint_col = use_text_color_for_tint ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
	ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
	ImGui::Image(texid, ImVec2(150, 150), uv_min, uv_max, tint_col, border_col);

	// click func
	if (_Inst && _Func)
	{
		ImGui::SameLine();
		sprintf_s(szID, "##TexBtn%d", g_ID++);
		if (ImGui::Button(szID, ImVec2(20, 20)))
		{
			// ¸®½ºÆ® UI
			UIListPannel* pListUI = (UIListPannel*)CImGuiMgr::GetInst()->FindUI("##List");

			vector<string> vecTexName;
			CAssetMgr::GetInst()->GetAssetName(ASSET_TYPE::TEXTURE, vecTexName);

			pListUI->AddString(vecTexName);
			pListUI->SetDBClickeDelegate(_Inst, (DELEGATE_1)_Func);
			pListUI->Activate();

			return true;
		}
	}

	return false;
}