#include "pch.h"
#include "UIAnimPreview.h"
#include "UIAnimPannel.h"
#include "UIAnimDetail.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CAnim.h>

UIAnimPreview::UIAnimPreview()
	: UI("Preview", "##Animation Tool_Preview")
	, m_FrmSize(0)
	, m_CurFrmIdx(-1)
	, m_AccTime(0)
{
	Deactivate();
}

UIAnimPreview::~UIAnimPreview()
{
}

void UIAnimPreview::begin()
{
	m_DetailPannel = (UIAnimDetail*)CImGuiMgr::GetInst()->FindUI("##Animation Tool_Detail");
	m_MainPannel = (UIAnimPannel*)CImGuiMgr::GetInst()->FindUI("##Animation Tool_Main");
}

void UIAnimPreview::tick()
{
	if (not m_AtlasTex.Get() || m_CurFrmIdx == -1)
		return;

	// frame Àç»ý
	m_AccTime += DT_ENGINE;
	if (m_AccTime > (*m_CurFrm).fDuration)
	{
		++m_CurFrmIdx;

		if (m_CurFrmIdx >= m_FrmSize)
		{
			m_CurFrmIdx = 0;
		}

		m_CurFrm = &m_DetailPannel->GetFrmByIdx(m_CurFrmIdx);
		m_AccTime = 0.f;
	}

	// detect close
	if (m_bPrevActive && not IsActivate())
	{
		m_MainPannel->Deactivate();
	}
	m_bPrevActive = IsActivate();
}

void UIAnimPreview::render_update()
{
	if (not m_AtlasTex.Get() || m_CurFrmIdx == -1)
		return;

	// draw img
	ImGui::BeginChild("##Prevew Image");

	ImVec2 Size = ImGui::GetWindowContentRegionMax();
	ImVec2 calcSize;
	if (m_MainPannel->GetGridSize() != ImVec2(0, 0))
	{
		ImVec2 GridSize = m_MainPannel->GetGridSize();
		float ratio = GridSize.x / GridSize.y;
		calcSize = Size;
		calcSize.y = 1 / ratio * calcSize.x;
	}
	else
	{
		calcSize.x = m_CurFrm->vBackgroundSize.x * m_AtlasTex->GetWidth();
		calcSize.y = m_CurFrm->vBackgroundSize.y * m_AtlasTex->GetHeight();
	}

	if (calcSize.y > Size.y)
	{
		float ratio = Size.y / calcSize.y;
		calcSize.y = Size.y;
		calcSize.x *= ratio;
	}

	ImVec2 LeftTopUV = ImVec2(m_CurFrm->vLeftTopUV.x, m_CurFrm->vLeftTopUV.y) + ImVec2(m_CurFrm->vOffsetUV.x, m_CurFrm->vOffsetUV.x);
	ImVec2 RightBottomUV = LeftTopUV + ImVec2(m_CurFrm->vCutSizeUV.x, m_CurFrm->vCutSizeUV.y) + ImVec2(m_CurFrm->vOffsetUV.x, m_CurFrm->vOffsetUV.x);
	
	ImGui::Image(m_AtlasTex->GetSRV().Get(), calcSize, LeftTopUV, RightBottomUV);
	
	ImGui::EndChild();
}

void UIAnimPreview::Clear()
{
	m_AtlasTex = nullptr;
	m_CurFrm = nullptr;
	m_CurFrmIdx = -1;
	m_FrmSize = 0;
	m_AccTime = 0;
}
