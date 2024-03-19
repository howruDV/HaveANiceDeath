#include "pch.h"
#include "CLight2D.h"
#include "CRenderMgr.h"
#include "CTransform.h"

CLight2D::CLight2D()
	: CComponent(COMPONENT_TYPE::LIGHT2D)
{
}

CLight2D::~CLight2D()
{
}

void CLight2D::finaltick()
{
	// gpu는 위치의 변화값만 받아오므로,
	// light의 world position을 전달해 알 수 있게 함
	m_Info.vWorldPos = Transform()->GetWorldPos();
	m_Info.vWorldDir = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	// 광원 등록
	CRenderMgr::GetInst()->RegisterLight2D(this);
}

void CLight2D::SaveToFile(FILE* _File)
{
	fwrite(&m_Info, sizeof(FLightInfo), 1, _File);
}

void CLight2D::LoadFromFile(FILE* _File)
{
	fread(&m_Info, sizeof(FLightInfo), 1, _File);
}