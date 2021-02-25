#pragma once

#ifndef __BASE_LOGO_SCENE_H__
#define __BASE_LOGO_SCENE_H__


#include "Scene.h"
#include "Base.h"

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

private:
	HANDLE		m_hThread;
	CRITICAL_SECTION	m_Crit;
	LOADINGID m_eLoadingID;

public:
	static unsigned int __stdcall Thread_Load(void* pArg);

};


#endif // !__BASE_LOGO_SCENE_H__