#include "SoundManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CSoundManager)

CSoundManager::CSoundManager()
{
	m_pSystem = nullptr;
}

HRESULT CSoundManager::Initialize()
{
	FMOD_System_Create(&m_pSystem);

	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	if (FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL) != FMOD_OK)
	{
		return E_FAIL;
	}
	return S_OK;
}

void CSoundManager::PlaySound(TCHAR* pSoundKey, SOUND_CHANNELID eID)
{
	std::map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_BOOL bPlay = FALSE;
	if (FMOD_Channel_IsPlaying(m_pChannelArr[(_uint)eID], &bPlay))
	{
		FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[(_uint)eID]);
	}
	FMOD_System_Update(m_pSystem);
}

void CSoundManager::PlayBGM(TCHAR* pSoundKey)
{
	std::map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE, iter->second, FALSE, &m_pChannelArr[(_uint)SOUND_CHANNELID::BGM]);
	FMOD_Channel_SetMode(m_pChannelArr[(_uint)SOUND_CHANNELID::BGM], FMOD_LOOP_NORMAL);
	FMOD_System_Update(m_pSystem);
}

void CSoundManager::StopSound(SOUND_CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[_uint(eID)]);
}

void CSoundManager::StopAll()
{
	for (int i = 0; i < (_uint)SOUND_CHANNELID::MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

HRESULT CSoundManager::LoadSoundFiles()
{
	_finddata_t fd;

	long handle = _findfirst("../../Resource/Sound/*.*", &fd);

	if (handle == 0)
		return E_FAIL;

	int iResult = 0;																		

	char szCurPath[256] = "../../Resource/Sound/";
	char szFullPath[256] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, fd.name);
		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		iResult = _findnext(handle, &fd);

	}
	
	FMOD_System_Update(m_pSystem);
	_findclose(handle);

	return S_OK;
}

HRESULT CSoundManager::LoadSoundFile(char* szFilePath, char* szFileName)
{

	char szFullPath[256] = "";
	strcpy_s(szFullPath, szFilePath);
	strcat_s(szFullPath, szFileName);

	FMOD_SOUND* pSound = nullptr;

	FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE, 0, &pSound);

	if (eRes == FMOD_OK)
	{
		int iLength = strlen(szFileName) + 1;

		TCHAR* pSoundKey = new TCHAR[iLength];
		ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
		MultiByteToWideChar(CP_ACP, 0, szFileName, iLength, pSoundKey, iLength);

		m_mapSound.emplace(pSoundKey, pSound);
	}
	else
	{
		return E_FAIL;
	}


FMOD_System_Update(m_pSystem);

return S_OK;

}

void CSoundManager::Free()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}