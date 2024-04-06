#include "pch.h"
#include "CCombine.h"

CCombine::CCombine()
	: CComputeShader(32, 32, 1)
	, m_fStrength(0.85f)
{
	Create(L"shader\\combind.fx", "CS_Combind");
}

CCombine::~CCombine()
{
}


int CCombine::UpdatePipeline()
{
	if (nullptr == m_RenderTargetCopyTex || nullptr == m_BloomTex || nullptr == m_RenderTarget)
		return E_FAIL;

	m_Const.fArr[0] = m_fStrength;

	// 텍스쳐 t0 에 바인딩
	if (FAILED(m_RenderTargetCopyTex->UpdateCS_SRV(0)))
	{
		return E_FAIL;
	}

	if (FAILED(m_BloomTex->UpdateCS_SRV(1)))
	{
		return E_FAIL;
	}

	if (FAILED(m_RenderTarget->UpdateCS_UAV(0)))
	{
		return E_FAIL;
	}


	return S_OK;
}

void CCombine::UpdateGroupCount()
{
	// 스레드 그룹 수 체크
	UINT width = m_RenderTarget->GetWidth();
	UINT height = m_RenderTarget->GetHeight();

	SetGroupX(1 + width / m_ThreadX);
	SetGroupY(1 + height / m_ThreadY);
	SetGroupZ(1);
}

void CCombine::Clear()
{
	m_RenderTargetCopyTex->ClearCS_SRV();
	m_BloomTex->ClearCS_SRV();
	m_RenderTarget->ClearCS_UAV();
}