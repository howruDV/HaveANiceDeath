#pragma once
#include "CEntity.h"
#include "Ptr.h"

// =======================================
// CAsset: ��� Asset�� ����ϴ� parent class
// =======================================
// - model �� ��Ȱ�� ������ resource�� asset���� ���
// - �޸� ����: reference count ����
// -- ref count�� 1(�ڱ��ڽ�)�� �Ǹ� ������ �ǽð����� �ּ����� �޸� ����

class CAsset :
    public CEntity
{
private:
    wstring             m_Key;
    wstring             m_RelativePath;
    int                 m_RefCount;
    const ASSET_TYPE    m_Type;
    const bool          m_bEngineAsset;     // Engine�� ���� (�������� ����)

private:
    virtual CAsset* Clone() = 0;
    virtual int Save(const wstring& _strRelativePath) { return E_FAIL; }
    virtual int Load(const wstring& _strFilePath) { return E_FAIL; } // private: Asset Mgr�θ� ����
    
    void AddRef() { ++m_RefCount; }
    void Release() { if (--m_RefCount <= 0) delete this; }

    // getter & setter
public:
    const wstring& GetKey() { return m_Key; }
    const wstring& GetRelativePath() { return m_RelativePath; }
    int GetRefCount() { return m_RefCount; }
    ASSET_TYPE GetType() { return m_Type; }
    bool IsEngineAsset() { return m_bEngineAsset; }

private:
    void SetKey(const wstring& _Key) { m_Key = _Key; }
    void SetRelativePath(const wstring& _RelativePath) { m_RelativePath = _RelativePath; }

public:
    CAsset(ASSET_TYPE _Type, bool _bEngineAsset);
    CAsset(const CAsset& _Origin);
    ~CAsset();

    friend class CAssetMgr;

    template <typename T>
    friend class Ptr;
};