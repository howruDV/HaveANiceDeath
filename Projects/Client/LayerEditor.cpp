#include "pch.h"
#include "LayerEditor.h"

#include "CImGuiMgr.h"

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>

LayerEditor::LayerEditor()
	:UI("LayerEditor", "##LayerEditor")
{
	Deactivate();
}

LayerEditor::~LayerEditor()
{
}


void LayerEditor::tick()
{
}

void LayerEditor::render_update()
{
	
	vector<string> LayerName = CImGuiMgr::GetInst()->GetLayerName();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	
	if (pCurLevel == nullptr)
		return;


	for (int i = 0; i < LayerName.size(); ++i)
	{
		string LayerNum = std::to_string(i);

		char szLayer[255] = {};
		sprintf(szLayer, "##Layer%s", LayerNum.c_str());

		string CurLayerName = LayerName[i];

		size_t end_pos = CurLayerName.find("]");
		if (end_pos != std::string::npos)
		{
			CurLayerName = CurLayerName.substr(end_pos+2);
		}

		char szLayerName[128] = {};
		strcpy(szLayerName, CurLayerName.c_str());

		ImGui::Text(LayerNum.c_str()); ImGui::SameLine(100);
		ImGui::InputText(szLayer, szLayerName, 128);


		pCurLevel->GetLayer(i)->SetName(StrToWstr(szLayerName));
	}

	CImGuiMgr::GetInst()->LoadLayerName();

}

