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
		case SCRIPT_PARAM::BOOL:
			UIParam::Param_BOOL((bool*)vecParam[i].pData, vecParam[i].Desc);
			break;
		case SCRIPT_PARAM::INT:
			UIParam::Param_INT((int*)vecParam[i].pData, vecParam[i].Desc);
			break;
		case SCRIPT_PARAM::FLOAT:
			UIParam::Param_FLOAT((float*)vecParam[i].pData, vecParam[i].Desc);
			break;
		case SCRIPT_PARAM::VEC2:
			UIParam::Param_VEC2((Vec2*)vecParam[i].pData, vecParam[i].Desc);
			break;
		case SCRIPT_PARAM::VEC3:
			UIParam::Param_VEC3((Vec3*)vecParam[i].pData, vecParam[i].Desc);
			break;
		case SCRIPT_PARAM::VEC4:
			UIParam::Param_VEC4((Vec4*)vecParam[i].pData, vecParam[i].Desc);
			break;
		case SCRIPT_PARAM::OBJECT:
			break;
		}
	}
}