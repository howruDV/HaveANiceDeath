#pragma once
#include "CEntity.h"
// =======================================
// CStructuredBuffer: size�� �������� �ʾҰų� ū data�� texture register�� �����ϴ� ����
// =======================================
// - ����ȭ���� ũ��: �ݵ�� 16������� ��
// - ��� ����: ���� �׷����ϴ� ������ ����(��Ÿ�� ����)

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
    ComPtr<ID3D11Buffer>                m_SB_Read;      // SB�� CPU Access �� ���� �������Ƿ� ReadWrite�� Buffer ���� ���� (GPU���� �����ϱ� ����)
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
    UINT GetElementSize() { return m_ElementSize; }
    UINT GetElementCount() { return m_ElementCount; }

public:
    CLONE(CStructuredBuffer);
    CStructuredBuffer();
    CStructuredBuffer(const CStructuredBuffer& _OriginBuffer);
    ~CStructuredBuffer();
};

