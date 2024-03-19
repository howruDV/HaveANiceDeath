#include "pch.h"
#include "CAnim.h"
#include "CTimeMgr.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CTexture.h"

CAnim::CAnim()
	: m_Animator(nullptr)
	, m_CurFrmIdx(0)
	, m_AccTime(0.f)
	, m_bFinish(false)
{
}

CAnim::CAnim(const CAnim& _OriginAnim)
	: m_Animator(nullptr)
	, m_vecFrm(_OriginAnim.m_vecFrm)
	, m_CurFrmIdx(_OriginAnim.m_CurFrmIdx)
	, m_bFinish(_OriginAnim.m_bFinish)
	, m_AtlasTex(_OriginAnim.m_AtlasTex)
	, m_AccTime(_OriginAnim.m_AccTime)
{
}

CAnim::~CAnim()
{
	if (!m_vecFrm.empty())
		m_vecFrm.clear();
}

void CAnim::finaltick()
{
	m_AccTime += DT;
	if (m_AccTime > m_vecFrm[m_CurFrmIdx].fDuration)
	{
		++m_CurFrmIdx;
		if (m_CurFrmIdx >= m_vecFrm.size())
		{
			m_CurFrmIdx = 0;
			m_bFinish = true;
		}
		m_AccTime = 0.f;
	}
}

void CAnim::UpdatePipeline()
{
	// constant buffer: 현재 프레임 정보
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIM2D_DATA);
	FAnimData2D data = {};
	data.UseAnim2D = true;
	data.vLeftTop = m_vecFrm[m_CurFrmIdx].vLeftTopUV;
	data.vOffset = m_vecFrm[m_CurFrmIdx].vOffsetUV;
	data.vCutSize = m_vecFrm[m_CurFrmIdx].vCutSizeUV;
	data.vBackgroundSize = m_vecFrm[m_CurFrmIdx].vBackgroundSize;

	pCB->SetData(&data);
	pCB->UpdatePipeline();

	// Shader Resource View 등록 (register 10)
	m_AtlasTex->UpdatePipeline(10);
}

void CAnim::Clear()
{
	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIM2D_DATA);
	FAnimData2D data = {};
	data.UseAnim2D = false;

	pCB->SetData(&data);
	pCB->UpdatePipeline();
}

void CAnim::Create(CAnimator2D* _Animator, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vOffset, Vec2 _vCutSize, Vec2 _vBackground, int _FrmCount, float _FPS)
{
	m_Animator = _Animator;
	m_AtlasTex = _AtlasTex;

	for (int i = 0; i < _FrmCount; ++i)
	{
		// UV값으로 세팅
		tAnimFrm frm = {};
		frm.vCutSizeUV = Vec2(_vCutSize.x / (float)_AtlasTex->GetWidth(), _vCutSize.y / (float)_AtlasTex->GetHeight());
		frm.vLeftTopUV = Vec2(_vLeftTop.x / (float)_AtlasTex->GetWidth() + frm.vCutSizeUV.x * i, _vLeftTop.y / (float)_AtlasTex->GetHeight());
		frm.vOffsetUV = Vec2(_vOffset.x / (float)_AtlasTex->GetWidth(), _vOffset.y / (float)_AtlasTex->GetHeight());
		frm.fDuration = 1.f / _FPS;
		frm.vBackgroundSize = Vec2(_vBackground.x / (float)_AtlasTex->GetWidth(), _vBackground.y / (float)_AtlasTex->GetHeight());

		m_vecFrm.push_back(frm);
	}
}

void CAnim::SaveToFile(FILE* _File)
{
	// 애니메이션 이름 저장
	SaveWString(GetName(), _File);

	// 모든 프레임 정보 저장
	size_t FrmSize = m_vecFrm.size();
	fwrite(&FrmSize, sizeof(size_t), 1, _File);
	fwrite(m_vecFrm.data(), sizeof(tAnimFrm), m_vecFrm.size(), _File);

	// 애니메이션이 참조하던 텍스쳐 정보 저장
	SaveAssetRef(m_AtlasTex, _File);
}

void CAnim::LoadFromFile(FILE* _File)
{
	// 애니메이션 이름 로드
	wstring strName;
	LoadWString(strName, _File);
	SetName(strName);

	// 모든 프레임 정보 로드
	size_t FrmSize = 0;
	fread(&FrmSize, sizeof(size_t), 1, _File);
	m_vecFrm.resize(FrmSize);
	fread(m_vecFrm.data(), sizeof(tAnimFrm), m_vecFrm.size(), _File);

	// 애니메이션이 참조하던 텍스쳐 정보 로드
	LoadAssetRef(m_AtlasTex, _File);
}