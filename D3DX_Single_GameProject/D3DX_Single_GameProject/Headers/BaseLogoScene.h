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
	virtual _int		Update_Scene(const _float& fDeltaTime);
	virtual _int		LateUpdate_Scene(const _float& fDeltaTime);
	virtual void		Render_Scene(void);

public:
	HRESULT Ready_Resource(_Device pDevice);
	HRESULT Ready_Layer(const _tchar* pLayerTag);

private:
	HANDLE		m_hThread;
	
	CRITICAL_SECTION	m_Crit;
	LOADINGID m_eLoadingID;
	_bool m_bFinish;
	_tchar m_szString[256];

	CLoadingScene* m_pLoading;

public:
	static CBaseLogoScene* Create(_Device pDevice);

private:
	virtual void Free() override;

};


#endif // !__BASE_LOGO_SCENE_H__