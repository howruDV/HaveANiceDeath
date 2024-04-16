#pragma once
#include "CAsset.h"
#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG
#pragma comment(lib, "FMOD/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/fmod64_vc.lib")
#endif

class CSound :
	public CAsset
{
public:
	static FMOD::System*	g_pFMOD;		// FMOD ������ Ŭ����

public:
	FMOD::Sound*			m_pSound;       // �ε��� ���� ��ü
	list<FMOD::Channel*>	m_listChannel;  // ���尡 ����ǰ� �ִ� ä��
	bool					m_bPlay;

public:
	// _iRoopCount : 0 (���ѹݺ�),  _fVolume : 0 ~ 1(Volume), _bOverlap : �Ҹ� ��ø ���ɿ���
	int Play(int _iRoopCount, float _fVolume = 1.f, bool _bOverlap = false);
	void Stop();

	// 0 ~ 1
	void SetVolume(float _f, int _iChannelIdx);

private:
	void RemoveChannel(FMOD::Channel* _pTargetChannel);
	friend FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
		, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
		, void* commanddata1, void* commanddata2);

public:
	virtual void UpdateData() {}

	virtual int Save(const wstring&) override { return S_OK; }
	virtual int Load(const wstring& _strFilePath) override;

public:
	void SetPlay(bool _Play) { m_bPlay = _Play; }
	bool IsPlaying() { return m_bPlay; }

public:
	CLONE_DISABLE(CSound);
	CSound(bool _Engine = false);
	~CSound();
};

