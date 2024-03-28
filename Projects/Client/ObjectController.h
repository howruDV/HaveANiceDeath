#pragma once
#include "UI.h"

class CGameObject;

class ObjectController 
{
private:
	vector<string>					m_vecLayerName;
	vector<CGameObject*>			m_vecObject;
	string							m_CurLayer;

	CGameObject*					m_ClickedObject;

public:
	void tick();
	void render();

	void FocusObject(CGameObject* _Object);
	void FindObject(vector<CGameObject*>& _OutObject, Vec2 _MouseWorldPos);

private:
	CGameObject* FindObject(Vec2 _MouseWorldPos);

public:
	void SetTargetObject(CGameObject* _Object) { m_ClickedObject = _Object; }

public:
    ObjectController();
    ~ObjectController();

};

