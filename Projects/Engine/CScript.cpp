#include "pch.h"
#include "CScript.h"
#include "CGameObject.h"
#include "CTransform.h"

CScript::CScript(UINT _ScriptType)
	: CComponent(COMPONENT_TYPE::SCRIPT)
	, m_iScriptType(_ScriptType)
{
}

CScript::CScript(const CScript& _Origin)
	: CComponent(_Origin)
	, m_iScriptType(_Origin.m_iScriptType)
{
}

CScript::~CScript()
{
}

void CScript::Instantiate(Ptr<CPrefab> _Prefab, Vec3 _vWorldPos, int _layerIdx)
{
	if (not _Prefab.Get())
		return;

	CGameObject* pNewObj = _Prefab->Instantiate();
	pNewObj->Transform()->SetRelativePos(_vWorldPos);
	GamePlayStatic::SpawnGameObject(pNewObj, _layerIdx);
}