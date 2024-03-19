#include "pch.h"
#include "CConstBuffer.h"
#include "CDevice.h"

CConstBuffer::CConstBuffer(CB_TYPE _Type)
	: m_Desc{}
	, m_ElementCount(0)
	, m_ElementSize(0)
	, m_Type(_Type)
{
}

CConstBuffer::~CConstBuffer()
{
}

int CConstBuffer::Create(UINT _ElementSize, UINT _ElementCount)
{
	m_ElementSize = _ElementSize;
	m_ElementCount = _ElementCount;

	m_Desc = {};
	m_Desc.ByteWidth = m_ElementSize * m_ElementCount;
	m_Desc.StructureByteStride = m_ElementSize;
	m_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CB에 데이터 쓰기 가능
	m_Desc.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, m_CB.GetAddressOf())))
	{
		MessageBox(nullptr, L"Failed to Create Constant Buffer", L"TestInit ERROR", MB_OK);
		return E_FAIL;
	}
	
	return S_OK;
}

void CConstBuffer::SetData(void* _Src, UINT _ElementCount)
{
	// 디폴트 값 세팅
	if (_ElementCount == 0)
		_ElementCount = m_ElementCount;
	
	// System Mem -> GPU Mem, 결정된 최종좌표 이동정보 보냄
	// - Map: GPU의 인터페이스 주소와 연결해둠
	// - Unmap: Map시켜둔 내용이 GPU에 복사됨
	D3D11_MAPPED_SUBRESOURCE tSub = {};

	CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _Src, m_ElementSize * _ElementCount);
	CONTEXT->Unmap(m_CB.Get(), 0);
}

void CConstBuffer::UpdatePipeline()
{
	// 모든 shader에 등록
	// type을 idx로 register 자동 등록
	CONTEXT->VSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());	// Pram0: register 등록 (~2^12 byte)
	CONTEXT->HSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());	// Pram0: register 등록 (~2^12 byte)
	CONTEXT->DSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());	// Pram0: register 등록 (~2^12 byte)
	CONTEXT->GSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());	// Pram0: register 등록 (~2^12 byte)
	CONTEXT->PSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());	// Pram0: register 등록 (~2^12 byte)
}

void CConstBuffer::UpdatePipeline_CS()
{
	CONTEXT->CSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
}
