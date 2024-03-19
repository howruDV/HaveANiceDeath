#pragma once
#include "singleton.h"
// =======================================
// CLevelMgr: Level 积己, 包府
// =======================================
class CLevel;

class CLevelMgr :
    public CSingleton<CLevelMgr>
{
    SINGLE(CLevelMgr)

private:
    CLevel* m_CurLevel;

public:
    void init();
    void tick();

public:
    void ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextStartState);
    void ChangeLevelState(LEVEL_STATE _State);

private:
    void ChangeLevel_proc(CLevel* _NextLevel, LEVEL_STATE _NextLevelState);

public:
    CLevel* GetCurrentLevel() { return m_CurLevel; }

    friend class CTaskMgr;
};

