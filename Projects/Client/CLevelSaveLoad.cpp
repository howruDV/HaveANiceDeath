#include "pch.h"
#include "CLevelSaveLoad.h"

#include <Engine/CPathMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CScript.h>
#include <Engine/CMaterial.h>
#include <Scripts/CScriptMgr.h>

void CLevelSaveLoad::SaveLevel(CLevel* _Level, const wstring& _strLevelPath)
{
	assert(_Level);
	if (not(_Level->GetState() == LEVEL_STATE::STOP || _Level->GetState() == LEVEL_STATE::NONE))
	{
		MessageBox(nullptr, L"Failed to Save Level", L"Failed to Save Level", MB_OK);
		return;
	}

	// save all using materials
	const unordered_map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssetsByType(ASSET_TYPE::MATERIAL);
	for (const pair<wstring, Ptr<CAsset>>& pair : mapAsset)
	{
		// case: 엔진 에셋인 경우, 엔진 에셋은 파일로부터 로딩된 에셋이 아니라 프로그램 실행 도중 만들어진 임시 에셋
		if (pair.second->IsEngineAsset())
			continue;

		Ptr<CMaterial> saveMat = dynamic_cast<CMaterial*>(pair.second.Get());
		saveMat->Save(pair.second->GetKey());
	}

	// open save file
	FILE* pFile = nullptr;
	wstring strLevelPath = CPathMgr::GetContentPath();
	strLevelPath += _strLevelPath;
	_wfopen_s(&pFile, strLevelPath.c_str(), L"wb");

	// save
	SaveWString(_Level->GetName(), pFile);

	for (UINT i = 0; i < LAYER_MAX; ++i)
		SaveLayer(_Level->GetLayer(i), pFile);

	// close save file
	fclose(pFile);
}

void CLevelSaveLoad::SaveLayer(CLayer* _Layer, FILE* _File)
{
	// 1. save layer name
	SaveWString(_Layer->GetName(), _File);

	// 2. save layer objs
	const vector<CGameObject*>& vecObject = _Layer->GetParentObjects();
	size_t ObjCount = vecObject.size();
	fwrite(&ObjCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecObject.size(); ++i)
		SaveGameObject(vecObject[i], _File);
}

void CLevelSaveLoad::SaveGameObject(CGameObject* _Obj, FILE* _File)
{
	// 1. obj name
	SaveWString(_Obj->GetName(), _File);

	// 2. components
	UINT i = 0;
	for (; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* pCom = _Obj->GetComponent((COMPONENT_TYPE)i);
		if (nullptr == pCom)
			continue;

		// 2.1 component type
		fwrite(&i, sizeof(UINT), 1, _File);

		// 2.2 component data
		pCom->SaveToFile(_File);
	}
	fwrite(&i, sizeof(UINT), 1, _File);

	// 3. scripts
	const vector<CScript*>& vecScripts = _Obj->GetScripts();
	size_t ScriptCount = vecScripts.size();

	// 3.1 scripts count
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		// 3.2 script
		SaveWString(CScriptMgr::GetScriptName(vecScripts[i]), _File);
		vecScripts[i]->SaveToFile(_File);
	}

	// 4. child objs
	const vector<CGameObject*>& vecChild = _Obj->GetChild();
	size_t childcount = vecChild.size();
	fwrite(&childcount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < childcount; ++i)
	{
		SaveGameObject(vecChild[i], _File);
	}
}

CLevel* CLevelSaveLoad::LoadLevel(const wstring& _strLevelPath)
{
	CLevel* pLevel = nullptr;

	// open read file
	FILE* pFile = nullptr;
	wstring strLevelPath = CPathMgr::GetContentPath();
	strLevelPath += _strLevelPath;

	if (!exists(strLevelPath))
	{
		MessageBox(nullptr, L"Invalid level path or not exist", L"Failed to Load Level", MB_OK);
		return nullptr;
	}

	_wfopen_s(&pFile, strLevelPath.c_str(), L"rb");

	// read
	pLevel = new CLevel;
	wstring strLevelName;
	LoadWString(strLevelName, pFile);
	pLevel->SetName(strLevelName);

	for (UINT i = 0; i < LAYER_MAX; ++i)
	{
		LoadLayer(pLevel->GetLayer(i), pFile);
	}

	// close read file
	fclose(pFile);

	return pLevel;
}

void CLevelSaveLoad::LoadLayer(CLayer* _Layer, FILE* _File)
{
	// 1. load layer name
	wstring strLayerName;
	LoadWString(strLayerName, _File);
	_Layer->SetName(strLayerName);

	// 2. load game objs
	size_t ObjCount = 0;
	fread(&ObjCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ObjCount; ++i)
	{
		CGameObject* pObject = LoadGameObject(_File);
		_Layer->AddObject(pObject, false);
	}
}

CGameObject* CLevelSaveLoad::LoadGameObject(FILE* _File)
{
	CGameObject* pObject = new CGameObject;

	// 1. obj name
	wstring strName;
	LoadWString(strName, _File);
	pObject->SetName(strName);

	// 2. copmonents
	COMPONENT_TYPE type = COMPONENT_TYPE::END;
	while (true)
	{
		fread(&type, sizeof(UINT), 1, _File);
		if (type == COMPONENT_TYPE::END)
			break;

		CComponent* pComponent = nullptr;

		switch (type)
		{
		case COMPONENT_TYPE::TRANSFORM:
			pComponent = new CTransform;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			pComponent = new CCollider2D;
			break;
		case COMPONENT_TYPE::ANIMATOR2D:
			pComponent = new CAnimator2D;
			break;
		case COMPONENT_TYPE::LIGHT2D:
			pComponent = new CLight2D;
			break;
		case COMPONENT_TYPE::CAMERA:
			pComponent = new CCamera;
			break;
		case COMPONENT_TYPE::STATEMACHINE:
			pComponent = new CStateMachine;
			break;
		case COMPONENT_TYPE::MESHRENDER:
			pComponent = new CMeshRender;
			break;
		case COMPONENT_TYPE::TILEMAP:
			pComponent = new CTileMap;
			break;
		case COMPONENT_TYPE::PARTICLESYSTEM:
			pComponent = new CParticleSystem;
			break;
		case COMPONENT_TYPE::MOVEMENT:
			pComponent = new CParticleSystem;
			break;
		default:
			assert(nullptr);
			break;
		}

		// 2.2 component data
		pObject->AddComponent(pComponent);
		pComponent->LoadFromFile(_File);
	}

	// 3. scripts
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		wstring strScriptName;
		LoadWString(strScriptName, _File);

		CScript* pScript = CScriptMgr::GetScript(strScriptName);
		pObject->AddComponent(pScript);
		pScript->LoadFromFile(_File);
	}

	// 4. child objs	
	size_t childcount = 0;
	fread(&childcount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < childcount; ++i)
	{
		pObject->AddChild(LoadGameObject(_File));
	}

	return pObject;
}
