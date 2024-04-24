#include "pch.h"
#include "CRenderMgr.h"
#include "CDevice.h"
#include "CCamera.h"
#include "CMeshRender.h"
#include "CAssetMgr.h"
#include "CTransform.h"
#include "CTimeMgr.h"
#include "CStructuredBuffer.h"
#include "CLight2D.h"
#include "CConstBuffer.h"

#define DEFAULT_WINSIZE Vec2(1600.f, 900.f)

CRenderMgr::CRenderMgr()
	: m_vecCam{}
	, m_DbgShapeInfo {}
	, m_pDbgObj(nullptr)
	, m_Light2DBuffer(nullptr)
	, m_DebugPosition(true)
	, m_EditorCam(nullptr)
	, m_RenderFunc(nullptr)
	, m_BloomLevel(3)
{
	m_RenderFunc = &CRenderMgr::render_play;

#ifdef _DEBUG
	m_isEditorMode = true;
	m_WinResol = DEFAULT_WINSIZE;
#else
	m_isEditorMode = false;
	m_WinResol = CDevice::GetInst()->GetRenderResolution();
#endif

	m_WinScale = m_WinResol / DEFAULT_WINSIZE;
}

CRenderMgr::~CRenderMgr()
{
	if (m_pDbgObj)
		delete m_pDbgObj;

	if (m_Light2DBuffer)
		delete m_Light2DBuffer;
}

void CRenderMgr::tick()
{
	// Output Merge State(OM)에 Render Target Texture와 Depth Stencil Texture 전달
	// ImGUI 붙이면서 renderTarget이 중간에 바뀔 수 있으므로 renderMgr로 옮겨줌
	/*Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());*/
	CDevice::GetInst()->SetRenderTarget();

	// buffer init
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->ClearRenderTarget(ClearColor);

	// update DX resources
	UpdatePipeline();

	// rendering: 등록된 camera에 맞춰 다른 렌더 돌려야 함
	(this->*m_RenderFunc)();
	render_debug();

	// clear used resources
	Clear();

	// drawing (buffer show)
	// - UI창도 함께 그릴 것이므로 Client에서 ImGUI 그린 후 Present해줌
	// CDevice::GetInst()->Present(); 
}

void CRenderMgr::render_play()
{
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->SortObject();
		m_vecCam[i]->render();
	}
}

void CRenderMgr::render_editor()
{
	if (not m_EditorCam)
		return;

	m_EditorCam->SortObject();
	m_EditorCam->render();
}

void CRenderMgr::render_debug()
{
	if (!m_DebugPosition || m_vecCam.empty() && !m_EditorCam)
		return;

	// TODO : 일단 0번 카메라로 지정한 듯? 나중에 고쳐야 함
	if (m_isEditorMode)
	{
		g_Transform.matView = m_EditorCam->GetViewMat();
		g_Transform.matProj = m_EditorCam->GetProjMat();
	}
	else
	{
		g_Transform.matView = m_vecCam[0]->GetViewMat();
		g_Transform.matProj = m_vecCam[0]->GetProjMat();
	}

	list<FDebugShapeInfo>::iterator iter = m_DbgShapeInfo.begin();
	while (iter!=m_DbgShapeInfo.end())
	{
		// 보유한 Debug obj마다 type 별로 transform, mesh 바꿔가며 render해줌
		switch ((*iter).ShapeType)
		{
		case DEBUG_SHAPE::RECT:
			m_pDbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_pDbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
			break;
		case DEBUG_SHAPE::CROSS:
			m_pDbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CrossMesh_Debug"));
			break;
		case DEBUG_SHAPE::CUBE:
			m_pDbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh_Debug"));
			break;
		case DEBUG_SHAPE::SPHERE:
			m_pDbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh_Debug"));
			break;
		}

		m_pDbgObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, (*iter).vColor); // 생성자로 vec3 -> vec4 자동 확장
		D3D11_PRIMITIVE_TOPOLOGY PrevTopology = m_pDbgObj->MeshRender()->GetMaterial()->GetShader()->GetTopology();
		if ((*iter).ShapeType == DEBUG_SHAPE::CROSS)
			m_pDbgObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		m_pDbgObj->Transform()->SetWorldMat((*iter).matWorld);
		m_pDbgObj->Transform()->UpdatePipeline();
		m_pDbgObj->render();
		
		m_pDbgObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(PrevTopology);

		// time check
		(*iter).fLifeTime += DT_ENGINE;
		if ((*iter).fLifeTime >= (*iter).fDuration)
			iter = m_DbgShapeInfo.erase(iter);
		else
			++iter;
	}
}

void CRenderMgr::CreatePostprocessTexture(Vec2 _vResolution)
{
	m_PostProcessTex = CAssetMgr::GetInst()->CreateTexture(L"PostProcessTex", (UINT)_vResolution.x, (UINT)_vResolution.y, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
}

void CRenderMgr::CreateRTCopyTexture(Vec2 _vResolution)
{
	m_RTCopyTex = CAssetMgr::GetInst()->CreateTexture(L"RTCopyTex", (UINT)_vResolution.x, (UINT)_vResolution.y, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);
}

void CRenderMgr::CreateBloomTextures(Vec2 _vResolution)
{
	m_BloomRTTex = CAssetMgr::GetInst()->CreateTexture(L"BloomRTTex", (UINT)_vResolution.x, (UINT)_vResolution.y, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS);
	m_BloomRTCopyTex = CAssetMgr::GetInst()->CreateTexture(L"BloomRTCopyTex", (UINT)_vResolution.x, (UINT)_vResolution.y, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS, D3D11_USAGE_DEFAULT); // @Todo 터짐여기

	for (int i = 1; i < m_BloomLevel; i++)
	{
		wstring strTexKey = L"BlurXTex" + std::to_wstring(i);
		Vec2 vResol = _vResolution / pow(2, i);

		Ptr<CTexture> pBlurTex = CAssetMgr::GetInst()->CreateTexture(strTexKey, (UINT)vResol.x, (UINT)vResol.y, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS);
		m_vecBlurXTex.push_back(pBlurTex);
		
		strTexKey = L"BlurXYTex" + std::to_wstring(i);
		pBlurTex = CAssetMgr::GetInst()->CreateTexture(strTexKey, (UINT)vResol.x, (UINT)vResol.y, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS);
		m_vecBlurXYTex.push_back(pBlurTex);
	}
}

void CRenderMgr::CreateDebugObj()
{
	m_pDbgObj = new CGameObject;
	m_pDbgObj->AddComponent(new CTransform);
	m_pDbgObj->AddComponent(new CMeshRender);
	m_pDbgObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMat"));
}

void CRenderMgr::CreateBloomCS()
{
	m_CSBloomDownScaling	= (CDownSampling*)	CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"BloomDownScalingShader").Get();
	m_CSBloomUpScaling		= (CUpsampling*)	CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"BloomUpScalingShader").Get();
	m_CSBloomBluringX		= (CBlurX*)			CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"BloomBluringXShader").Get();
	m_CSBloomBluringY		= (CBlurY*)			CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"BloomBluringYShader").Get();
	m_CSBloomCombine		= (CCombine*)		CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"BloomCombineShader").Get();
}

void CRenderMgr::UpdatePipeline()
{
	static vector<FLightInfo> vecLight2DInfo;
	g_Global.g_Light2DCount = (int)m_vecLight2D.size();
	//g_Global.g_Light3DCount = (int)m_vecLight3D.size();

	// CB update: global data
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL_DATA);
	pCB->SetData(&g_Global);
	pCB->UpdatePipeline();
	pCB->UpdatePipeline_CS();

	// SB update: 2D Light
	if (!m_vecLight2D.empty())
	{
		for (size_t i = 0; i < m_vecLight2D.size(); ++i)
		{
			const FLightInfo& info = m_vecLight2D[i]->GetLightInfo();
			vecLight2DInfo.push_back(info);
		}
		m_Light2DBuffer->SetData(vecLight2DInfo.data(), (UINT)vecLight2DInfo.size());
	}
	m_Light2DBuffer->UpdatePipeline(11);
	vecLight2DInfo.clear();

	// SB update: 3D Light
}

void CRenderMgr::ActiveEditorMode(bool _bActive)
{
	if (_bActive)
	{
		m_RenderFunc = &CRenderMgr::render_editor;
		m_isEditorMode = true;
	}
	else
	{
		m_RenderFunc = &CRenderMgr::render_play;
		m_isEditorMode = false;
	}
}

void CRenderMgr::Clear()
{
	m_vecLight2D.clear();
}

void CRenderMgr::CopyRenderTargetToPostProcessTarget()
{
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_PostProcessTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void CRenderMgr::CopyRenderTargetToImGuiRenderTexture()
{
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void CRenderMgr::CopyRenderTargetToBloomTarget()
{
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_BloomRTCopyTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void CRenderMgr::Execute_Bloom()
{
	CDevice::GetInst()->ClearRenderTarget();

	// 1. DownScaling
	for (int i = 0; i < m_BloomLevel - 1; i++)
	{
		// case: first (full size -> half size)
		if (i == 0)
			m_CSBloomDownScaling->SetResourceTex(m_BloomRTTex);			// source
		else
			m_CSBloomDownScaling->SetResourceTex(m_vecBlurXTex[i-1]);	// source

		m_CSBloomDownScaling->SetTargetTexture(m_vecBlurXTex[i]);		// target
		m_CSBloomDownScaling->Execute();
	}

	// 2. Bluring (x, y) & UpScaling
	for (int i = m_BloomLevel - 2; i >= 0; i--)
	{
		//// Blur X
		//m_CSBloomBluringX->SetResourceTex(m_vecBlurXTex[i]);			// source
		//m_CSBloomBluringX->SetTargetTexture(m_vecBlurXYTex[i]);			// target
		//m_CSBloomBluringX->Execute();

		//// Blur Y
		//m_CSBloomBluringY->SetResourceTex(m_vecBlurXYTex[i]);			// source
		//m_CSBloomBluringY->SetTargetTexture(m_vecBlurXTex[i]);			// target
		//m_CSBloomBluringY->Execute();

		// UpScaling
		m_CSBloomUpScaling->SetResourceTex(m_vecBlurXTex[i]);			// source

		if (i == 0)
			m_CSBloomUpScaling->SetTargetTexture(m_BloomRTTex);			// target
		else
			m_CSBloomUpScaling->SetTargetTexture(m_vecBlurXTex[i-1]);	// target

		m_CSBloomUpScaling->Execute();
	}

	// 3. Combine
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	CopyRenderTargetToBloomTarget();
	m_CSBloomCombine->SetRenderTargetCopyTex(m_BloomRTCopyTex);			// combine1
	m_CSBloomCombine->SetBloomTex(m_BloomRTTex);						// combine2
	m_CSBloomCombine->SetRenderTargetTex(pRTTex);						// RT(output)
	m_CSBloomCombine->Execute();

	CDevice::GetInst()->SetRenderTarget();
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _idx)
{
	if (m_vecCam.size() <= _idx + 1)
		m_vecCam.resize(_idx + 1);

	assert(!m_vecCam[_idx]);
	m_vecCam[_idx] = _Cam;
}