#include "pch.h"
#include "UIAsset.h"

UIAsset::UIAsset(const string& _strName, const string& _ID, ASSET_TYPE _Type)
	: UI(_strName, _ID)
	, m_Type(_Type)
{
}

UIAsset::~UIAsset()
{
}

void UIAsset::render()
{
	if (!IsActivate())
		return;

	if (ImGui::CollapsingHeader(ASSET_TYPE_STRING[(UINT)m_Type], ImGuiTreeNodeFlags_DefaultOpen))
	{
		render_update();
		const vector<UI*> vecChild = GetChilds();
		for (size_t i = 0; i < vecChild.size(); ++i)
			vecChild[i]->render();
	}
}

void UIAsset::render_update()
{
}

void UIAsset::SetAsset(Ptr<CAsset> _Asset)
{
	m_Asset = _Asset;

	if (!m_Asset.Get())
		Deactivate();
	else
	{
		if (_Asset->GetType() == m_Type)
			Activate();
		else
			Deactivate();
	}
}
