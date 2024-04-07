#pragma once
#include "CEntity.h"

// =======================================
// CLevel: GameObject가 배치되는 Scene
// =======================================
class CLevel;
typedef void(*SAVE_LEVEL)(CLevel*, const wstring&);
typedef CLevel* (*LOAD_LEVEL)(const wstring&);

class CLayer;
class CGameObject;

class CLevel :
    public CEntity
{
public:
    static SAVE_LEVEL LEVEL_SAVE;
    static LOAD_LEVEL LEVEL_LOAD;

private:
    CLayer*         m_arrLayer[LAYER_MAX];
    LEVEL_STATE		m_State;

private:
    void clear();

public:
    void begin();
    void tick();
    void finaltick();

public:
    void AddObject(CGameObject* _Object, int _LayerIdx, bool _bChildMove = true);
    void AddObject(CGameObject* _Object, const wstring& _LayerName, bool _bChildMove = true);
    void ChangeState(LEVEL_STATE _NextState);

    int GetLayerIdxByName(const wstring& _strLayerName);
    wstring GetLayerNameByIdx(int _LayerIdx);
    CLayer* GetLayer(int _iLayerIdx) { return m_arrLayer[_iLayerIdx]; }
    CLayer* GetLayer(const wstring& _strLayerName);
    CGameObject* FindObjectByName(const wstring& _strName);
    CGameObject* FindObjectByName(const wstring& _strName, int _LayerIdx);
    void FindObjectsByName(const wstring& _strName, vector<CGameObject*>& _out);
    LEVEL_STATE GetState() { return m_State; }

public:
    CLONE(CLevel);
    CLevel();
    CLevel(const CLevel& _OriginLevel);
    ~CLevel();

    friend class CLevelMgr;
};

