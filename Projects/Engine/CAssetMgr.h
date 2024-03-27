#pragma once
#include "singleton.h"
#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CComputeShader.h"
#include "CPathMgr.h"
#include "CTexture.h"
#include "CMaterial.h"
#include "CPrefab.h"
#include "CSound.h"
#include "CFSM.h"

// =======================================
// AssetMgr: Asset(resource) 생성, 관리
// =======================================
// - Asset load: Asset mgr를 통해서만 가능

class CAsset;

class CAssetMgr :
    public CSingleton<CAssetMgr>
{
    SINGLE(CAssetMgr)

private:
    unordered_map<wstring, Ptr<CAsset>> m_hashAsset[(UINT)ASSET_TYPE::END]; // hash를 ASSET_TYPE마다 만들어 관리

private:
    void InitSound();
    void CreateDefaultMesh();
    void CreateDefaultGraphicsShader();
    void CreateDefaultComputeShader();
    void CreateDefaultMaterial();

public:
    void init();
    Ptr<CTexture> CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _Flag, D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);
    Ptr<CTexture> CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _tex2D);

public:
    const unordered_map<wstring, Ptr<CAsset>>& GetAssetsByType(ASSET_TYPE _Type) { return m_hashAsset[(UINT)_Type]; }
    void GetAssetName(ASSET_TYPE _Type, vector<string>& _out);

    template<typename T>
    const unordered_map<wstring, Ptr<CAsset>>& GetAssetsByType();

    template<typename T>
    Ptr<T> Load(const wstring& _strKey, const wstring& _strRelativePath); // asset load: asset mgr로 관리
    template<typename T>
    Ptr<T> Load(const wstring& _strKey);

    void AddAsset(const wstring& _strKey, CAsset* _Asset);
    template<typename T>
    void AddAsset(const wstring& _strKey, T* _Asset);

    template<typename T>
    Ptr<T> FindAsset(const wstring& _strKey);

private:
    template<typename T>
    void DeleteAsset(const wstring& _strKey);
    void DeleteAsset(ASSET_TYPE _Type, const wstring& _strKey);

    friend class CTaskMgr;
};

template<typename T>
ASSET_TYPE GetAssetType()
{
    //const type_info& info = typeid(T);  // typeid: 런타임에 개체 형식 확인 가능
    ASSET_TYPE Type = ASSET_TYPE::END;

    if constexpr (std::is_same_v<CMesh, T>)
        Type = ASSET_TYPE::MESH;
    else if constexpr (std::is_same_v<CTexture, T>)
        Type = ASSET_TYPE::TEXTURE;
    else if constexpr (std::is_same_v<CGraphicsShader, T>)
        Type = ASSET_TYPE::GRAPHICS_SHADER;
    else if constexpr (std::is_same_v<CComputeShader, T>)
        Type = ASSET_TYPE::COMPUTE_SHADER;
    else if constexpr (std::is_same_v<CMaterial, T>)
        Type = ASSET_TYPE::MATERIAL;
    if constexpr (std::is_same_v<CPrefab, T>)
        Type = ASSET_TYPE::PREFAB;
    if constexpr (std::is_same_v<CSound, T>)
        Type = ASSET_TYPE::SOUND;
    if constexpr (std::is_same_v<CFSM, T>)
        Type = ASSET_TYPE::FSM;

    return Type;
}



// ----------------------
// template imp.
// ----------------------
template<typename T>
inline const unordered_map<wstring, Ptr<CAsset>>& CAssetMgr::GetAssetsByType()
{
    ASSET_TYPE Type = GetAssetType<T>();
    return m_hashAsset[(UINT)Type];
}

template<typename T>
inline Ptr<T> CAssetMgr::Load(const wstring& _strKey, const wstring& _strRelativePath)
{
    // 1. find asset
    Ptr<T> pAsset = FindAsset<T>(_strKey);

    if (pAsset.Get())
        return (T*)pAsset.Get();

    // 2. load asset
    if constexpr (std::is_same_v<CFSM, T>)
        pAsset = new CFSM(nullptr);
    else
        pAsset = new T;
    wstring strFilePath = CPathMgr::GetContentPath() + _strRelativePath;

    if (FAILED(pAsset->Load(strFilePath)))
    {
        MessageBox(nullptr, L"Failed to Load Asset", L"Failed to Load Asset", MB_OK);
        pAsset = nullptr;
        return nullptr;
    }

    pAsset->SetKey(_strKey);
    pAsset->SetRelativePath(_strRelativePath);
    AddAsset<T>(_strKey, (T*)pAsset.Get());

    return (T*)pAsset.Get();
}

template<typename T>
inline Ptr<T> CAssetMgr::Load(const wstring& _strKey)
{
    return CAssetMgr::Load<T>(_strKey, _strKey);
}

template<typename T>
inline void CAssetMgr::AddAsset(const wstring& _strKey, T* _Asset)
{
    ASSET_TYPE Type = GetAssetType<T>();
    unordered_map<wstring, Ptr<CAsset>>::iterator iter = m_hashAsset[(UINT)Type].find(_strKey);

    if (iter != m_hashAsset[(UINT)Type].end())
    {
        MessageBoxA(nullptr, "Asset already exist! Overwriting the asset.", "Add Asset Warning", MB_OK);
        _Asset->SetKey(_strKey);
        _Asset->SetRelativePath(_strKey);
        m_hashAsset[(UINT)Type][_strKey] = _Asset;
    }
    else
    {
        _Asset->SetKey(_strKey);
        _Asset->SetRelativePath(_strKey);
        m_hashAsset[(UINT)Type].insert(make_pair(_strKey, _Asset));
    }
}

template<typename T>
inline Ptr<T> CAssetMgr::FindAsset(const wstring& _strKey)
{
    ASSET_TYPE Type = GetAssetType<T>();
    unordered_map<wstring, Ptr<CAsset>>::iterator iter = m_hashAsset[(UINT)Type].find(_strKey);

    if (iter == m_hashAsset[(UINT)Type].end())
        return nullptr;

    return (T*)(iter->second.Get());
}

template<typename T>
inline void CAssetMgr::DeleteAsset(const wstring& _strKey)
{
    ASSET_TYPE AssetType = GetAssetType<T>();
    unordered_map<wstring, Ptr<CAsset>>::iterator iter = m_hashAsset[(UINT)AssetType].find(_strKey);
    if (iter != m_hashAsset[(UINT)AssetType].end())
        return;

    m_hashAsset[(UINT)AssetType].erase(iter);
}
