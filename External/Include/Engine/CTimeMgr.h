#pragma once
#include "singleton.h"
class CTimeMgr :
    public CSingleton<CTimeMgr>
{
    SINGLE(CTimeMgr)

private:
    LARGE_INTEGER   m_Frequency;        // LARGE_INTGER: 8byte long long type 정수로 취급
    LARGE_INTEGER   m_PrevCount;
    LARGE_INTEGER   m_CurCount;
    double          m_DeltaTime;        // 현재 게임의 DT
    double			m_EngineDeltaTime;  // 프레임당 DT

    UINT            m_iCall;
    UINT            m_FPS;
    float           m_fTime;
    wchar_t			m_szText[256];
    bool			m_bPause;

public:
    void init();
    void tick();
    void render();

public:
    UINT GetFPS() { return m_FPS; }
    float GetDeltaTime() { return (float)m_DeltaTime; }
    float GetEngineDeltaTime() { return (float)m_EngineDeltaTime; }
    double GetEngineDeltaTime_d() { return m_EngineDeltaTime; }
    float GetGameTime() { return g_Global.g_time; }
    void PauseTime(bool _bPause) { m_bPause = _bPause; }
};

