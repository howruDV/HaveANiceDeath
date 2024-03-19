#pragma once
#include "CEntity.h"
// =======================================
// CConstBuffer: Obj의 상수 전달 버퍼
// =======================================
// - ex) Transform 관련된 상수 정보 전달 
// - object의 이동 위해서 vertex를 직접 update하지 않고, 이동정보(offset), 즉 transform 보냄
// - vertex shader에서 vector를 해당 값만큼 이동

class CConstBuffer :
    public CEntity
{
private:
    D3D11_BUFFER_DESC       m_Desc;
    ComPtr<ID3D11Buffer>    m_CB;

    UINT                    m_ElementSize;
    UINT                    m_ElementCount;

    CB_TYPE                 m_Type;

public:
    int Create(UINT _ElementSize, UINT _ElementCount);
    void SetData(void* _Src, UINT _ElementCount = 0);
    void UpdatePipeline();
    void UpdatePipeline_CS();

public:
    CLONE_DISABLE(CConstBuffer);
    CConstBuffer(CB_TYPE _Type);
    ~CConstBuffer();
};

