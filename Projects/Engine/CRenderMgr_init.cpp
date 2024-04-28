#include "pch.h"
#include "CRenderMgr.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CStructuredBuffer.h"
#include "CDevice.h"
#include "CAssetMgr.h"

void CRenderMgr::init()
{
	// create structured buffer
	m_Light2DBuffer = new CStructuredBuffer;
	m_Light2DBuffer->Create(sizeof(FLightInfo), 10, SB_TYPE::READ_ONLY, true);

	// create debug object
	CreateDebugObj();
	CreateBloomCS();

	// create postprocess texture
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();
	CreatePostprocessTexture(vRenderResolution);
	CreateRTCopyTexture(vRenderResolution);
	CreateBloomTextures(vRenderResolution);

	// nosie texture load
	m_vecNoiseTex.push_back(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\noise\\noise_01.png", L"texture\\noise\\noise_01.png"));
	m_vecNoiseTex.push_back(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\noise\\noise_02.png", L"texture\\noise\\noise_02.png"));
	m_vecNoiseTex.push_back(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\noise\\noise_03.jpg", L"texture\\noise\\noise_03.jpg"));
	m_vecNoiseTex.push_back(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\noise\\noise_04.jpg", L"texture\\noise\\noise_04.jpg"));
	m_vecNoiseTex.push_back(CAssetMgr::GetInst()->Load<CTexture>(L"texture\\noise\\noise_05.png", L"texture\\noise\\noise_05.png"));
	m_vecNoiseTex[4]->UpdatePipeline(14);
	m_vecNoiseTex[4]->UpdateCS_SRV(14);
}