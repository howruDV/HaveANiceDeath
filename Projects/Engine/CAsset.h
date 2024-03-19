#pragma once
#include "CEntity.h"
#include "Ptr.h"

// =======================================
// CAsset: 모든 Asset이 상속하는 parent class
// =======================================
// - model 등 재활용 가능한 resource는 asset으로 취급
// - 메모리 관리: reference count 관리
// -- ref count가 1(자기자신)이 되면 날리며 실시간으로 최소한의 메모리 유지

class CAsset :
    public CEntity
{
private:
    wstring             m_Key;
    wstring             m_RelativePath;
    int                 m_RefCount;
    const ASSET_TYPE    m_Type;
    const bool          m_bEngineAsset;     // Engine용 에셋 (저장하지 않음)

private:
    virtual CAsset* Clone() = 0;
    virtual int Save(const wstring& _strRelativePath) { return E_FAIL; }
    virtual int Load(const wstring& _strFilePath) { return E_FAIL; } // private: Asset Mgr로만 공개
    
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