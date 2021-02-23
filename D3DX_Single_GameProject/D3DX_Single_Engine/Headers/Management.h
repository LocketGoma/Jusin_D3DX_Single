#pragma once

#ifndef __MANAGEMENT_H__
#define __MANAGEMENT_H__

#include "Engine_Include.h"
#include "Base.h"
#include "Scene.h"

#include "GraphicDevice.h"
#include "TimeManager.h"
#include "KeyManager.h"
#include "Renderer.h"
#include "SceneManager.h"

BEGIN_NAMESPACE(Engine)
//마스터 클래스
//작정하고 ㄹㅇ 슈퍼 클래스로 만들 생각.
class ENGINE_DLL CManagement final : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	CManagement();
	virtual ~CManagement() = default;

	
public: 
	//base Setting
	HRESULT Ready_Engine(HWND hWnd, int iWinCX, int iWinCY, WINMODE eDisplayMode, _uint iSceneCount);
	_uint Update_Engine(_float fDeltaTime);
	_uint LateUpdate_Engine(_float fDeltaTime);
	HRESULT Render_Engine(HWND hWnd = nullptr);

public :
	//GraphicDevice Setting
	_Device	Get_Device();

public :
	//TimeManager Setting
	//HRESULT Ready_Timer();
	_float Get_DeltaTime();

public:
	//SceneManager Setting
	HRESULT Setup_CurrentScene(_int iSceneIndex, class CScene* pCurrentScene);
	//_uint	Update_Scene(_float fDeltaTime);
	//_uint	LateUpdate_Scene(_float fDeltaTime);

public :
	//KeyManager Setting
	_bool Key_Pressing(_uint iKey);
	_bool Key_Down(_uint iKey);
	_bool Key_Up(_uint iKey);	 

public: /*For.Renderer*/
	HRESULT Add_RenderList(RENDERID eRenderID, class CGameObject* pGameObject);


public:
	virtual void Free() override;
	static void Release_Engine();

private:
	CGraphicDevice* m_pDeviceManager	= nullptr;
	CRenderer*		m_pRenderer			= nullptr;
	CKeyManager*	m_pKeyManager		= nullptr;
	CTimeManager*	m_pTimeManager		= nullptr;
	CSceneManager*	m_pSceneManager		= nullptr;


//private :
//	CScene* m_pScene;

};

END_NAMESPACE

#endif // !__MANAGEMENT_H__