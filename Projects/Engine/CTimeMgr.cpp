#include "pch.h"
#include "CTimeMgr.h"
#include "CFontMgr.h"
#include "CEngine.h"

CTimeMgr::CTimeMgr()
	: m_Frequency{}
	, m_PrevCount{}
	, m_CurCount{}
	, m_fTime(0.f)
	, m_bPause(false)
	, m_szText{}
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::init()
{
	// 초당 빈도
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_PrevCount);
}

void CTimeMgr::tick()
{
	// Update DT
	QueryPerformanceCounter(&m_CurCount);
	m_EngineDeltaTime = m_DeltaTime = double(m_CurCount.QuadPart - m_PrevCount.QuadPart) / double(m_Frequency.QuadPart);
	if (m_bPause)
		m_DeltaTime = 0.f;

	m_PrevCount = m_CurCount;

	// DT 보정
	if ((1.f / 60.f) < m_DeltaTime)
		m_DeltaTime = (1.f / 60.f);

	// 1 tick
	m_fTime += (float)m_EngineDeltaTime;
	m_szText[50] = {};
	if (1.f <= m_fTime)
	{		
		swprintf_s(m_szText, 50, L"DeltaTime : %f, FPS : %d", m_DeltaTime, m_iCall);
		m_iCall = 0;
		m_fTime = 0.f;
	}

	++m_iCall;

	g_Global.g_dt = (float)m_DeltaTime;
	g_Global.g_time += (float)m_DeltaTime;
}

void CTimeMgr::render()
{
	// 폰트 출력
	CFontMgr::GetInst()->DrawFont(m_szText, 10.f, 30.f, 16, FONT_RGBA(255, 30, 30, 255));
}