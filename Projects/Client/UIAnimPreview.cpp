#include "pch.h"
#include "UIAnimPreview.h"
#include "UIAnimPannel.h"
#include "UIAnimDetail.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CAnim.h>
#include <Engine/CKeyMgr.h>

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

	// frame ���
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

	// move pivot

}

void UIAnimPreview::render_update()
{
	if (not m_AtlasTex.Get() || m_CurFrmIdx == -1)
		return;

	// draw img
	ImGui::BeginChild("##Prevew Image");

	// size: grid
	static float previewScaling = 0.5f;
	ImVec2 atlasSize = ImVec2((float)m_AtlasTex->GetWidth(), (float)m_AtlasTex->GetHeight());
	ImVec2 Size = ImVec2(m_CurFrm->vCutSizeUV.x, m_CurFrm->vCutSizeUV.y) * atlasSize;
	Vec2 GlobalOffset = m_DetailPannel->GetGlobalOffset();
	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::IsWindowHovered()) 
	{
		// Ȯ��/���: ��
		if (io.MouseWheel != 0.0f)
		{
			float zoomSpeed = 0.05f;
			float zoomDelta = io.MouseWheel * zoomSpeed;
			previewScaling += zoomDelta;
			if (previewScaling < 0) previewScaling = 0;
		}

		// �ǹ��̵�: ���콺 �巡��
		if (ImGui::IsMouseDown(0) && ImGui::IsMouseDragging(0))
		{
			Vec2 DragScale = CKeyMgr::GetInst()->GetMouseDrag();
			DragScale.y *= -1;

			GlobalOffset -= DragScale;
			m_DetailPannel->SetGlobalOffset(GlobalOffset);
		}
	}
	Size *= previewScaling;

	// �׸��� ��ǥ: offset ��� (y�� ��ǥ �ݴ��)
	GlobalOffset.x /= atlasSize.x;
	GlobalOffset.y /= atlasSize.y;

	ImVec2 CenterPos = ImGui::GetWindowContentRegionMax() * 0.5f;
	ImVec2 ImgPos = CenterPos - (Size * 0.5) - ImVec2(m_CurFrm->vOffsetUV.x + GlobalOffset.x, -(m_CurFrm->vOffsetUV.y + GlobalOffset.y)) * atlasSize * previewScaling;
	ImVec2 LeftTopUV = ImVec2(m_CurFrm->vLeftTopUV.x, m_CurFrm->vLeftTopUV.y);
	ImVec2 RightBottomUV = LeftTopUV + ImVec2(m_CurFrm->vCutSizeUV.x, m_CurFrm->vCutSizeUV.y);
	
	ImGui::SetCursorPos(ImgPos);
	ImGui::Image(m_AtlasTex->GetSRV().Get(), Size, LeftTopUV, RightBottomUV);

	// cross hair
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	DrawCrosshair(ImGui::GetWindowPos() + CenterPos, 30.f, drawList);
	
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

void UIAnimPreview::DrawCrosshair(const ImVec2& center, float size, ImDrawList* drawList)
{
	float halfSize = size * 0.5f;
	ImVec2 topLeft = ImVec2(center.x - halfSize, center.y - halfSize);
	ImVec2 bottomRight = ImVec2(center.x + halfSize, center.y + halfSize);

	drawList->AddLine(ImVec2(topLeft.x, center.y), ImVec2(bottomRight.x, center.y), IM_COL32(0, 0, 0, 255), 6);
	drawList->AddLine(ImVec2(center.x, topLeft.y), ImVec2(center.x, bottomRight.y), IM_COL32(0, 0, 0, 255), 6);
	drawList->AddLine(ImVec2(topLeft.x, center.y), ImVec2(bottomRight.x, center.y), IM_COL32(255, 255, 255, 255), 3);
	drawList->AddLine(ImVec2(center.x, topLeft.y), ImVec2(center.x, bottomRight.y), IM_COL32(255, 255, 255, 255), 3);
}