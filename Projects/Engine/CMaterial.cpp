#include "pch.h"
#include "CMaterial.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CGraphicsShader.h"
#include "CTexture.h"
#include "CPathMgr.h"
#include "Ptr.h"

CMaterial::CMaterial(bool _bEngine)
	: CAsset(ASSET_TYPE::MATERIAL, _bEngine)
	, m_Const{}
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::UpdatePipeline()
{
	if (!m_pShader.Get())
		return;

	// binding Shader
	m_pShader->UpdatePipeline();

	// binding Texture
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		if (m_arrTex[i].Get())
		{
			m_arrTex[i]->UpdatePipeline(i);
			m_Const.bTex[i] = 1;
		}
		else
		{
			CTexture::ClearPipeline(i); // 남아있는 이전 obj의 pipeline data 초기화
			m_Const.bTex[i] = 0;
		}
	}

	// binding Material Const Param
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);	// static
	pCB->SetData(&m_Const);
	pCB->UpdatePipeline();
}

void* CMaterial::GetScalarParam(SCALAR_PARAM _ParamType)
{
	switch (_ParamType)
	{
	case SCALAR_PARAM::INT_0:
	case SCALAR_PARAM::INT_1:
	case SCALAR_PARAM::INT_2:
	case SCALAR_PARAM::INT_3:
	{
		int idx = (int)_ParamType - (int)SCALAR_PARAM::INT_0;
		return m_Const.iArr + idx;
	}
	break;
	case SCALAR_PARAM::FLOAT_0:
	case SCALAR_PARAM::FLOAT_1:
	case SCALAR_PARAM::FLOAT_2:
	case SCALAR_PARAM::FLOAT_3:
	{
		int idx = (int)_ParamType - (int)SCALAR_PARAM::FLOAT_0;
		return m_Const.fArr + idx;
	}
	break;
	case SCALAR_PARAM::VEC2_0:
	case SCALAR_PARAM::VEC2_1:
	case SCALAR_PARAM::VEC2_2:
	case SCALAR_PARAM::VEC2_3:
	{
		int idx = (int)_ParamType - (int)SCALAR_PARAM::VEC2_0;
		return m_Const.v2Arr + idx;
	}
	break;
	case SCALAR_PARAM::VEC4_0:
	case SCALAR_PARAM::VEC4_1:
	case SCALAR_PARAM::VEC4_2:
	case SCALAR_PARAM::VEC4_3:
	{
		int idx = (int)_ParamType - (int)SCALAR_PARAM::VEC4_0;
		return m_Const.v4Arr + idx;
	}
	break;
	case SCALAR_PARAM::MAT_0:
	case SCALAR_PARAM::MAT_1:
	case SCALAR_PARAM::MAT_2:
	case SCALAR_PARAM::MAT_3:
	{
		int idx = (int)_ParamType - (int)SCALAR_PARAM::MAT_0;
		return m_Const.matArr + idx;
	}
	break;
	}

	return nullptr;
}

int CMaterial::Save(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");
	if (not pFile)
		return E_FAIL;

	// 재질 상수값 저장
	fwrite(&m_Const, sizeof(FMatConst), 1, pFile);

	// 재질이 참조하는 텍스쳐 정보를 저장	
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		SaveAssetRef<CTexture>(m_arrTex[i], pFile);
	}

	// 재질이 참조하는 쉐이더 정보를 저장
	SaveAssetRef<CGraphicsShader>(m_pShader, pFile);

	return 0;
}

int CMaterial::Load(const wstring& _strFilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");
	if (not pFile)
		return E_FAIL;

	// 재질 상수값 저장
	fread(&m_Const, sizeof(FMatConst), 1, pFile);

	// 재질이 참조하는 텍스쳐 정보를 로드
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		LoadAssetRef<CTexture>(m_arrTex[i], pFile);
	}

	// 재질이 참조하는 쉐이더 정보를 저장
	LoadAssetRef<CGraphicsShader>(m_pShader, pFile);

	return 0;
}