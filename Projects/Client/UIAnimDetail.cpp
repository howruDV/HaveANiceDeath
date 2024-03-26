#include "pch.h"
#include "UIAnimDetail.h"
#include "UIAnimPannel.h"

#include "Engine/CAnimator2D.h"
#include "Engine/CAnim.h"

UIAnimDetail::UIAnimDetail()
	: UI("Detail", "##Animation Tool_Detail")
	, m_AnimName("Untitled")
	, m_NumCellsX(10)
	, m_NumCellsY(1)
	, m_FPS(32)
{
	buf1[0] = '1'; buf1[1] = '0';
	buf2[0] = '1';
	buf3[0] = '3'; buf3[1] = '2';
	Deactivate();
}

UIAnimDetail::~UIAnimDetail()
{
}

void UIAnimDetail::begin()
{
	m_MainPannel = (UIAnimPannel*)CImGuiMgr::GetInst()->FindUI("##Animation Tool_Main");
	m_PreviewPannel = (UIAnimPreview*)CImGuiMgr::GetInst()->FindUI("##Animation Tool_Preview");
}

void UIAnimDetail::tick()
{
	// detect close
	if (m_bPrevActive && not IsActivate())
	{
		m_MainPannel->Deactivate();
	}
	m_bPrevActive = IsActivate();
}

void UIAnimDetail::render_update()
{
	// ---------------------이름------------------------
	if (ImGui::CollapsingHeader("Naiming", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Animation Name "); ImGui::SameLine(); ImGui::InputText("##AnimationNaming", m_AnimName, 32);
	}

	// ---------------------칸수------------------------
	if (ImGui::CollapsingHeader("Grid", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Num Cells X    "); ImGui::SameLine(); ImGui::InputText("##AtlasNumCellsX", buf1, 10, ImGuiInputTextFlags_CharsDecimal);
		ImGui::Text("Num Cells Y    "); ImGui::SameLine(); ImGui::InputText("##AtlasNumCellsY", buf2, 10, ImGuiInputTextFlags_CharsDecimal);
		m_NumCellsX = (string(buf1) == "") ? 0 : stoi(string(buf1));
		m_NumCellsY = (string(buf2) == "") ? 0 : stoi(string(buf2));
	}

	// ---------------------프레임------------------------
	if (ImGui::CollapsingHeader("Sprites", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// Frame Run 입력
		ImGui::Text("FPS            "); ImGui::SameLine(); ImGui::InputText("##AnimationFrame", buf3, 10, ImGuiInputTextFlags_CharsDecimal);
		m_FPS = (string(buf3) == "") ? 32 : stoi(string(buf3));
		ImGui::Separator();

		// 각 frame 별 정보 출력
		if (not m_Atlas.Get() || m_vecAnimUV.empty())
			return;

		for (int i = 0; i < m_vecAnimUV.size(); ++i)
		{
			// draw img
			ImVec2 UV;
			UV.x = m_vecAnimUV[i].vLeftTopUV.x;
			UV.y = m_vecAnimUV[i].vLeftTopUV.y;
			ImGui::Image(m_Atlas->GetSRV().Get(), ImVec2(70, 70), UV, UV + ImVec2(m_vecAnimUV[i].vCutSizeUV.x, m_vecAnimUV[i].vCutSizeUV.y), ImVec4(1, 1, 1, 1), ImVec4(0.8f, 0.8f, 0.8f, 0.8f));

			// offset input
			Vec3 vOffset = Vec3(m_vecAnimUV[i].vOffsetUV.x * (float)m_Atlas->GetWidth(), m_vecAnimUV[i].vOffsetUV.y * (float)m_Atlas->GetHeight(), 0.f);
			string ID = "##detail" + std::to_string(i);
			ImGui::SameLine();

			ImGui::BeginChild(ID.c_str(), ImVec2(0, 70));

			ImGui::Text("[ %i ]", i);
			ImGui::Text("Offset  "); ImGui::SameLine(); ImGui::DragFloat2(ID.c_str(), vOffset, 0.1f);

			// frame input
			int fFps = int(1.f / m_vecAnimUV[i].fDuration);
			ImGui::Text("FPS     "); ImGui::SameLine(); ImGui::DragInt(ID.c_str(), &fFps, 0.1f);

			ImGui::EndChild();
			ImGui::Separator();

			// current anim vec update (owenr: UIAnimDeatil)
			m_vecAnimUV[i].vOffsetUV = Vec2(vOffset.x / (float)m_Atlas->GetWidth(), vOffset.y / (float)m_Atlas->GetHeight());
			m_vecAnimUV[i].fDuration = 1.f / fFps;
		}
	}
}

void UIAnimDetail::Activate()
{
	if (m_MainPannel && not m_MainPannel->IsActivate())
		m_MainPannel->Activate();
	else
		UI::Activate();
}

void UIAnimDetail::Deactivate()
{
	if (m_MainPannel && m_MainPannel->IsActivate())
		m_MainPannel->Deactivate();
	else
		UI::Deactivate();
}

void UIAnimDetail::Clear()
{
	m_Atlas = nullptr;
	m_vecAnimUV.clear();
	sprintf_s(m_AnimName, "%s", "Untitled");
	m_NumCellsX = 10;
	m_NumCellsY = 1;
	sprintf_s(buf1, "%s", "10");
	sprintf_s(buf2, "%s", "1");
}

void UIAnimDetail::UpdateFrm(vector<FAnimFrm>& _vec)
{
	Vec2 AtlasSize = Vec2(m_MainPannel->GetAtlas().Get()->GetWidth(), m_MainPannel->GetAtlas().Get()->GetHeight());
	m_vecAnimUV.clear();

	for (int i = 0; i < _vec.size(); ++i)
	{
		// UV값으로 세팅
		FAnimFrm frm = {};
		frm.vCutSizeUV = Vec2(_vec[i].vCutSizeUV.x / (float)AtlasSize.x, _vec[i].vCutSizeUV.y / (float)AtlasSize.y);
		frm.vLeftTopUV = Vec2(_vec[i].vLeftTopUV.x / (float)AtlasSize.x, _vec[i].vLeftTopUV.y / (float)AtlasSize.y);
		frm.vOffsetUV = Vec2(_vec[i].vOffsetUV.x / (float)AtlasSize.x, _vec[i].vOffsetUV.y / (float)AtlasSize.y);
		frm.fDuration = 1.f / m_FPS;
		frm.vBackgroundSize = Vec2(_vec[i].vBackgroundSize.x / (float)AtlasSize.x, _vec[i].vBackgroundSize.y / (float)AtlasSize.y);

		m_vecAnimUV.push_back(frm);
	}
}