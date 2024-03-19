#include "pch.h"
#include "RenderingTest.h"
#include "CDevice.h"
#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

int TestInit()
{
	// Vertex Buffer에 vertex 위치 설정
	
	// --------------------------
	// (0) Square
	// --------------------------
	// (0) Circle
	// --------------------------
	// (1) Vertex Buffer 생성
	// --------------------------
	// Constant Buffer

	// --------------------------
	// (2) Vertex Shader 생성
	// --------------------------
	// (3) Input Layout 생성
	// --------------------------
	// (4) Pixel Shader 생성
	// --------------------------

	// --------------------------
	// GameObject 생성
	// --------------------------

	return S_OK;
}

void TestProgress()
{
	Tick();
	Render();
}

void TestRelease()
{
}

void Tick()
{
	/*
	for (size_t i = 0; i < g_vecObj.size(); ++i)
	{
		g_vecObj[i]->tick();
		g_vecObj[i]->finaltick();
	}
	*/

	// object move
	// - 움직이기 위해 모델 좌표가 아니라 이동 정보를 수정

	// System Mem -> GPU Mem, 결정된 최종좌표 이동정보 보냄
	// - Map: GPU의 인터페이스 주소와 연결해둠
	
}

void Render()
{
	/*
	// buffer init
	float ClearColor[4] = { 0.3f, 0.3f, 0.3f, 1.f };
	CDevice::GetInst()->ClearRenderTarget(ClearColor);

	for (size_t i = 0; i < g_vecObj.size(); ++i)
	{
		g_vecObj[i]->render();
	}

	// shader setting
	// CB도 Update
	g_Shader->UpdateData();

	// mesh 선택, draw
	//g_RectMesh->render();
	g_CircleMesh->render();

	// rendering
	CDevice::GetInst()->Present();
	*/
}
