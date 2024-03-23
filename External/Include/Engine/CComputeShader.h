#pragma once
#include "CShader.h"
#include "CStructuredBuffer.h"
// =======================================
// CComputeShader: ComputerShader가 상속하는 parent class
// =======================================

class CComputeShader :
    public CShader
{
private:
    ComPtr<ID3DBlob>            m_CSBlob;
    ComPtr<ID3D11ComputeShader> m_CS;
    UINT                        m_GroupX;
    UINT                        m_GroupY;
    UINT                        m_GroupZ;

protected:
    const UINT                  m_ThreadX;
    const UINT                  m_ThreadY;
    const UINT                  m_ThreadZ;
    FMatConst                   m_Const;

private:
    virtual int UpdatePipeline() = 0;
    virtual void UpdateGroupCount() = 0;
    virtual void Clear() = 0;

public:
    int Create(const wstring& _strRelativePath, const string& _strFuncName);
    void Execute();

protected:
    void SetGroup(UINT _X, UINT _Y, UINT _Z) { m_GroupX = _X; m_GroupY = _Y; m_GroupZ = _Z; }
    void SetGroupX(UINT _GroupX) { m_GroupX = _GroupX; }
    void SetGroupY(UINT _GroupY) { m_GroupY = _GroupY; }
    void SetGroupZ(UINT _GroupZ) { m_GroupZ = _GroupZ; }
    UINT GetGroupX() { return m_GroupX; }
    UINT GetGroupY() { return m_GroupY; }
    UINT GetGroupZ() { return m_GroupZ; }

public:
    CComputeShader(UINT _ThreadX, UINT _ThreadY, UINT _ThreadZ);
    ~CComputeShader();
};

