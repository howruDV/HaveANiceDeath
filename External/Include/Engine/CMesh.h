#pragma once
#include "CAsset.h"

// =======================================
// CMesh: model(mesh model) class
// =======================================

class CMesh :
    public CAsset
{
private:
    ComPtr<ID3D11Buffer>    m_VB;	// vertex buffer: �ʿ� ��ŭ�� (�ߺ�X)
    ComPtr<ID3D11Buffer>    m_IB;	// index buffer: rendering�� ������ index ����

    D3D11_BUFFER_DESC       m_VBDesc;
    D3D11_BUFFER_DESC       m_IBDesc;

    UINT                    m_VtxCount;
    UINT                    m_IdxCount;

    void*                   m_VtxSysMem;    // setting�� Buffer�� ���� ����
    void*                   m_IdxSysMem;

private:
    void UpdatePipeline();

public:
    int Create(void* _Vtx, UINT _VtxCount, void* _Idx, UINT _IdxCount);
    void render();
    void renderInstanced(UINT _Count);

public:
    CLONE_DISABLE(CMesh);
    CMesh(bool _Engine = false);
    ~CMesh();
};

