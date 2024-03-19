#include "pch.h"
#include "CBlackboard.h"

CBlackboard::CBlackboard()
{
}

CBlackboard::CBlackboard(const CBlackboard& _board)
	: CEntity(_board)
	, m_mapBBData()
{
}

CBlackboard::~CBlackboard()
{
}

void CBlackboard::AddBlackboardData(const wstring& _strKey, BB_DATA _Type, void* _Data)
{
	unordered_map<wstring, FBlackboardData>::iterator iter = m_mapBBData.find(_strKey);
	assert(iter == m_mapBBData.end());

	m_mapBBData.insert(make_pair(_strKey, FBlackboardData{ _Type , _Data }));
}

void* CBlackboard::GetBlackboardData(const wstring& _strKey)
{
	unordered_map<wstring, FBlackboardData>::iterator iter = m_mapBBData.find(_strKey);

	if (iter == m_mapBBData.end())
		return nullptr;

	return iter->second.pData;
}