#include "pch.h"
#include "CBloomDownScaling.h"

CBloomDownScaling::CBloomDownScaling()
	: CComputeShader(32, 1, 1)
{
	Create(L"shader\\stdBloom.fx", "CS_BloomDownScaling");
}

CBloomDownScaling::~CBloomDownScaling()
{
}

int CBloomDownScaling::UpdatePipeline()
{
	if (!m_TargetTex.Get() || !m_SourceTex.Get())
		return E_FAIL;

	m_Const.iArr[0] = m_TargetTex->GetWidth();
	m_Const.iArr[1] = m_TargetTex->GetHeight();
	m_Const.bTex[0] = true;
	m_Const.bTex[1] = true;

	if (FAILED(m_SourceTex->UpdateCS_UAV(0)))
		return E_FAIL;

	if (FAILED(m_TargetTex->UpdateCS_UAV(1)))
		return E_FAIL;

	return S_OK;
}

void CBloomDownScaling::UpdateGroupCount()
{
	// 픽셀 하나당 thread 하나씩 배정하는 CS
	UINT width = m_TargetTex->GetWidth();
	UINT height = m_TargetTex->GetHeight();
	SetGroupX(1 + width / m_ThreadX);
	SetGroupY(1 + height / m_ThreadY);
	SetGroupZ(1);
}

void CBloomDownScaling::Clear()
{
	m_SourceTex->ClearCS_UAV();
	m_TargetTex->ClearCS_UAV();
}
