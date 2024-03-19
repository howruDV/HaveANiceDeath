#include "pch.h"
#include "CGraphicsShader.h"
#include "CPathMgr.h"
#include "CDevice.h"

CGraphicsShader::CGraphicsShader()
	: CShader(ASSET_TYPE::GRAPHICS_SHADER)
	, m_Topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_RasterizerType(RS_TYPE::CULL_BACK)
	, m_BlendType(BS_TYPE::DEFAULT)
	, m_DSType(DS_TYPE::LESS)
	, m_Domain(SHADER_DOMAIN::DOMAIN_MASKED)
{

}

CGraphicsShader::~CGraphicsShader()
{

}

int CGraphicsShader::CreateVertexShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	// --------------------------
	// (2) Vertex Shader 생성
	// --------------------------
	// HLSL Vertex Shader Function Compile
	// - HLSL code -> binary code
	// - flag: debug option으로 해야 실패 사유 나옴
	// - ErrBlob에 char 문자열로 실패 사유 저장 (file path가 잘못된경우 errBlob에도 저장 X)

	wstring strContentPath = CPathMgr::GetContentPath();
	wstring strFilePath = strContentPath + _strRelativePath;

	// HLSL Vertex Shader Function Compile
	if (FAILED(D3DCompileFromFile(strFilePath.c_str()
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "vs_5_0", D3DCOMPILE_DEBUG, 0
		, m_VSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Failed to Compile Shader", MB_OK);
		}

		return E_FAIL;
	}

	// Create Vertex Shader
	// - binary code 읽어둔 blob으로 생성
	DEVICE->CreateVertexShader(m_VSBlob->GetBufferPointer()
		, m_VSBlob->GetBufferSize(), nullptr
		, m_VS.GetAddressOf());

	// --------------------------
	// (3) Input Layout 생성
	// --------------------------
	// Layout 구조체
	// - Layout: Vertex의 구조 정보
	D3D11_INPUT_ELEMENT_DESC arrElement[3] = {};

	arrElement[0].InputSlot = 0;
	arrElement[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[0].SemanticName = "POSITION";		// sementic name (매칭할 이름)
	arrElement[0].SemanticIndex = 0;				// sementic name이 동일할 때 index
	arrElement[0].InstanceDataStepRate = 0;
	arrElement[0].AlignedByteOffset = 0;			// 시작위치
	arrElement[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

	arrElement[1].InputSlot = 0;
	arrElement[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[1].SemanticName = "COLOR";
	arrElement[1].SemanticIndex = 0;
	arrElement[1].InstanceDataStepRate = 0;
	arrElement[1].AlignedByteOffset = 12;
	arrElement[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	arrElement[2].InputSlot = 0;
	arrElement[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	arrElement[2].SemanticName = "TEXCOORD";
	arrElement[2].SemanticIndex = 0;
	arrElement[2].InstanceDataStepRate = 0;
	arrElement[2].AlignedByteOffset = 28;
	arrElement[2].Format = DXGI_FORMAT_R32G32_FLOAT;

	// Create Layout
	DEVICE->CreateInputLayout(arrElement, 3
		, m_VSBlob->GetBufferPointer()
		, m_VSBlob->GetBufferSize()
		, m_Layout.GetAddressOf());

	return S_OK;
}

int CGraphicsShader::CreateHullShader()
{

	return 0;
}

int CGraphicsShader::CreateDomainShader()
{

	return 0;
}

int CGraphicsShader::CreateGeometryShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	wstring strContentPath = CPathMgr::GetContentPath();
	wstring strFilePath = strContentPath + _strRelativePath;

	// compile shader code
	if (FAILED(D3DCompileFromFile(strFilePath.c_str()
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "gs_5_0", D3DCOMPILE_DEBUG, 0
		, m_GSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Failed to Compile Shader", MB_OK);
		}

		return E_FAIL;
	}

	// create gemotery shader
	DEVICE->CreateGeometryShader(m_GSBlob->GetBufferPointer(), m_GSBlob->GetBufferSize(), nullptr, m_GS.GetAddressOf());

	return 0;
}

int CGraphicsShader::CreatePixelShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	// --------------------------
	// (4) Pixel Shader 생성
	// --------------------------
	// - VS에서 PS return값을 받음
	// - Rasterlizer: pixel이 각 Vertex로부터 떨어진 거리를 찾아 비율에 맞게 선형보간함
	// - ★★★ 즉, 어떤 type의 값이든지, pixel shader에는 각 정점에서 보간된 값이 들어옴 ★★★

	wstring strContentPath = CPathMgr::GetContentPath();
	wstring strFilePath = strContentPath + _strRelativePath;

	// Pixel Shader Function Compile
	if (FAILED(D3DCompileFromFile(strFilePath.c_str()
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "ps_5_0", D3DCOMPILE_DEBUG, 0
		, m_PSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf())))
	{
		if (m_ErrBlob)
		{
			char* pErrMsg = (char*)m_ErrBlob->GetBufferPointer();
			MessageBoxA(nullptr, pErrMsg, "Failed to Compile Shader", MB_OK);
		}

		return E_FAIL;
	}

	// Create Pixel Shader
	DEVICE->CreatePixelShader(m_PSBlob->GetBufferPointer()
		, m_PSBlob->GetBufferSize(), nullptr
		, m_PS.GetAddressOf());

	return S_OK;
}

int CGraphicsShader::UpdatePipeline()
{
	// rendering pipeline
	// - 세팅하지 않은 shader도 무조건 set
	// - 이전에 세팅된 shader를 밀어야 하기 때문
	CONTEXT->IASetInputLayout(m_Layout.Get());
	CONTEXT->IASetPrimitiveTopology(m_Topology);	// Vertex의 구조정보

	CONTEXT->RSSetState(CDevice::GetInst()->GetRState(m_RasterizerType).Get());
	CONTEXT->OMSetDepthStencilState(CDevice::GetInst()->GetDSState(m_DSType).Get(), 0);
	CONTEXT->OMSetBlendState(CDevice::GetInst()->GetBState(m_BlendType).Get(), nullptr, 0xffffffff);

	CONTEXT->VSSetShader(m_VS.Get(), nullptr, 0);
	CONTEXT->HSSetShader(m_HS.Get(), nullptr, 0);
	CONTEXT->DSSetShader(m_DS.Get(), nullptr, 0);
	CONTEXT->GSSetShader(m_GS.Get(), nullptr, 0);
	CONTEXT->PSSetShader(m_PS.Get(), nullptr, 0);

	return S_OK;
}
