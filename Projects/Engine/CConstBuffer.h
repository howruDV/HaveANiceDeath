#pragma once
#include "CEntity.h"
// =======================================
// CConstBuffer: Obj�� ��� ���� ����
// =======================================
// - ex) Transform ���õ� ��� ���� ���� 
// - object�� �̵� ���ؼ� vertex�� ���� update���� �ʰ�, �̵�����(offset), �� transform ����
// - vertex shader���� vector�� �ش� ����ŭ �̵�

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

