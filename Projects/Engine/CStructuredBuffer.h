#pragma once
#include "CEntity.h"
// =======================================
// CStructuredBuffer: size가 정해지지 않았거나 큰 data를 texture register로 전달하는 버퍼
// =======================================
// - 구조화버퍼 크기: 반드시 16배수여야 함
// - 사용 예시: 현재 그려야하는 광원들 전달(런타임 결정)

enum class SB_TYPE
{
    READ_ONLY,  // SRV (default)
    READ_WRITE, // SRV + UAV
};

class CStructuredBuffer :
    public CEntity
{
private:
    SB_TYPE                             m_Type;
    ComPtr<ID3D11Buffer>                m_SB;
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;
    ComPtr<ID3D11Buffer>                m_SB_Read;      // SB는 CPU Access 및 수정 막았으므로 ReadWrite용 Buffer 따로 생성 (GPU에서 접근하기 위함)
    ComPtr<ID3D11Buffer>                m_SB_Write;
    UINT                                m_ElementSize;
    UINT                                m_ElementCount;
    bool                                m_bSysMemMove;
    UINT                                m_RecentSRV;
    UINT                                m_RecentUAV;

public:
    int Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE _Type, bool _bSysMemMove = false, void* _pSysMem = nullptr);
    void UpdatePipeline(UINT _RegisterNum);
    int UpdateCS_SRV(UINT _RegisterNum);
    int UpdateCS_UAV(UINT _RegisterNum);
    void Clear(UINT _RegisterNum);
    void ClearCS_SRV();
    void ClearCS_UAV();
    
public:
    void SetData(void* _SysMem, UINT _ElementCount = 0);
    void GetData(void* _Dest, UINT _ElementCount = 0);
    template <typename T> void GetData_ToVec(vector<T>& _Dest, UINT _ElementCount = 0);
    UINT GetElementSize() { return m_ElementSize; }
    UINT GetElementCount() { return m_ElementCount; }

public:
    CLONE(CStructuredBuffer);
    CStructuredBuffer();
    CStructuredBuffer(const CStructuredBuffer& _OriginBuffer);
    ~CStructuredBuffer();
};

#include "struct.h"
#include "CDevice.h"
template<typename T>
void CStructuredBuffer::GetData_ToVec(vector<T>& _Dest, UINT _ElementCount)
{
    assert(m_bSysMemMove);

    if (_ElementCount == 0)
        _ElementCount = m_ElementCount;
    if (_Dest.size() != _ElementCount)
        _Dest.resize(_ElementCount);

    // structured buffer(main buffer) -> read buffer
    CONTEXT->CopyResource(m_SB_Read.Get(), m_SB.Get());

    // read buffer -> system memory
    D3D11_MAPPED_SUBRESOURCE tSub = {};
    CONTEXT->Map(m_SB_Read.Get(), 0, D3D11_MAP_READ, 0, &tSub);

    for (int i=0; i<_Dest.size(); ++i)
        memcpy(&_Dest[i], tSub.pData, m_ElementSize);

    CONTEXT->Unmap(m_SB_Read.Get(), 0);
}
