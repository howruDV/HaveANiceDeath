#include "pch.h"
#include "CTexture.h"
#include "CDevice.h"

CTexture::CTexture(bool _Engine)
	: CAsset(ASSET_TYPE::TEXTURE, _Engine)
	, m_Desc{}
{
}

CTexture::~CTexture()
{
}

// image file load
int CTexture::Load(const wstring& _strFilePath)
{
	// 1. 이미지 파일 ScratchImage로 로드
	HRESULT hr = S_OK;
	wchar_t szExt[20] = {};
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 20);

	if (!wcscmp(szExt, L".dds") || !wcscmp(szExt, L".DDS"))
		hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image);
	else if (!wcscmp(szExt, L".tga") || !wcscmp(szExt, L"TGA"))
		hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
	else
		hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, nullptr, m_Image); // png, bmp, jpg, jpeg ...
		
	if FAILED(hr)
		return E_FAIL;

	// 2. DX texture 객체, ShaderResourceView 생성
	// - 2.1. DX texture 객체 생성 (ID3D11Texture2D)
	// - 2.1.1. 로딩한 데이터 전달 (Image File -> ScratchImage -> ID3D11Texture2D)
	// - 2.2. 생성한 texture로 ShaderResourceView 생성 (Image File -> ScratchImage -> ID3D11Texture2D -> ShaderResourceView)
	CreateShaderResourceView(DEVICE, m_Image.GetImages(), m_Image.GetImageCount(), m_Image.GetMetadata(), m_SRV.GetAddressOf());

	// ShaderResourceView로 (SRV의) texture 객체를 알아냄
	m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());

	// texture 객체로 사용될 때 사용된 옵션(desc) 정보를 가져옴
	m_Tex2D->GetDesc(&m_Desc);

	return S_OK;
}

int CTexture::Create(UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _USAGE)
{
	m_Desc.Format = _Format;
	m_Desc.Width = _Width;
	m_Desc.Height = _Height;
	m_Desc.BindFlags = _BindFlag;
	m_Desc.Usage = _USAGE;
	if (_USAGE == D3D11_USAGE_DYNAMIC)
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;
	m_Desc.MipLevels = 1;
	m_Desc.MiscFlags = 0;
	m_Desc.ArraySize = 1;

	if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
		return E_FAIL;

	// bind flag가 enum이 아닌 UINT인 이유: bit조합으로 사용
	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		if (FAILED(DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf())))
			return E_FAIL;
	}
	else
	{
		if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			if (FAILED(DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf())))
				return E_FAIL;
		}

		if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			if (FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf())))
				return E_FAIL;
		}

		if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			if (FAILED(DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf())))
				return E_FAIL;
		}

	}

	return S_OK;
}

int CTexture::Create(ComPtr<ID3D11Texture2D> _tex2D)
{
	assert(_tex2D.Get());

	m_Tex2D = _tex2D;
	m_Tex2D->GetDesc(&m_Desc);

	// bind flag가 enum이 아닌 UINT인 이유: bit조합으로 사용
	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		if (FAILED(DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf())))
			return E_FAIL;
	}
	else
	{
		if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			if (FAILED(DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf())))
				return E_FAIL;
		}

		if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			if (FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf())))
				return E_FAIL;
		}

		if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			if (FAILED(DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf())))
				return E_FAIL;
		}

	}

	return S_OK;
}

// Rendering Pipeline으로 넘김
void CTexture::UpdatePipeline(int _registerNum)
{
	CONTEXT->VSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
}

// Rendering Pipeline 초기화
void CTexture::ClearPipeline(int _registerNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	CONTEXT->VSSetShaderResources(_registerNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_registerNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_registerNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_registerNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_registerNum, 1, &pSRV);
}

int CTexture::UpdateCS_SRV(int _registerNum)
{
	if (!m_SRV)
		return E_FAIL;

	m_RecentCSRegNum_SRV = _registerNum;
	CONTEXT->CSSetShaderResources(_registerNum, 1, m_SRV.GetAddressOf());
	return S_OK;
}

int CTexture::UpdateCS_UAV(int _registerNum)
{
	if (!m_UAV)
		return E_FAIL;

	m_RecentCSRegNum_UAV = _registerNum;
	UINT i = -1; // -1 값은 현재 오프셋을 유지함
	CONTEXT->CSSetUnorderedAccessViews(_registerNum, 1, m_UAV.GetAddressOf(), &i);
	return S_OK;
}

void CTexture::ClearCS_SRV()
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->CSSetShaderResources(m_RecentCSRegNum_SRV, 1, &pSRV);
}

void CTexture::ClearCS_UAV()
{
	ID3D11UnorderedAccessView* pUAV = nullptr;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(m_RecentCSRegNum_UAV, 1, &pUAV, &i);
}

FPixel* CTexture::GetPixels()
{
	if (!m_Image.GetPixels())										// 픽셀의 빨강, 녹색, 파랑(RGB) 색 값 arr return
		CaptureTexture(DEVICE, CONTEXT, m_Tex2D.Get(), m_Image);	// Captures a Direct3D render target and returns an image
	return (FPixel*)m_Image.GetPixels();
}
