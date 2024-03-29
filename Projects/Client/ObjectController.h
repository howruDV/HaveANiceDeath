#pragma once
#include "UI.h"

class CGameObject;
class UIInspectorPannel;

class ObjectController 
{
private:
	UIInspectorPannel*				m_Inspector;
	vector<string>					m_vecLayerName;
	vector<CGameObject*>			m_vecObject;
	string							m_CurLayer;

public:
	void begin();
	void tick();
	void render();

	void FocusObject(CGameObject* _Object);
	void FindObject(vector<CGameObject*>& _OutObject, Vec2 _MouseWorldPos);

private:
	CGameObject* FindObject(Vec2 _MouseWorldPos);

public:
    ObjectController();
    ~ObjectController();
};

