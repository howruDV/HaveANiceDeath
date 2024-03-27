#include "pch.h"
#include "CEngine.h"
#include "CDevice.h"

#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CPathMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CTaskMgr.h"
#include "CGC.h"
#include "CRenderMgr.h"
#include "CCollisionMgr.h"
#include "CFontMgr.h"
#include "CLevel.h"
#include "CSound.h"

CEngine::CEngine()
	: m_hMainWnd(nullptr)
	, m_vResolution()
{
}

CEngine::~CEngine()
{
	if (CSound::g_pFMOD)
	{
		CSound::g_pFMOD->release();
		CSound::g_pFMOD = nullptr;
	}
}

void CEngine::DebugFunctionCheck()
{
	if (KEY_TAP(KEY::TAB))
		CRenderMgr::GetInst()->IsDebugPosition() ? CRenderMgr::GetInst()->SetDebugPosition(false) : CRenderMgr::GetInst()->SetDebugPosition(true);
}

int CEngine::init(HWND _hWnd, Vec2 _vResolution)
{
	m_hMainWnd = _hWnd;
	m_vResolution = _vResolution;

	// device init
	if (FAILED(CDevice::GetInst()->init(m_hMainWnd, m_vResolution)))
	{
		MessageBox(nullptr, L"Device �ʱ�ȭ ����", L"�ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	// Manager init
	CPathMgr::init();
	CTimeMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CAssetMgr::GetInst()->init();
	CLevelMgr::GetInst()->init();
	CRenderMgr::GetInst()->init();
	CFontMgr::GetInst()->init();

	//CLevelMgr::GetInst()->GetCurrentLevel()->begin();

	return S_OK;
}

void CEngine::progress()
{
	// Manager Update
	CTimeMgr::GetInst()->tick();
	CKeyMgr::GetInst()->tick();

	// FMOD Update
	CSound::g_pFMOD->update();

	// Level Update
	CLevelMgr::GetInst()->tick();
	//if (CLevelMgr::GetInst()->GetCurrentLevel())
	//{ 
	//	CCollisionMgr::GetInst()->tick();
	//	CRenderMgr::GetInst()->tick();
	//}

	// GC
	CGC::GetInst()->tick();

	// Task
	CTaskMgr::GetInst()->tick();

	DebugFunctionCheck();
}
