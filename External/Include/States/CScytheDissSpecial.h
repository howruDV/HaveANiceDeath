#pragma once
#include <Engine/CState.h>
#include <Engine/CSound.h>

class CScytheDissSpecial :
    public CState
{
private:
    Vec3 m_OrginScale;
    CGameObject* m_pHitbox;
    vector<Ptr<CSound>> m_vecSound;
    int m_SoundIdx;

public:
    virtual void finaltick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CScytheDissSpecial();
    ~CScytheDissSpecial();
};