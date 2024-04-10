#include "pch.h"
#include "CGameObject.h"
#include "CComponent.h"
#include "CRenderComponent.h"
#include "CScript.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CLayer.h"
#include "CGC.h"
#include "CRenderMgr.h"
#include "CTransform.h"

CGameObject::CGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1)	// 어떤 Layer에도 소속되지 않은 상태
	, m_bDead(false)
	, m_bActivate(true)
{
}

CGameObject::CGameObject(const CGameObject& _OriginObject)
	: CEntity(_OriginObject)
	, m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_iLayerIdx(-1)
	, m_bDead(false)
	, m_bActivate(_OriginObject.m_bActivate)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == _OriginObject.m_arrCom[i])
			continue;

		AddComponent(_OriginObject.m_arrCom[i]->Clone());
	}

	for (size_t i = 0; i < _OriginObject.m_vecScript.size(); ++i)
	{
		AddComponent(_OriginObject.m_vecScript[i]->Clone());
	}

	// 복사되는 GameObject 는 부모만 레이어소속을 -1 로 하고,
	// 자식들은 원본객체랑 동일한 레이어소속을 유지한다.
	// @Todo: 레이어는 똑같이 있어도 괜찮지않나?
	for (size_t i = 0; i < _OriginObject.m_vecChild.size(); ++i)
	{
		CGameObject* ChildClone = _OriginObject.m_vecChild[i]->Clone();
		AddChild(ChildClone);
		ChildClone->m_iLayerIdx = _OriginObject.m_vecChild[i]->m_iLayerIdx;
	}
}

CGameObject::~CGameObject()
{
	Delete_Array(m_arrCom);
	Delete_Vec(m_vecScript);
	Delete_Vec(m_vecChild);
}

void CGameObject::begin()
{
	if (!m_bActivate)
		return;

	size_t CurChildSize = m_vecChild.size();

	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (m_arrCom[i])
			m_arrCom[i]->begin();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
		m_vecScript[i]->begin();

	for (size_t i = 0; i < CurChildSize; ++i)
		m_vecChild[i]->begin();
}

void CGameObject::tick()
{
	if (!m_bActivate)
		return;

	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (m_arrCom[i])
			m_arrCom[i]->tick();
	}

	for (size_t i = 0; i < m_vecScript.size(); ++i)
		m_vecScript[i]->tick();

	for (size_t i = 0; i < m_vecChild.size(); ++i)
		m_vecChild[i]->tick();
}

void CGameObject::finaltick()
{
	if (!m_bActivate)
		return;

	for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
	{
		if (m_arrCom[i])
			m_arrCom[i]->finaltick();
	}

	CLayer* pCurLayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(m_iLayerIdx);	
	pCurLayer->RegisterGameObject(this);

	vector<CGameObject*>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end();)
	{
		(*iter)->finaltick();

		if ((*iter)->m_bDead)
		{
			CGC::GetInst()->Add(*iter);
			iter = m_vecChild.erase(iter);
		}
		else
			iter++;
	}

	// Register Debug Draw
	if (GetLayerIdx() == 31 || m_arrCom[(UINT)COMPONENT_TYPE::CAMERA])
		return;

	if (CRenderMgr::GetInst()->IsDebugPosition())
		GamePlayStatic::DrawDebugCross(Transform()->GetWorldPos(), 20.f, Vec3(0.1f, 1.f, 0.f), true);
}

void CGameObject::render()
{
	if (!m_bActivate)
		return;

	if (m_RenderCom)
		m_RenderCom->render();
}

// Usage: disconnet childObj with Parent and return prev layer idx
// @TODO: 왜 Layer Idx를 바꿔야하지?
int CGameObject::DisconnectWithParent()
{
	// 부모가 없는 오브젝트에 호출한 경우
	if (!m_Parent)
		return -1;

	vector<CGameObject*>::iterator iter = find(m_Parent->m_vecChild.begin(), m_Parent->m_vecChild.end(), this);
	
	if (iter != m_Parent->m_vecChild.end())
	{
		int prevLayer = m_iLayerIdx;
		m_Parent->m_vecChild.erase(iter);
		m_Parent = nullptr;
		m_iLayerIdx = -1;

		return prevLayer;
	}
	// 부모는 자식을 가리키지 않는데, 자식은 부모를 가리키는 경우
	else
	{
		assert(nullptr);
	}

	return -1;
}

// Usage:: disconnect gameObj with layer and return prev Layer Idx
int CGameObject::DisconnectWithLayer()
{
	if (m_iLayerIdx == -1)
		return -1;

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pCurLayer = pCurLevel->GetLayer(m_iLayerIdx);

	int LayerIdx = m_iLayerIdx;
	pCurLayer->DetachGameObject(this);
	return LayerIdx;
}

int CGameObject::ChangeLayer(int _Idx)
{
	DisconnectWithLayer();
	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(this, _Idx, false);

	return 0;
}

int CGameObject::ChangeLayer(const wstring& _Layer)
{
	DisconnectWithLayer();
	CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(this, _Layer, false);

	return 0;
}

void CGameObject::Destroy()
{
	GamePlayStatic::DestroyGameObject(this);
}

void CGameObject::DeleteComponent(COMPONENT_TYPE _Type)
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (LEVEL_STATE::PLAY == pLevel->GetState() || LEVEL_STATE::PAUSE == pLevel->GetState())
	{
		return;
	}

	if (m_arrCom[(UINT)_Type])
	{
		delete m_arrCom[(UINT)_Type];
		m_arrCom[(UINT)_Type] = nullptr;

		if (COMPONENT_TYPE::MESHRENDER == _Type || COMPONENT_TYPE::TILEMAP == _Type || COMPONENT_TYPE::PARTICLESYSTEM == _Type)
		{
			m_RenderCom = nullptr;
		}
	}
}

void CGameObject::AddComponent(CComponent* _Component)
{
	COMPONENT_TYPE type = _Component->GetType();

	if (type == COMPONENT_TYPE::SCRIPT)
	{
		// type isn't script
		assert(dynamic_cast<CScript*>(_Component));
		m_vecScript.push_back((CScript*)_Component);
		_Component->m_Owner = this;
	}
	else
	{
		// already exist
		if (m_arrCom[(UINT)type])
		{
			MessageBox(nullptr, L"Already exist component", L"Failed to Add Component", MB_OK);
			return;
		}

		// render component
		CRenderComponent* tmp = dynamic_cast<CRenderComponent*>(_Component);
		if (tmp)
		{
			// already exist
			if (m_RenderCom)
			{
				MessageBox(nullptr, L"Render component is already exist!", L"Failed to Add Component", MB_OK);
				return;
			}

			m_RenderCom = tmp;
		}

		m_arrCom[(UINT)type] = _Component;
		_Component->m_Owner = this;
	}
}

void CGameObject::AddChild(CGameObject* _Child)
{
	// case: Level에 속하지 않았던 Object가 들어오는 경우
	if (_Child->m_iLayerIdx == -1)
	{
		_Child->m_iLayerIdx = m_iLayerIdx;
	}
	// case: 최상위 부모가 child로 들어오는 경우
	else if (!_Child->m_Parent)
	{
		// 소속 레이어의 Parent 오브젝트 목록에서 제거되어야 함
		int LayerIdx = _Child->DisconnectWithLayer();
		_Child->m_iLayerIdx = LayerIdx;
	}
	else
	{
		int LayerIdx = _Child->DisconnectWithParent();
		_Child->m_iLayerIdx = LayerIdx;
	}

	_Child->m_Parent = this;
	m_vecChild.push_back(_Child);
}

void CGameObject::AddChild_Load(CGameObject* _Child)
{
	// 부모 자식 연결
	_Child->m_Parent = this;
	m_vecChild.push_back(_Child);
}

CGameObject* CGameObject::GetChildByName(const wstring& _strName)
{
	for (CGameObject* it : m_vecChild)
	{
		if (it->GetName() == _strName)
			return it;
	}

	return nullptr;
}

bool CGameObject::IsAncestor(CGameObject* _Other)
{
	CGameObject* pParent = m_Parent;

	while (pParent)
	{
		if (pParent == _Other)
			return true;

		pParent = pParent->m_Parent;
	}

	return false;
}
