#include "pch.h"
#include "CCamera.h"
#include "CTransform.h"
#include "CDevice.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CRenderMgr.h"
#include "CRenderComponent.h"

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_ProjType(PROJ_TYPE::PERSPECTIVE)
	, m_FOV(XM_PI / 2.f)	// 90��
	, m_Width(0.f)
	, m_Scale(1.f)
	, m_AspectRatio(1.f)
	, m_Far(10000.f)
	, m_LayerCheck(0)
	, m_CameraPriority(-1)
{
	Vec2 vRes = CDevice::GetInst()->GetRenderResolution();
	m_AspectRatio = vRes.x / vRes.y;
}

CCamera::~CCamera()
{
}

void CCamera::render(vector<CGameObject*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
		_vec[i]->render();
	_vec.clear();	// �� ƽ���� domain �з��� �ְ� clear �ݺ�
}

void CCamera::render_postprocess()
{
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		// ���� render image ���� �� register binding
		CRenderMgr::GetInst()->CopyRenderTargetToPostProcessTarget();
		Ptr<CTexture> pPostProcessTex = CRenderMgr::GetInst()->GetPostProcessTex();
		pPostProcessTex->UpdatePipeline(13);
		
		m_vecPostProcess[i]->render();
	}
	m_vecPostProcess.clear(); // �� ƽ���� ���� ���, clear
}

void CCamera::begin()
{
	// ī�޶� �켱�������� �°� RenderMgr �� ��Ͻ�Ŵ
	CRenderMgr::GetInst()->RegisterCamera(this, m_CameraPriority);
}

void CCamera::finaltick()
{
	// camera finaltick: view matrix�� projection matrix ���

	// view matrix
	Vec3 vCamPos = Transform()->GetRelativePos();
	Matrix matTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);
	
	// cam dir�� coord�� ��ġ��Ű�� ����, cam dir�� ����� ����
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vUp	= Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	Matrix matRot = XMMatrixIdentity(); 
	matRot._11 = vRight.x; matRot._12 = vUp.x; matRot._13 = vFront.x;
	matRot._21 = vRight.y; matRot._22 = vUp.y; matRot._23 = vFront.y;
	matRot._31 = vRight.z; matRot._32 = vUp.z; matRot._33 = vFront.z;

	// translation -> rotation = view matrix
	m_matView = matTrans * matRot;

	// projection matrix
	m_matProj = XMMatrixIdentity();

	if (m_ProjType == PROJ_TYPE::ORTHOGRAPHIC)
	{
		Vec2 vRes = CDevice::GetInst()->GetRenderResolution();
		m_matProj = XMMatrixOrthographicLH(vRes.x * m_Scale, (vRes.x / m_AspectRatio) * m_Scale, 1.f, m_Far); // LH: LeftHand Coord
	}
	else
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
}

void CCamera::render()
{
	// ī�޶��� matrix �������� ����
	g_Transform.matView = m_matView;
	g_Transform.matProj = m_matProj;

	// domain ������� ������
	render(m_vecOpaque);
	render(m_vecMasked);
	render(m_vecTransparent);

	render_postprocess();
}

void CCamera::SortObject()
{
	// ī�޶� �� �� �ִ� layer�� objets�� ���, domain���� sorting��
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (int i = 0; i < LAYER_MAX; ++i)
	{
		// 1. layer check
		if (false == (m_LayerCheck & (1 << i)))
			continue;

		// 2. sort by domain
		CLayer* pLayer = pCurLevel->GetLayer(i);
		const vector<CGameObject*>& vecObjects = pLayer->GetLayerObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (!(vecObjects[j]->GetRenderComponent()
				&& vecObjects[j]->GetRenderComponent()->GetMesh().Get()
				&& vecObjects[j]->GetRenderComponent()->GetMaterial().Get()
				&& vecObjects[j]->GetRenderComponent()->GetMaterial()->GetShader().Get()))
				continue;

			SHADER_DOMAIN domain = vecObjects[j]->GetRenderComponent()->GetMaterial()->GetShader()->GetDomain();
			switch (domain)
			{
			case SHADER_DOMAIN::DOMAIN_OPAQUE:
				m_vecOpaque.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_MASKED:
				m_vecMasked.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
				m_vecTransparent.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
				m_vecPostProcess.push_back(vecObjects[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_DEBUG:
				break;
			}
		}
	}
}

void CCamera::SetCameraPriority(int _Priority)
{
	m_CameraPriority = _Priority;
	//CRenderMgr::GetInst()->RegisterCamera(this, _Priority);
}

void CCamera::LayerCheck(UINT _LayerIdx, bool _bCheck)
{
	if (_bCheck)
		m_LayerCheck |= (1 << _LayerIdx);
	else
		m_LayerCheck &= ~(1 << _LayerIdx);
}

void CCamera::LayerCheck(const wstring& _strLayerName, bool _bCheck)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pCurLevel->GetLayer(_strLayerName);
	if (!pLayer)
		return;

	int idx = pLayer->GetLayerIdx();
	LayerCheck(idx, _bCheck);
}

// Usage: ���ڷ� ������ ���� ī�޶� ���� Diff ����
Vec2 CCamera::GetWorldPosInWindow(Vec2 _Diff)
{
	Vec3 Center = Transform()->GetWorldPos();

	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();
	Vec2 vWinSize = Vec2(vResol.x * m_Scale, (vResol.x / m_AspectRatio) * m_Scale);

	Vec2 MouseWorldPos = Vec2(Center.x + _Diff.x * m_Scale, Center.y + _Diff.y * m_Scale);

	return MouseWorldPos;
}

void CCamera::SaveToFile(FILE* _File)
{
	fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fwrite(&m_FOV, sizeof(float), 1, _File);
	fwrite(&m_Width, sizeof(float), 1, _File);
	fwrite(&m_Scale, sizeof(float), 1, _File);
	//fwrite(&m_AspectRatio, sizeof(float), 1, _File);
	fwrite(&m_Far, sizeof(float), 1, _File);
	fwrite(&m_LayerCheck, sizeof(UINT), 1, _File);
	fwrite(&m_CameraPriority, sizeof(int), 1, _File);
}

void CCamera::LoadFromFile(FILE* _File)
{
	fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fread(&m_FOV, sizeof(float), 1, _File);
	fread(&m_Width, sizeof(float), 1, _File);
	fread(&m_Scale, sizeof(float), 1, _File);
	//fread(&m_AspectRatio, sizeof(float), 1, _File);
	fread(&m_Far, sizeof(float), 1, _File);
	fread(&m_LayerCheck, sizeof(UINT), 1, _File);
	fread(&m_CameraPriority, sizeof(int), 1, _File);
}

bool CCamera::IsLayerCheck(const wstring& _strLayerName)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pCurLevel->GetLayer(_strLayerName);

	if (nullptr == pLayer)
	{
		MessageBoxA(nullptr, "���̾ �����ϴ�.", "IsLayerCheck Failed!", MB_OK);
		return false;
	}

	int idx = pLayer->GetLayerIdx();

	return IsLayerCheck(idx);
}