#pragma once
#include "CComponent.h"
#include "CPrefab.h"

// =======================================
// CScript: 모든 scprit의  parent class
// =======================================
// - 세세한 게임 구현부는 Scripts class를 상속한 scripts를 작성해 구현

struct FScriptParam
{
    SCRIPT_PARAM    Type;
    string          Desc;
    void*           pData;
};

class CScript :
    public CComponent
{
private:
    const UINT              m_iScriptType;
    vector<FScriptParam>    m_ScriptParam;

public:
    virtual void finaltick() final {}
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {};
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {};
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {};
    
    CScript* Clone() = 0;
    virtual void SaveToFile(FILE* _File) = 0;
    virtual void LoadFromFile(FILE* _File) = 0;

protected:
    void Instantiate(Ptr<CPrefab> _Prefab, Vec3 _vWorldPos, int _LayerIdx);
    void AddScriptParam(SCRIPT_PARAM _Param, const string& _Desc, void* _Data) { m_ScriptParam.push_back(FScriptParam{ _Param , _Desc, _Data }); }

public:
    void SetGlobalState(bool _play);

    UINT GetScriptType() { return m_iScriptType; }
    const vector<FScriptParam>& GetScriptParam() { return m_ScriptParam; }
    bool IsGlobalState();

public:
    CScript(UINT m_iScriptType);
    CScript(const CScript& _Origin);
    ~CScript();
};