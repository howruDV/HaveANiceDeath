#include "pch.h"
#include "CMeshRender.h"
#include "CMesh.h"
#include "CMaterial.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CAnimator2D.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::finaltick()
{

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
