#include "pch.h"
#include "CTaskMgr.h"
#include "CRenderMgr.h"
#include <random>

void GamePlayStatic::SpawnGameObject(CGameObject* _Target, int _LayerIdx)
{
	FTask task = {};
	task.Type = TASK_TYPE::CREATE_OBJECT;
	task.Param_1 = (DWORD_PTR)_LayerIdx; //WORD: CPU가 한번에 처리할 수 있는 데이터의 크기 단위 (유지보수 문제로 OS와 관계없이 16bit로 유지)
	task.Param_2 = (DWORD_PTR)_Target;
	CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DestroyGameObject(CGameObject* _Target)
{
	FTask task = {};
	task.Type = TASK_TYPE::DELETE_OBJECT;
	task.Param_1 = (DWORD_PTR)_Target;
	CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	FDebugShapeInfo info = {};
	info.ShapeType = DEBUG_SHAPE::RECT;
	info.matWorld = _WorldMat;
	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _WorldRot, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	FDebugShapeInfo info = {};
	info.ShapeType = DEBUG_SHAPE::RECT;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = _vWorldScale;
	info.vWorldRot = _WorldRot;
	info.matWorld = XMMatrixScaling(_vWorldScale.x, _vWorldScale.y, _vWorldScale.z)
		* XMMatrixRotationX(_WorldRot.x) * XMMatrixRotationY(_WorldRot.y) * XMMatrixRotationZ(_WorldRot.z)
		* XMMatrixTranslation(_vWorldPos.x, _vWorldPos.y, _vWorldPos.z);
	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	FDebugShapeInfo info = {};
	info.ShapeType = DEBUG_SHAPE::CIRCLE;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fRadius * 2.f, _fRadius * 2.f, 1.f);
	info.vWorldRot = Vector3();
	info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
		* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y) * XMMatrixRotationZ(info.vWorldRot.z)
		* XMMatrixTranslation(_vWorldPos.x, _vWorldPos.y, _vWorldPos.z);
	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::DrawDebugCross(Vec3 _vWorldPos, float _fScale, Vec3 _Color, bool _bDepthTest, float _Duration)
{
	FDebugShapeInfo info = {};
	info.ShapeType = DEBUG_SHAPE::CROSS;
	info.vWorldPos = _vWorldPos;
	info.vWorldScale = Vec3(_fScale);
	info.vWorldRot = Vec3();
	info.matWorld = XMMatrixScaling(info.vWorldScale.x, info.vWorldScale.y, info.vWorldScale.z)
		* XMMatrixRotationX(info.vWorldRot.x) * XMMatrixRotationY(info.vWorldRot.y) * XMMatrixRotationZ(info.vWorldRot.z)
		* XMMatrixTranslation(_vWorldPos.x, _vWorldPos.y, _vWorldPos.z);
	info.vColor = _Color;
	info.bDepthTest = _bDepthTest;
	info.fDuration = _Duration;

	CRenderMgr::GetInst()->AddDebugShapeInfo(info);
}

void GamePlayStatic::AddAsset(CAsset* _Asset)
{
	FTask task = {};
	task.Type = TASK_TYPE::ADD_ASSET;
	task.Param_1 = (DWORD_PTR)_Asset;
	CTaskMgr::GetInst()->AddTask(task);
}

void GamePlayStatic::Play2DSound(const wstring& _SoundPath, int _Loop, float _Volume, bool _Overlap)
{
	Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(_SoundPath, _SoundPath);

	if (nullptr != pSound)
	{
		pSound->Play(_Loop, _Volume, _Overlap);
	}
}

void GamePlayStatic::Stop2DSound(const wstring& _SoundPath)
{
	Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(_SoundPath, _SoundPath);

	if (nullptr != pSound)
		pSound->Stop();
}

void GamePlayStatic::Play2DBGM(const wstring& _SoundPath, float _Volume)
{
	static Ptr<CSound> CurBGM = nullptr;

	Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(_SoundPath, _SoundPath);

	if (nullptr != pSound)
	{
		if (nullptr != CurBGM)
		{
			CurBGM->Stop();
		}

		pSound->Play(0, _Volume);
		CurBGM = pSound;
	}
}

void GamePlayStatic::Stop2DBGM(const wstring& _SoundPath)
{
	Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(_SoundPath, _SoundPath);
	if (nullptr != pSound)
		pSound->Stop();
}

void Vector3::ToDegree()
{
	x = (x / XM_PI) * 180.f;
	y = (y / XM_PI) * 180.f;
	z = (z / XM_PI) * 180.f;
}

void Vector3::ToRadian()
{
	x = (x / 180.f) * XM_PI;
	y = (y / 180.f) * XM_PI;
	z = (z / 180.f) * XM_PI;
}

int Random(int min, int max)
{
	int ret = 0;
	std::random_device rd;  // true 랜덤 수 발생기
	std::mt19937 gen(rd()); // mersenne_twister_engine 알고리즘 사용
	std::uniform_int_distribution<int> dis(min, max); // 분포 설정

	return dis(gen); // 랜덤 정수 반환
}

string WstrToStr(const wstring& _wstr)
{
	return string(_wstr.begin(), _wstr.end());
}

wstring StrToWstr(const string& _str)
{
	return wstring(_str.begin(), _str.end());
}

void SaveWString(const wstring& _str, FILE* _File)
{
	size_t len = _str.length();
	fwrite(&len, sizeof(size_t), 1, _File);
	fwrite(_str.c_str(), sizeof(wchar_t), _str.length(), _File);
}

void LoadWString(wstring& _str, FILE* _FILE)
{
	size_t len = 0;
	wchar_t szBuff[256] = {};
	fread(&len, sizeof(size_t), 1, _FILE);
	fread(szBuff, sizeof(wchar_t), len, _FILE);
	_str = szBuff;
}