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
	// Vertex Buffer�� vertex ��ġ ����
	
	// --------------------------
	// (0) Square
	// --------------------------
	// (0) Circle
	// --------------------------
	// (1) Vertex Buffer ����
	// --------------------------
	// Constant Buffer

	// --------------------------
	// (2) Vertex Shader ����
	// --------------------------
	// (3) Input Layout ����
	// --------------------------
	// (4) Pixel Shader ����
	// --------------------------

	// --------------------------
	// GameObject ����
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
	// - �����̱� ���� �� ��ǥ�� �ƴ϶� �̵� ������ ����

	// System Mem -> GPU Mem, ������ ������ǥ �̵����� ����
	// - Map: GPU�� �������̽� �ּҿ� �����ص�
	
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
	// CB�� Update
	g_Shader->UpdateData();

	// mesh ����, draw
	//g_RectMesh->render();
	g_CircleMesh->render();

	// rendering
	CDevice::GetInst()->Present();
	*/
}
