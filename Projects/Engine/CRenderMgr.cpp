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

CRenderMgr::CRenderMgr()
	: m_vecCam{}
	, m_DbgShapeInfo {}
	, m_pDbgObj(nullptr)
	, m_Light2DBuffer(nullptr)
	, m_DebugPosition(true)
	, m_EditorCam(nullptr)
	, m_RenderFunc(nullptr)
{
	m_RenderFunc = &CRenderMgr::render_play;

#ifdef _DEBUG
	m_isEditorMode = true;
#else
	m_isEditorMode = false;
#endif

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
	// Output Merge State(OM)�� Render Target Texture�� Depth Stencil Texture ����
	// ImGUI ���̸鼭 renderTarget�� �߰��� �ٲ� �� �����Ƿ� renderMgr�� �Ű���
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	// buffer init
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->ClearRenderTarget(ClearColor);

	// update DX resources
	UpdatePipeline();

	// rendering: ��ϵ� camera�� ���� �ٸ� ���� ������ ��
	(this->*m_RenderFunc)();
	render_debug();

	// clear used resources
	Clear();

	// drawing (buffer show)
	// - UIâ�� �Բ� �׸� ���̹Ƿ� Client���� ImGUI �׸� �� Present����
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
	if (m_vecCam.empty() && !m_EditorCam)
		return;

	// TODO : �ϴ� 0�� ī�޶�� ������ ��? ���߿� ���ľ� ��
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
		// ������ Debug obj���� type ���� transform, mesh �ٲ㰡�� render����
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

		m_pDbgObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMat"));
		m_pDbgObj->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, (*iter).vColor); // �����ڷ� vec3 -> vec4 �ڵ� Ȯ��
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

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _idx)
{
	if (m_vecCam.size() <= _idx + 1)
		m_vecCam.resize(_idx + 1);

	assert(!m_vecCam[_idx]);
	m_vecCam[_idx] = _Cam;
}
