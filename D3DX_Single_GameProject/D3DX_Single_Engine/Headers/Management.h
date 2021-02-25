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
#include "PrototypeManager.h"
#include "GraphicResourceManager.h"

BEGIN_NAMESPACE(Engine)
//������ Ŭ����
//�����ϰ� ���� ���� Ŭ������ ���� ����.
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
	HRESULT Setup_SceneManager(_int iMaxSceneIndex);
	HRESULT Setup_CurrentScene(_int iSceneIndex, class CScene* pCurrentScene);
	//_uint	Update_Scene(_float fDeltaTime);
	//_uint	LateUpdate_Scene(_float fDeltaTime);

public :
	//KeyManager Setting
	_bool Key_Pressing(_uint iKey);
	_bool Key_Down(_uint iKey);
	_bool Key_Up(_uint iKey);	 

public: 
	//Renderer Setting
	HRESULT Add_RenderList(RENDERID eRenderID, class CGameObject* pGameObject);

public:
	//ProtoTypeManager & ResourceManager & GameObjectManager
	//1. PrototypeManager (Non-GraphicResource Component)
	HRESULT		Ready_Prototype(const _tchar * pProtoTag, CComponent * pInstance);
	CComponent* Clone_Prototype(const _tchar * pProtoTag);

	//2. GraphicResourceManager
		//�����̳� ������ �̸� �󸶳� �Ҵ��Ұ����� ����.
	HRESULT Reserve_Size(const _uint& wSize);
	//����̽� ����, �ε��� ����, ���� �±׸�, ���� ���̵�, ������ (X,Y), ���ؽ��� ����
	HRESULT Ready_Buffer(_Device pDevice, const _uint & iIndex, const _tchar * pBufferTag, BUFFERID eID, const _ulong & dwCountX = 129, const _ulong & dwCountZ = 129, const _ulong & dwVTXInterval = 1);
	HRESULT	Ready_Texture(_Device pDevice, const _ushort & wContainerIdx, const _tchar * pTextureTag, TEXTYPE eType, const _tchar * pPath, const _uint & iCnt = 1);


	CComponent* Clone_Resource(const _uint & iIndex, const _tchar * pResourceTag);

	//3. GameObjectManager - ������ �߰��Ұ�.


public:
	virtual void Free() override;
	static void Release_Engine();

private:
	CGraphicDevice*				m_pDeviceManager	= nullptr;
	CRenderer*					m_pRenderer			= nullptr;
	CKeyManager*				m_pKeyManager		= nullptr;
	CTimeManager*				m_pTimeManager		= nullptr;
	CSceneManager*				m_pSceneManager		= nullptr;
	CPrototypeManager*			m_pPrototypeManager = nullptr;	//�� ComponentManager ����
	CGraphicResourceManager*	m_pGraphicManager	= nullptr;	//�� ComponentManager���� "�׷��� ���ҽ�"�� �и���.

//private :
//	CScene* m_pScene;

};

END_NAMESPACE

#endif // !__MANAGEMENT_H__