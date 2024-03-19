#include "pch.h"
#include "CStructuredBuffer.h"
#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
    : m_Type(SB_TYPE::READ_ONLY)
    , m_bSysMemMove(false)
    , m_ElementCount(0)
    , m_ElementSize(0)
    , m_RecentSRV(0)
    , m_RecentUAV(0)
{
}

CStructuredBuffer::CStructuredBuffer(const CStructuredBuffer& _OriginBuffer)
    : m_ElementSize(_OriginBuffer.m_ElementSize)
    , m_ElementCount(_OriginBuffer.m_ElementCount)
    , m_Type(_OriginBuffer.m_Type)
    , m_bSysMemMove(_OriginBuffer.m_bSysMemMove)
    , m_RecentSRV(0)   // 최근에 바인딩한 t레지스터 번호
    , m_RecentUAV(0) // 최근에 바인딩한 u레지스터 번호
{
    Create(m_ElementSize, m_ElementCount, m_Type, m_bSysMemMove);
}

CStructuredBuffer::~CStructuredBuffer()
{
}

int CStructuredBuffer::Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE _Type, bool _bSysMemMove, void* _pSysMem)
{
    // 구조화버퍼 크기: 반드시 16배수여야 함
    assert(!(_ElementSize % 16));

    // buffer 초기화
    m_SB = nullptr;
    m_SRV = nullptr;
    m_SB_Read = nullptr;
    m_SB_Write = nullptr;

    m_Type = _Type;
    m_ElementSize = _ElementSize;
    m_ElementCount = _ElementCount;
    m_bSysMemMove = _bSysMemMove;

    // buffer create
    D3D11_BUFFER_DESC tDesc = {};
    tDesc.ByteWidth = m_ElementSize * m_ElementCount;
    tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;               // Texture Register로 전달하므로
    if (m_Type == SB_TYPE::READ_WRITE)
        tDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
    tDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;    // Structured Buffer Flag
    tDesc.StructureByteStride = m_ElementSize;
    tDesc.Usage = D3D11_USAGE_DEFAULT;                          // Strcutured Buffer은 Usage가 default여야만 생성됨;;
    tDesc.CPUAccessFlags = 0;

    // create buffer
    HRESULT hr = E_FAIL;
    if (!_pSysMem)
        hr = DEVICE->CreateBuffer(&tDesc, nullptr, m_SB.GetAddressOf());
    else
    {
        D3D11_SUBRESOURCE_DATA tSub = {};
        tSub.pSysMem = _pSysMem;
        hr = DEVICE->CreateBuffer(&tDesc, &tSub, m_SB.GetAddressOf());
    }

    if (FAILED(hr)) return E_FAIL;

    // create shader resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;         // texture가 아닌 buffer임을 명시
    SRVDesc.Buffer.NumElements = m_ElementCount;
    
    hr = DEVICE->CreateShaderResourceView(m_SB.Get(), &SRVDesc, m_SRV.GetAddressOf());
    if (FAILED(hr)) return E_FAIL;

    // create unordered access view
    if (_Type == SB_TYPE::READ_WRITE)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
        UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        UAVDesc.Buffer.NumElements = m_ElementCount;

        hr = DEVICE->CreateUnorderedAccessView(m_SB.Get(), &UAVDesc, m_UAV.GetAddressOf());
        if (FAILED(hr)) return E_FAIL;
    }

    // system memory access 위한 ReadWrite Buffer 생성
    if (m_bSysMemMove)
    {
        // 쓰기용 버퍼
        tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        tDesc.Usage = D3D11_USAGE_DYNAMIC;
        tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        hr = DEVICE->CreateBuffer(&tDesc, nullptr, m_SB_Write.GetAddressOf());
        if (FAILED(hr)) return E_FAIL;

        // 읽기용 버퍼
        tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        tDesc.Usage = D3D11_USAGE_DEFAULT;
        hr = DEVICE->CreateBuffer(&tDesc, nullptr, m_SB_Read.GetAddressOf());
        if (FAILED(hr)) return E_FAIL;
    }

    return S_OK;
}

void CStructuredBuffer::UpdatePipeline(UINT _RegisterNum)
{
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

int CStructuredBuffer::UpdateCS_SRV(UINT _RegisterNum)
{
    if (!m_SRV)
        return E_FAIL;

    m_RecentSRV = _RegisterNum;
    CONTEXT->CSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());

    return S_OK;
}

int CStructuredBuffer::UpdateCS_UAV(UINT _RegisterNum)
{
    if (!m_UAV)
        return E_FAIL;

    UINT i = -1;
    m_RecentUAV = _RegisterNum;
    CONTEXT->CSSetUnorderedAccessViews(_RegisterNum, 1, m_UAV.GetAddressOf(), &i);
   
    return S_OK;
}

void CStructuredBuffer::Clear(UINT _RegisterNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	CONTEXT->VSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, &pSRV);
}

void CStructuredBuffer::ClearCS_SRV()
{
    ID3D11ShaderResourceView* pSRV = nullptr;
    CONTEXT->CSSetShaderResources(m_RecentSRV, 1, &pSRV);
}

void CStructuredBuffer::ClearCS_UAV()
{
    ID3D11UnorderedAccessView* pUAV = nullptr;
    UINT i = -1;
    CONTEXT->CSSetUnorderedAccessViews(m_RecentUAV, 1, &pUAV, &i);
}

void CStructuredBuffer::SetData(void* _SysMem, UINT _ElementCount)
{
    assert(m_bSysMemMove);

    if (_ElementCount == 0)
        _ElementCount = m_ElementCount;

    // 입력 데이터가 구조화버퍼보다 크면 버퍼 재생성
    if (m_ElementCount < _ElementCount)
        Create(m_ElementSize, _ElementCount, m_Type, m_bSysMemMove, nullptr);

    // system memory -> write buffer
    D3D11_MAPPED_SUBRESOURCE tSub = {};
    CONTEXT->Map(m_SB_Write.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
    memcpy(tSub.pData, _SysMem, m_ElementSize * _ElementCount);
    CONTEXT->Unmap(m_SB_Write.Get(), 0);

    // write buffer -> structured buffer (main buffer)
    CONTEXT->CopyResource(m_SB.Get(), m_SB_Write.Get());
}

void CStructuredBuffer::GetData(void* _Dest, UINT _ElementCount)
{
    assert(m_bSysMemMove);

    if (_ElementCount == 0)
        _ElementCount = m_ElementCount;

    // structured buffer(main buffer) -> read buffer
    CONTEXT->CopyResource(m_SB_Read.Get(), m_SB.Get());

    // read buffer -> system memory
    D3D11_MAPPED_SUBRESOURCE tSub = {};
    CONTEXT->Map(m_SB_Read.Get(), 0, D3D11_MAP_READ, 0, &tSub);
    memcpy(_Dest, tSub.pData, m_ElementSize * _ElementCount);
    CONTEXT->Unmap(m_SB_Read.Get(), 0);
}