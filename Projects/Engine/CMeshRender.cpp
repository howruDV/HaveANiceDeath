#include "pch.h"
#include "CMeshRender.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CAnimator2D.h"
#include "CRenderMgr.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::finaltick()
{
	if (CRenderMgr::GetInst()->IsDebugPosition())
		GamePlayStatic::DrawDebugCross(Transform()->GetWorldPos(), 20.f, Vec3(0.1f, 1.f, 0.f), true);
}

void CMeshRender::render()
{
	if (GetMesh() == nullptr || GetMaterial() == nullptr)
		return;

	if (Animator2D())
		Animator2D()->UpdatePipeline();
	else
		Animator2D()->Clear();

	UpdatePipeline();
	GetMesh()->render();
}

void CMeshRender::UpdatePipeline()
{
	if (GetMaterial().Get())
		GetMaterial()->UpdatePipeline();

	Transform()->UpdatePipeline();
}
