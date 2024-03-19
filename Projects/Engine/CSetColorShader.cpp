#include "pch.h"
#include "CSetColorShader.h"

CSetColorShader::CSetColorShader()
	:CComputeShader(32, 32, 1)	// 임시 Group 수
{
	Create(L"shader\\setcolor.fx", "CS_SetColor");
}

CSetColorShader::~CSetColorShader()
{
}

int CSetColorShader::UpdatePipeline()
{
	if (!m_TargetTex.Get())
		return E_FAIL;

	m_Const.iArr[0] = m_TargetTex->GetWidth();
	m_Const.iArr[1] = m_TargetTex->GetHeight();
	m_Const.v4Arr[0] = m_Color;

	if (FAILED(m_TargetTex->UpdateCS_UAV(0)))
		return E_FAIL;

	return S_OK;
}

void CSetColorShader::UpdateGroupCount()
{
	// 픽셀 하나당 thread 하나씩 배정하는 CS
	UINT width = m_TargetTex->GetWidth();
	UINT height = m_TargetTex->GetHeight();
	SetGroupX(1 + width / m_ThreadX);
	SetGroupY(1 + height / m_ThreadY);
	SetGroupZ(1);
}

void CSetColorShader::Clear()
{
	m_TargetTex->ClearCS_UAV();
}
