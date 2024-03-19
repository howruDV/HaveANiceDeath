#include "pch.h"
#include "CEditorObjMgr.h"
#include "CEditorObj.h"

#include <Engine/components.h>
#include <Engine/CRenderMgr.h>
#include "CCameraMoveScript.h"

CEditorObjMgr::CEditorObjMgr()
{

}

CEditorObjMgr::~CEditorObjMgr()
{
	Delete_Vec(m_vecEditorObj);
}

void CEditorObjMgr::init()
{
	// create editor camera
	CEditorObj* pEditorCam = new CEditorObj;
	pEditorCam->AddComponent(new CTransform);
	pEditorCam->AddComponent(new CCamera);
	pEditorCam->AddComponent(new CCameraMoveScript);

	pEditorCam->Camera()->LayerCheckAll();
	pEditorCam->Camera()->LayerCheck(31, false);
	pEditorCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	pEditorCam->Camera()->SetFOV(XM_PI / 2.f);
	pEditorCam->Camera()->SetFar(100000.f);

	m_vecEditorObj.push_back(pEditorCam);

	// register with renderMgr (Editor camera)
	CRenderMgr::GetInst()->RegisterEditorCamera(pEditorCam->Camera());
}

void CEditorObjMgr::progress()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
		m_vecEditorObj[i]->tick();

	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
		m_vecEditorObj[i]->finaltick();
}
