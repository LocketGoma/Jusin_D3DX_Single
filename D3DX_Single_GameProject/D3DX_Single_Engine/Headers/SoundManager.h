#pragma once

#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include "Base.h"

BEGIN_NAMESPACE(Engine)

//BGM : 배경음
//PLAYER : 플레이어 피격음 / 환경음 (삐익 등)
//ENEMY : 몬스터 (키에엑 소리)
//BOSS : 보스 (키에엑 소리)
//ENEMYDEAD : 몬스터 죽을때 소리
//EFFECTA : 총알 발사 소리 + 무기 작동 소리
//EFFECTB : 총알 소리
//EFFECTC : 피격 / 폭발소리
//UI : UI 전환 사운드
enum class SOUND_CHANNELID { BGM, PLAYER, ENEMY, BOSS, ENEMYDEAD, EFFECTA, EFFECTB, EFFECTC, UI, MAXCHANNEL };

class ENGINE_DLL CSoundManager : public CBase
{
	DECLARE_SINGLETON(CSoundManager);

private:
	explicit CSoundManager();
	virtual ~CSoundManager() = default;

public:
	HRESULT Initialize();

public:
	void PlaySound(TCHAR* pSoundKey, SOUND_CHANNELID eID);
	void PlayBGM(TCHAR* pSoundKey);
	void StopSound(SOUND_CHANNELID eID);
	void StopAll();

	virtual void Free() override;

public:
	HRESULT LoadSoundFiles();
	HRESULT LoadSoundFile(char* szFilePath, char* szFileName);


private:
	std::map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[(_uint)SOUND_CHANNELID::MAXCHANNEL];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;

};

END_NAMESPACE

#endif // !__SOUND_MANAGER_H__