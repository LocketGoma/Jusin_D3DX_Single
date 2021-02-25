#pragma once

#ifndef __BASE_LOGO_SCENE_H__
#define __BASE_LOGO_SCENE_H__

#include "Scene.h"
#include "Base.h"

#include "LoadingScene.h"

class CBaseLogoScene : public Engine::CScene
{
private:
	explicit CBaseLogoScene(_Device pDevice);
	virtual ~CBaseLogoScene() = default;

public:
	virtual HRESULT		Ready_Scene(void);
	virtual _int		Update_Scene(const _float& fTimeDelta);
	virtual _int		LateUpdate_Scene(const _float& fTimeDelta);
	virtual void		Render_Scene(void);

public:
	HRESULT Ready_Resource(_Device pDevice);


private:
	HANDLE		m_hThread;
	
	CRITICAL_SECTION	m_Crit;
	LOADINGID m_eLoadingID;
	_bool m_bFinish;
	_tchar m_szString[256];

	CLoadingScene* m_pLoading;

public:
	static unsigned int __stdcall Thread_Load(void* pArg);

public:
	static CBaseLogoScene* Create(_Device pDevice, LOADINGID eLoadingID);

};


#endif // !__BASE_LOGO_SCENE_H__