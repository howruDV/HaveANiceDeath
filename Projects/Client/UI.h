#pragma once
#include "imgui.h"
#include "CImGuiMgr.h"
// =======================================
// UI: ImGUI를 사용한 UI가 상속하는 Parent Class
// =======================================
// - 고유객체, 생애, 상속구조 등 관리

class UI
{
private:
	string			m_strName;	// 드러낼 이름 (중복가능)
	const string	m_strID;	// key (중복불가능)
	bool			m_bActive;
	bool			m_bModal;

	UI*				m_Parent;
	vector<UI*>		m_vecChild;

public:
	virtual void begin() {};
	virtual void tick();
	virtual void render();
	virtual void render_update() = 0;

public:
	void SetName(const string& _name) { m_strName = _name; }
	void AddChild(UI* _ChildUI) { m_vecChild.push_back(_ChildUI); _ChildUI->m_Parent = this; }
	virtual void Activate() { m_bActive = true; }
	virtual void Deactivate() { m_bActive = false; }
	void SetModal(bool _bModal) { m_bModal = _bModal; }
	const string& GetName() { return m_strName; }
	const string& GetID() { return m_strID; }
	UI* GetParentUI() { return m_Parent; }
	const vector<UI*>& GetChilds() { return m_vecChild; }
	bool IsActivate() { return m_bActive; }

public:
	UI(const string& _strName, const string& _strID);
	virtual ~UI();
};

