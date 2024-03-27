#include "pch.h"
#include "CAssetMgr.h"

CAssetMgr::CAssetMgr()
{

}

CAssetMgr::~CAssetMgr()
{
	// Ptr 사용해 refcount 1이 되면 할당 해제하므로 AssetMgr가 종료시 처리하지 않아도 됨
}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _Flag, D3D11_USAGE _Usage)
{
	Ptr<CTexture> pTex = FindAsset<CTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new CTexture(true);
	if (FAILED(pTex->Create(_Width, _Height, _Format, _Flag, _Usage)))
	{
		MessageBox(nullptr, L"Failed to Create Texture", L"Faile to Create Resource", MB_OK);
		return nullptr;
	}
	
	AddAsset<CTexture>(_strKey, pTex.Get());
	return pTex;
}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _strKey, ComPtr<ID3D11Texture2D> _tex2D)
{
	Ptr<CTexture> pTex = FindAsset<CTexture>(_strKey);
	assert(!pTex.Get());

	pTex = new CTexture(true);
	if (FAILED(pTex->Create(_tex2D)))
	{
		MessageBox(nullptr, L"Failed to Create Texture", L"Faile to Create Resource", MB_OK);
		return nullptr;
	}

	AddAsset<CTexture>(_strKey, pTex.Get());
	return pTex;
}

void CAssetMgr::GetAssetName(ASSET_TYPE _Type, vector<string>& _out)
{
	for (const auto& pair : m_hashAsset[(UINT)_Type])
		_out.push_back(WstrToStr(pair.first));
}

void CAssetMgr::AddAsset(const wstring& _strKey, CAsset* _Asset)
{
	ASSET_TYPE Type = _Asset->GetType();
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

void CAssetMgr::DeleteAsset(ASSET_TYPE _Type, const wstring& _strKey)
{
	unordered_map<wstring, Ptr<CAsset>>::iterator iter = m_hashAsset[(UINT)_Type].find(_strKey);
	assert(!(iter == m_hashAsset[(UINT)_Type].end()));

	m_hashAsset[(UINT)_Type].erase(iter);
}