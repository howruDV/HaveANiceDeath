#pragma once

#include "Ptr.h"
class CGameObject;
class CAsset;

namespace GamePlayStatic
{
	// object create/delete
	void SpawnGameObject(CGameObject* _Target, int _LayerIdx);
	void DestroyGameObject(CGameObject* _Target);

	// register asset
	void AddAsset(CAsset* _Asset);

	// debug draw
	void DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
	void DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _WorldRot, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

	void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
	void DrawDebugCross(Vec3 _vWorldPos, float _fScale, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
	
	//void DrawDebugCube();
	//void DrawDebugSphere();

	// sound
	void Play2DSound(const wstring& _SoundPath, int _Loop, float _Volume, bool _Overlap = true);
	void Stop2DSound(const wstring& _SoundPath);
	void Play2DBGM(const wstring& _SoundPath, float _Volume);
	void Stop2DBGM(const wstring& _SoundPath);
}

string WstrToStr(const wstring& _wstr);
wstring StrToWstr(const string& _str);
void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _FILE);

int Random(int min, int max);

template<typename T, UINT SIZE>
void Delete_Array(T* (&Arr)[SIZE])
{
	for (int i = 0; i < SIZE; ++i)
	{
		if (Arr[i])
		{
			delete Arr[i];
			Arr[i] = nullptr;
		}
	}
}

template <typename T>
void Delete_Vec(vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (_vec[i])
		{
			delete _vec[i];
			_vec[i] = nullptr;
		}
	}
	_vec.clear();
}

template <typename T1, typename T2>
void Delete_Map(unordered_map<T1, T2>& _map)
{
	for (const auto& pair : _map)
		delete pair.second;

	_map.clear();
}

#include "CAssetMgr.h"
template<typename T>
class Ptr;
template<typename T>
void SaveAssetRef(Ptr<T> _Asset, FILE* _File)
{
	bool bAssetExist = false;
	_Asset.Get() == nullptr ? bAssetExist = false : bAssetExist = true;

	fwrite(&bAssetExist, sizeof(bool), 1, _File);

	if (bAssetExist)
	{
		SaveWString(_Asset->GetKey(), _File);
		SaveWString(_Asset->GetRelativePath(), _File);
	}
}

template<typename T>
void LoadAssetRef(Ptr<T>& _Asset, FILE* _File)
{
	bool bAssetExist = false;
	fread(&bAssetExist, sizeof(bool), 1, _File);

	if (bAssetExist)
	{
		wstring strKey, strRelativePath;
		LoadWString(strKey, _File);
		LoadWString(strRelativePath, _File);

		_Asset = CAssetMgr::GetInst()->Load<T>(strKey, strRelativePath);
	}
}