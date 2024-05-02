#include "pch.h"
#include "CAnim.h"
#include "CTimeMgr.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CTexture.h"
#include "CAnimator2D.h"

CAnim::CAnim()
	: m_Animator(nullptr)
	, m_CurFrmIdx(0)
	, m_AccTime(0.f)
	, m_bFinish(false)
	, m_bReverse(false)
{
}

CAnim::CAnim(const CAnim& _OriginAnim)
	: CEntity(_OriginAnim)
	, m_Animator(nullptr)
	, m_vecFrm(_OriginAnim.m_vecFrm)
	, m_CurFrmIdx(0)
	, m_bFinish(false)
	, m_AtlasTex(_OriginAnim.m_AtlasTex)
	, m_AccTime(0)
	, m_bReverse(false)
{
}

CAnim::~CAnim()
{
	if (!m_vecFrm.empty())
		m_vecFrm.clear();
}

void CAnim::finaltick()
{
	if (m_bFinish)
		return;

	m_AccTime += DT;

	if (m_AccTime > m_vecFrm[m_CurFrmIdx].fDuration)
	{
		if (!m_bReverse)
		{
			++m_CurFrmIdx;

			if (m_CurFrmIdx >= m_vecFrm.size())
			{
				m_CurFrmIdx = (int)m_vecFrm.size() - 1;
				m_bFinish = true;
			}
		}
		else
		{
			--m_CurFrmIdx;

			if (m_CurFrmIdx <= -1)
			{
				m_CurFrmIdx = 0;
				m_bFinish = true;
			}
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
	data.FlipAnimXY = (data.FlipAnimXY & ~(1 << 1)) | ((m_Animator->GetFlipX() == UNIT_DIRX::LEFT) ? (1 << 1) : 0);
	data.FlipAnimXY = (data.FlipAnimXY & ~(1 << 0)) | ((m_Animator->GetFlipY() == UNIT_DIRY::DOWN) ? (1 << 0) : 0);
	data.vLeftTop = m_vecFrm[m_CurFrmIdx].vLeftTopUV;
	data.vOffset = m_vecFrm[m_CurFrmIdx].vOffsetUV;
	data.vCutSize = m_vecFrm[m_CurFrmIdx].vCutSizeUV;
	data.vBackgroundSize = m_vecFrm[m_CurFrmIdx].vBackgroundSize;
	data.vAtlasSize = Vec2(m_AtlasTex->GetWidth(), m_AtlasTex->GetHeight());

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
		FAnimFrm frm = {};
		frm.vCutSizeUV = Vec2(_vCutSize.x / (float)_AtlasTex->GetWidth(), _vCutSize.y / (float)_AtlasTex->GetHeight());
		frm.vLeftTopUV = Vec2(_vLeftTop.x / (float)_AtlasTex->GetWidth() + frm.vCutSizeUV.x * i, _vLeftTop.y / (float)_AtlasTex->GetHeight());
		frm.vOffsetUV = Vec2(_vOffset.x / (float)_AtlasTex->GetWidth(), _vOffset.y / (float)_AtlasTex->GetHeight());
		frm.fDuration = 1.f / _FPS;
		frm.vBackgroundSize = Vec2(_vBackground.x / (float)_AtlasTex->GetWidth(), _vBackground.y / (float)_AtlasTex->GetHeight());

		m_vecFrm.push_back(frm);
	}
}

void CAnim::Create(CAnimator2D* _Animator, Ptr<CTexture> _Atlas, const vector<FAnimFrm> _vecFrm, bool aleardyUV)
{
	m_Animator = _Animator;
	m_AtlasTex = _Atlas;

	if (not aleardyUV)
	{
		for (int i = 0; i < _vecFrm.size(); ++i)
		{
			FAnimFrm frm = {};
			frm.vCutSizeUV = Vec2(_vecFrm[i].vCutSizeUV.x / (float)_Atlas->GetWidth(), _vecFrm[i].vCutSizeUV.y / (float)_Atlas->GetHeight());
			frm.vLeftTopUV = Vec2(_vecFrm[i].vLeftTopUV.x / (float)_Atlas->GetWidth(), _vecFrm[i].vLeftTopUV.y / (float)_Atlas->GetHeight());
			frm.vOffsetUV = Vec2(_vecFrm[i].vOffsetUV.x / (float)_Atlas->GetWidth(), _vecFrm[i].vOffsetUV.y / (float)_Atlas->GetHeight());
			frm.fDuration = _vecFrm[i].fDuration;
			frm.vBackgroundSize = Vec2(_vecFrm[i].vBackgroundSize.x / (float)_Atlas->GetWidth(), _vecFrm[i].vBackgroundSize.y / (float)_Atlas->GetHeight());

			m_vecFrm.push_back(frm);
		}
	}
	else
	{
		m_vecFrm = _vecFrm;
	}
}

void CAnim::SetReverse()
{
	m_bReverse = true;
	m_CurFrmIdx = (int)m_vecFrm.size() - 1;
}

vector<float> CAnim::GetDurationList()
{
	vector<float> ret;

	for (FAnimFrm& it : m_vecFrm)
		ret.push_back(it.fDuration);

	return ret;
}

vector<Vec4> CAnim::GetOffsetList()
{
	vector<Vec4> ret;

	for (FAnimFrm& it : m_vecFrm)
		ret.push_back(Vec4(it.vOffsetUV.x, it.vOffsetUV.y, 0.f, 0.f));

	return ret;
}

void CAnim::SaveToFile(FILE* _File)
{
	// 애니메이션 이름 저장
	SaveWString(GetName(), _File);

	// 모든 프레임 정보 저장
	size_t FrmSize = m_vecFrm.size();
	fwrite(&FrmSize, sizeof(size_t), 1, _File);
	fwrite(m_vecFrm.data(), sizeof(FAnimFrm), m_vecFrm.size(), _File);

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
	fread(m_vecFrm.data(), sizeof(FAnimFrm), m_vecFrm.size(), _File);

	// 애니메이션이 참조하던 텍스쳐 정보 로드
	LoadAssetRef(m_AtlasTex, _File);
}