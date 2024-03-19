#pragma once

class CEngine :
    public CSingleton<CEngine>
{
    SINGLE(CEngine);

private:
    HWND    m_hMainWnd;
    Vec2    m_vResolution;

private:
    void DebugFunctionCheck();

public:
    int init(HWND _hWnd, Vec2 _vResolution);
    void progress();

    HWND GetMainWind() { return m_hMainWnd; }
};