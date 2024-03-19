#include "pch.h"
#include "CPathMgr.h"

wchar_t CPathMgr::g_szContent[255] = {};

void CPathMgr::init()
{
	GetCurrentDirectory(255, g_szContent);

	int Len = (int)wcslen(g_szContent);
	for (int i = Len - 1; i >= 0; --i)
	{
		if ('\\' == g_szContent[i])
		{
			g_szContent[i + 1] = '\0';
			break;
		}
	}

	wcscat_s(g_szContent, 255, L"Contents\\");
}

wstring CPathMgr::GetRelativePath(const wstring& _FullPath)
{
	wstring strContentPath = GetContentPath();

	// �Էµ� ��ο�, content ��ΰ� ������ ����
	if (-1 == _FullPath.find(strContentPath, 0))
		return nullptr;

	// Content ��� ������ ���ڿ� �������� �߶� ��ȯ
	wstring strRelativePath = _FullPath.substr(strContentPath.length(), _FullPath.length());

	return strRelativePath;
}