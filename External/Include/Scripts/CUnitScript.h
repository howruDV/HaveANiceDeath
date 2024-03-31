#pragma once
#include <Engine/CScript.h>

class CUnitScript :
    public CScript
{
protected:
    // Info
    float       m_fSpeed;
    UNIT_DIRX   m_Dir;
    UNIT_DIRX   m_Dir_Prev;
    bool        m_bDirLock;
    bool        m_bDirChange;

    // hp
    int         m_iHPMax;
    int         m_iHPCur;

public:
    virtual void begin() override {};
    virtual void tick() override;

    virtual void SaveToFile(FILE* _File) {}
    virtual void LoadFromFile(FILE* _File) {}

    // Hit & Attack
    void GetDamage(int _iHP) { m_iHPCur = -_iHP; }
    //void AddDamage(float iDamage);

public:
    void SetSpeed(float _fSpeed) { m_fSpeed = m_fSpeed; }
    void SetDir(UNIT_DIRX _dir) { if(!m_bDirLock) m_Dir = _dir; }
    void SetHPMax(int _iHP) { m_iHPMax = _iHP; }
    void SetHPCur(int _iHP) { m_iHPCur = _iHP; }
    void SetDirLock(bool _bLock) { m_bDirLock = _bLock; }

    float GetSpeed() { return m_fSpeed; }
    UNIT_DIRX GetDir() { return m_Dir; }
    UNIT_DIRX GetDirPrev() { return m_Dir_Prev; }
    int GetHPMax() { return m_iHPMax; }
    int GetHPCur() { return m_iHPCur; }
    bool IsDead() { return m_iHPCur <= 0; }
    bool IsDirChange() { return m_bDirChange; }
    bool IsDirLock() { return m_bDirLock; }

public:
    CLONE(CUnitScript);
    CUnitScript(UINT m_iScriptType);
    ~CUnitScript();
};