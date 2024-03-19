#pragma once

// =======================================
// CEntity: 모든 GameObjects가 상속하는 기본 class
// =======================================

class CEntity
{
private:
	static UINT g_NextID;

	const UINT	m_ID;
	wstring		m_strName;

public:
	virtual CEntity* Clone() = 0;

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() const { return m_strName; }
	UINT GetID() const { return m_ID; }

public:
	CEntity();
	CEntity(const CEntity& _Origin);
	virtual ~CEntity();
};

