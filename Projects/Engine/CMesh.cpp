#include "pch.h"
#include "CMesh.h"
#include "CDevice.h"

CMesh::CMesh(bool _Engine)
	: CAsset(ASSET_TYPE::MESH, _Engine)
	, m_VtxCount(0)
	, m_IdxCount(0)
	, m_VBDesc{}
	, m_IBDesc{}
	, m_VtxSysMem(nullptr)
	, m_IdxSysMem(nullptr)
{
}

CMesh::~CMesh()
{
	if (m_VtxSysMem)
		delete m_VtxSysMem;
	if (m_IdxSysMem)
		delete m_IdxSysMem;
}

void CMesh::UpdatePipeline()
{
	// IA - VB, IB Update
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;

	CONTEXT->IASetVertexBuffers(0, 1, m_VB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT, 0);
}

int CMesh::Create(void* _Vtx, UINT _VtxCount, void* _Idx, UINT _IdxCount)
{
	m_VtxCount = _VtxCount;
	m_IdxCount = _IdxCount;

	// --------------------------
	// (1) Vertex Buffer ����
	// --------------------------
	// Buffer ����ü
	// - buffer ũ��: DRX11 header����  ```#pragm pack(1)``` ���ԵǾ��ֱ� ������, struct�� ũ�Ⱑ ���� ū ũ�� ������ �ƴ�, �ۼ��� ũ�� �״����
	m_VBDesc = {};
	m_VBDesc.ByteWidth = sizeof(Vtx) * _VtxCount;				// buffer ��ü ũ��
	m_VBDesc.StructureByteStride = sizeof(Vtx);			// vertex �ϳ� ũ��
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// �뵵
	m_VBDesc.CPUAccessFlags = 0;						// Cpu Access: VB�� ������ ���� �Ұ���
	m_VBDesc.Usage = D3D11_USAGE_DEFAULT;

	// SubResource:  g_Vtx �迭 �Ҵ�
	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = _Vtx;

	// Vertex Buffer ����
	if (FAILED(DEVICE->CreateBuffer(&m_VBDesc, &tSubData, m_VB.GetAddressOf())))
	{
		MessageBox(nullptr, L"Failed to Create Vertex Buffer", L"Mesh::Create ERROR", MB_OK);
		return E_FAIL;
	}

	// Index Buffer
	m_IBDesc = {};
	m_IBDesc.ByteWidth = sizeof(UINT) * _IdxCount;
	m_IBDesc.StructureByteStride = sizeof(UINT);
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.Usage = D3D11_USAGE_DEFAULT;

	tSubData = {};
	tSubData.pSysMem = _Idx;

	if (FAILED(DEVICE->CreateBuffer(&m_IBDesc, &tSubData, m_IB.GetAddressOf())))
	{
		MessageBox(nullptr, L"Failed to Create Index Buffer", L"Mesh::Create ERROR", MB_OK);
		return E_FAIL;
	}

	// ���� �������� �����Ҵ��� ������ ����, ����
	// - ���� GPU�� ������ ����ų� �����Ǵ� ��찡 �־
	m_VtxSysMem = new Vtx[m_VtxCount];
	m_IdxSysMem = new UINT[m_IdxCount];

	memcpy(m_VtxSysMem, _Vtx, sizeof(Vtx) * m_VtxCount);
	memcpy(m_IdxSysMem, _Idx, sizeof(UINT) * m_IdxCount);

	return 0;
}

void CMesh::render()
{
	UpdatePipeline();
	CONTEXT->DrawIndexed(m_IdxCount, 0, 0);
}

void CMesh::renderInstanced(UINT _Count)
{
	UpdatePipeline();
	CONTEXT->DrawIndexedInstanced(m_IdxCount, _Count, 0, 0, 0);
}
