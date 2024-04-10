#include "pch.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CGC.h"

CLayer::CLayer()
	: m_iLayerIdx(-1)
{
}

CLayer::CLayer(const CLayer& _OriginLayer)
	: CEntity(_OriginLayer)
	, m_iLayerIdx(_OriginLayer.m_iLayerIdx)
{
	for (size_t i = 0; i < _OriginLayer.m_vecParent.size(); ++i)
	{
		AddObject(_OriginLayer.m_vecParent[i]->Clone(), false);
	}
}

CLayer::~CLayer()
{
	Delete_Vec(m_vecParent);
}

void CLayer::begin()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
		m_vecParent[i]->begin();
}

void CLayer::tick()
{
	for (size_t i = 0; i < m_vecParent.size(); ++i)
		m_vecParent[i]->tick();
}

void CLayer::finaltick()
{
	vector<CGameObject*>::iterator iter = m_vecParent.begin();

	for (; iter != m_vecParent.end();)
	{
		(*iter)->finaltick();

		if ((*iter)->IsDead())
		{
			CGC::GetInst()->Add(*iter);
			iter = m_vecParent.erase(iter);
		}
		else
			++iter;
	}
}

void CLayer::AddObject(CGameObject* _Object, bool _bMove)
{
	// _bMove: true	 - Object�� �ڽ��� �ִ� ���, �ڽı��� ��� �ش� ���̾�� �Ѿ��
	// _bMove: false - Objet�� �ڽ��� �ش� ���̾�� �Ѿ���� ����

	// case: vecParent
	if (!_Object->GetParent())
	{
		// �ٸ� layer �Ҽ�
		if (_Object->m_iLayerIdx != -1)
			_Object->DisconnectWithLayer(); // delete from prev layer's parent vector

		m_vecParent.push_back(_Object);
	}

	// case: vecObject
	// - change Obj + child's layer
	list<CGameObject*> queue;
	queue.push_back(_Object);

	while (!queue.empty())
	{
		CGameObject* pObj = queue.front();
		queue.pop_front();

		for (size_t i = 0; i < pObj->m_vecChild.size(); ++i)
			queue.push_back(pObj->m_vecChild[i]);

		// ���� �Է� obj�̰ų�, _bMove�� true�̰ų�, ���̾� �Ҽ��� ���� ��� ����
		if (pObj == _Object || _bMove || pObj->m_iLayerIdx == -1)
			pObj->m_iLayerIdx = m_iLayerIdx;
	}
}

void CLayer::AddObject_Load(CGameObject* _Object, bool _bMove)
{
	assert(m_iLayerIdx == _Object->m_iLayerIdx);

	// �ֻ��� �θ� ������Ʈ����.
	if (nullptr == _Object->GetParent())
	{
		// �ֻ��� �θ� ������Ʈ�� m_vecParent ���� ����Ű���� �Ѵ�.
		m_vecParent.push_back(_Object);
	}
}

void CLayer::DetachGameObject(CGameObject* _Object)
{
	assert(!(_Object->m_iLayerIdx == -1 || _Object->m_iLayerIdx != m_iLayerIdx));

	_Object->m_iLayerIdx = -1;

	// case: top parent object
	if (!_Object->GetParent())
	{
		vector<CGameObject*>::iterator iter = find(m_vecParent.begin(), m_vecParent.end(), _Object);
		if (iter != m_vecParent.end())
			m_vecParent.erase(iter);
		else
			assert(nullptr);
	}
}