#pragma once

#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include "Base.h"

BEGIN_NAMESPACE(Engine)


//BGM : �����
//PLAYER : �÷��̾� �ǰ��� / ȯ���� (���� ��)
//PLAYERSUIT : HEV ��ȣ�� ȿ����
//ENEMY : ���� (Ű���� �Ҹ�)
//ENEMYA,B,C,D,E : ���� (Ű���� �Ҹ�) (ȯ���� �߰���)
//BOSS : ���� (Ű���� �Ҹ�)
//ENEMYDEAD : ���� ������ �Ҹ�
//EFFECTA : �Ѿ� �߻� �Ҹ� + ���� �۵� �Ҹ�
//EFFECTB : �Ѿ� �Ҹ�
//EFFECTC : �ǰ� / ���߼Ҹ�
//UI : UI ��ȯ ����
enum class SOUND_CHANNELID { BGM, PLAYER, PLAYERSUIT,ENEMY, ENEMYA, ENEMYB, ENEMYC, ENEMYD, ENEMYE, BOSS, ENEMYDEAD, EFFECTA, EFFECTB, EFFECTC, EFFECTD, EFFECTE, UI, MAXCHANNEL };

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
	bool Check_PlaySound(SOUND_CHANNELID eID);
	void StopSound(SOUND_CHANNELID eID);
	void StopAll();

	virtual void Free() override;

public:
	HRESULT LoadSoundFiles();
	HRESULT LoadSoundFile(char* szFilePath, char* szFileName);


private:
	std::map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[(_uint)SOUND_CHANNELID::MAXCHANNEL];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;

};

END_NAMESPACE

#endif // !__SOUND_MANAGER_H__