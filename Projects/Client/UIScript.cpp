#include "pch.h"
#include "UIScript.h"
#include "UIParam.h"

#include <Scripts/CScriptMgr.h>

UIScript::UIScript()
	: UIComponent("Script", "##Script", COMPONENT_TYPE::SCRIPT)
	, m_TargetScript(nullptr)
{
}

UIScript::~UIScript()
{
}

void UIScript::SetScript(CScript* _Script)
{
	m_TargetScript = _Script;

	wstring strScriptName = CScriptMgr::GetScriptName(_Script);
	SetName(WstrToStr(strScriptName));
	SetTitle(WstrToStr(strScriptName));
	Activate();
}

void UIScript::render_update()
{
	if (not GetTargetObject())
		return;

	UIComponent::render_update();

	const vector<FScriptParam>& vecParam = m_TargetScript->GetScriptParam();

	for (size_t i = 0; i < vecParam.size(); ++i)
	{
		switch (vecParam[i].Type)
		{
		case SCRIPT_PARAM::INT:
			break;
		case SCRIPT_PARAM::FLOAT:
			UIParam::Param_FLOAT((float*)vecParam[i].pData, vecParam[i].Desc);
			break;
		case SCRIPT_PARAM::VEC2:
			break;
		case SCRIPT_PARAM::VEC3:
			break;
		case SCRIPT_PARAM::VEC4:
			break;
		case SCRIPT_PARAM::OBJECT:
			break;
		}
	}
}