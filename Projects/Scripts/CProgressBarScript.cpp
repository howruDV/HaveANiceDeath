#include "pch.h"
#include "CProgressBarScript.h"

#include <Engine\CTimeMgr.h>
#include <Engine\CTransform.h>

CProgressBarScript::CProgressBarScript(UINT _ScriptType)
	: CScript(_ScriptType)
	, m_BarL(nullptr)
	, m_BarM(nullptr)
	, m_BarR(nullptr)
	, m_BarM_Extra(nullptr)
	, m_BarR_Extra(nullptr)
	, m_PrevDecAcc(0.f)
	, m_PrevIncAcc(0.f)
	, m_pUnit(nullptr)
	, m_fWidth(0.f)
	, m_fDecTime(0.f)
	, m_fIncTime(0.f)
	, m_fProgressSpeed(0.f)
{
}

CProgressBarScript::~CProgressBarScript()
{
}

void CProgressBarScript::begin()
{
	m_BarL = GetOwner()->GetChildByName(L"Bar_L");
	m_BarM = GetOwner()->GetChildByName(L"Bar_M");
	m_BarR = GetOwner()->GetChildByName(L"Bar_R");
	m_BarM_Extra = GetOwner()->GetChildByName(L"Bar_M_Extra");
	m_BarR_Extra = GetOwner()->GetChildByName(L"Bar_R_Extra");

	m_BarM_Extra->Deactivate();
	m_BarR_Extra->Deactivate();

	if (m_DefaultColor != Vec4())
	{
		m_BarL->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, m_DefaultColor);
		m_BarM->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, m_DefaultColor);
		m_BarR->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, m_DefaultColor);
	}
}

void CProgressBarScript::tick()
{
	// 구성: background(max), avail, cur
	// bar: damage 발생하면 hp도 전달받음 (cur hp, prev available hp)
	// avail hp 증감: 바로 표시
	// cur hp 감소: 줄어든만큼 빨간색 표시 << 없어짐 
	// cur hp 증가: 증가한만큼 파란색 표시 >> 없어짐

	if (!m_queueEvent.empty())
	{
		BAR_EVENT& curEvent = m_queueEvent.front();

		Vec3 posM = m_BarM->Transform()->GetRelativePos();
		Vec3 posR = m_BarR->Transform()->GetRelativePos();
		posM.x = m_BarL->Transform()->GetRelativePos().x + m_BarL->Transform()->GetRelativeScale().x / 2.f;
		posR.x = posM.x + m_BarR->Transform()->GetRelativeScale().x / 2.f;

		Vec3 posM_Extra = m_BarM_Extra->Transform()->GetRelativePos();
		Vec3 posR_Extra = m_BarR_Extra->Transform()->GetRelativePos();
		posM_Extra.x = posM.x;
		posR_Extra.x = posR.x;

		if (curEvent.Type == BAR_EVENT_TYPE::DEC_BAR)
		{
			// update transform
			Vec3 scale = m_BarM->Transform()->GetRelativeScale();
			scale.x = m_fWidth * GetProgress();
			posM.x += scale.x / 2.f;
			posR.x += scale.x;

			// set transform
			m_BarM_Extra->Transform()->SetRelativeScale(m_BarM->Transform()->GetRelativeScale());
			m_BarM_Extra->Transform()->SetRelativePos(posM_Extra);
			m_BarR_Extra->Transform()->SetRelativePos(posR_Extra);
			m_BarM->Transform()->SetRelativeScale(scale);
			m_BarM->Transform()->SetRelativePos(posM);
			m_BarR->Transform()->SetRelativePos(posR);

			// activate
			m_BarM_Extra->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, m_DecColor);
			m_BarR_Extra->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, m_DecColor);
			m_BarM_Extra->Activate();
			m_BarR_Extra->Activate();
		}

		else if (curEvent.Type == BAR_EVENT_TYPE::DEC_EXTRA)
		{
			m_PrevDecAcc -= DT;

			// update transform
			float speed = m_fProgressSpeed * (curEvent.Diff / curEvent.PlayTIme);
			Vec3 scale = m_BarM_Extra->Transform()->GetRelativeScale();

			scale.x -= speed * DT;
			posM_Extra.x += scale.x / 2.f;
			posR_Extra.x += scale.x;
			
			// deactivate
			if (scale.x <= 0 || curEvent.AccTime + DT >= curEvent.PlayTIme)
			{
				scale.x = 0.f;
				curEvent.AccTime = curEvent.PlayTIme;

				m_BarM_Extra->Deactivate();
				m_BarR_Extra->Deactivate();
			}

			// set transform
			m_BarM_Extra->Transform()->SetRelativeScale(scale);
			m_BarM_Extra->Transform()->SetRelativePos(posM_Extra);
			m_BarR_Extra->Transform()->SetRelativePos(posR_Extra);
		}

		else if (curEvent.Type == BAR_EVENT_TYPE::INC_BAR)
		{
			// update transform
			float speed = m_fProgressSpeed * (curEvent.Diff / curEvent.PlayTIme);
			Vec3 scale = m_BarM->Transform()->GetRelativeScale();

			scale.x += speed * DT;
			posM.x += scale.x / 2.f;
			posR.x += scale.x;

			// deactivate
			if (scale.x >= m_BarM_Extra->Transform()->GetRelativeScale().x || curEvent.AccTime + DT >= curEvent.PlayTIme)
			{
				scale.x = m_BarM_Extra->Transform()->GetRelativeScale().x;
				curEvent.AccTime = curEvent.PlayTIme;

				m_BarM_Extra->Deactivate();
				m_BarR_Extra->Deactivate();
			}

			// set transform
			m_BarM->Transform()->SetRelativeScale(scale);
			m_BarM->Transform()->SetRelativePos(posM);
			m_BarR->Transform()->SetRelativePos(posR);
		}

		else if (curEvent.Type == BAR_EVENT_TYPE::INC_EXTRA)
		{
			// update transform
			Vec3 scale = m_BarM->Transform()->GetRelativeScale();
			scale.x = m_fWidth * GetProgress();

			posM_Extra.x += scale.x / 2.f;
			posR_Extra.x += scale.x;

			// activate
			m_BarM_Extra->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, m_IncColor);
			m_BarR_Extra->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, m_IncColor);
			m_BarM_Extra->Activate();
			m_BarR_Extra->Activate();

			// set transform
			m_BarM_Extra->Transform()->SetRelativeScale(scale);
			m_BarM_Extra->Transform()->SetRelativePos(posM_Extra);
			m_BarR_Extra->Transform()->SetRelativePos(posR_Extra);
		}

		// update Time
		curEvent.AccTime += DT;
		if (curEvent.AccTime >= curEvent.PlayTIme)
			m_queueEvent.pop_front();
	}
}

void CProgressBarScript::SaveToFile(FILE* _File)
{
}

void CProgressBarScript::LoadFromFile(FILE* _File)
{
}

void CProgressBarScript::SetProgress(float _progress)
{
	if (_progress > 1.f)
		_progress = 1.f;

	// update transform
	Vec3 scale = m_BarM->Transform()->GetRelativeScale();
	Vec3 posM = m_BarM->Transform()->GetRelativePos();
	Vec3 posR = m_BarR->Transform()->GetRelativePos();
	posM.x = m_BarL->Transform()->GetRelativePos().x + m_BarL->Transform()->GetRelativeScale().x / 2.f;
	posR.x = posM.x + m_BarR->Transform()->GetRelativeScale().x / 2.f;

	scale.x = m_fWidth * _progress;
	posM.x += scale.x / 2.f;
	posR.x += scale.x;

	// set transform
	m_BarM->Transform()->SetRelativeScale(scale);
	m_BarM->Transform()->SetRelativePos(posM);
	m_BarR->Transform()->SetRelativePos(posR);
}

void CProgressBarScript::Decrease(float _diff)
{
	// case: already in event queue
	if (m_PrevDecAcc > 0.f)
	{
		BAR_EVENT* DecExtraEvent = nullptr;
		deque<BAR_EVENT>::iterator iter = m_queueEvent.begin();

		for (BAR_EVENT it : m_queueEvent)
		{
			if (it.Type == BAR_EVENT_TYPE::DEC_BAR)
			{
				DecExtraEvent = &it;
				break;
			}
			iter++;
		}

		if (DecExtraEvent)
			DecExtraEvent->Diff += _diff;

		if (iter == m_queueEvent.begin())
		{
			BAR_EVENT event{ BAR_EVENT_TYPE::DEC_BAR, 0.f, 0.f, _diff };
			m_queueEvent.push_front(event);
		}
	}

	else
	{
		m_PrevDecAcc = m_fDecTime;

		BAR_EVENT event{ BAR_EVENT_TYPE::DEC_BAR, 0.f, 0.f, _diff };
		m_queueEvent.push_back(event);

		event = { BAR_EVENT_TYPE::DEC_EXTRA, m_fDecTime, 0.f, _diff };
		m_queueEvent.push_back(event);
	}
}

void CProgressBarScript::Increase(float _diff)
{
	BAR_EVENT event = { BAR_EVENT_TYPE::INC_EXTRA, 0.f, 0.f, _diff };
	m_queueEvent.push_back(event);

	event = { BAR_EVENT_TYPE::INC_BAR, m_fIncTime, 0.f, _diff };
	m_queueEvent.push_back(event);
}
