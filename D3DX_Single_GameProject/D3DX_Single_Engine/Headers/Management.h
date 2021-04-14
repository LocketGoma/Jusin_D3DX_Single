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
#include "RenderTargetManager.h"
#include "SceneManager.h"
#include "GameObjectManager.h"
#include "PrototypeManager.h"
#include "GraphicResourceManager.h"
#include "FontManager.h"

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

public :
	//FontManager Setting
	HRESULT	Ready_Font(_Device pDevice, const _tchar * pFontTag, const _tchar * pFontType, const _uint & iWidth, const _uint & iHeight, const _uint & iWeight, _bool bExtra = false);

	void	Render_Font(const _tchar * pFontTag, const _tchar * pString, const _vec2 * pPos, D3DXCOLOR Color);

public:
	//SceneManager Setting
	HRESULT Setup_SceneManager(_int iMaxSceneIndex);
	HRESULT Setup_CurrentScene(_int iSceneIndex, class CScene* pCurrentScene);
	class CScene* Get_NowScene();
	void Render_Scene();

	CGameObject* Get_GameObject_From_Layer(const std::wstring & pLayerTag, const std::wstring & pObjectTag);
	CComponent* Get_Component_From_Layer(const std::wstring & pLayerTag, const std::wstring & pObjectTag, const wstring & pCompTag, COMPONENT_ID eID);
	//_uint	Update_Scene(_float fDeltaTime);
	//_uint	LateUpdate_Scene(_float fDeltaTime);

public :
	//KeyManager Setting
	 void Key_Update();
	_bool Key_Pressing(_uint iKey);
	_bool Key_Down(_uint iKey);
	_bool Key_Up(_uint iKey);	 

public: 
	//Renderer Setting
	HRESULT Add_RenderList(RENDERID eRenderID, class CGameObject* pGameObject);

public:
	//RenderTarget Setting
	HRESULT Ready_Sheader(_Device pDevice);

	HRESULT Ready_RenderTarget(const _tchar * pTargetTag, _Device pDevice, const _uint & iWidth, const _uint & iHeight, D3DFORMAT Format, D3DXCOLOR Color);

	HRESULT		Ready_MRT(const _tchar * pMRTTag, const _tchar * pTargetTag);
	HRESULT		Begin_MRT(const _tchar * pMRTTag);
	HRESULT		End_MRT(const _tchar * pMRTTag);
	HRESULT			Ready_DebugBuffer(const _tchar * pTargetTag, const _float & fX, const _float & fY, const _float & fSizeX, const _float & fSizeY);
	void			Render_DebugBuffer(const _tchar * pMRTTag);
	void			SetUp_OnShader(LPD3DXEFFECT & pEffect, const _tchar * pTargetTag, const char* pConstantTable);



public:
	//ProtoTypeManager & ResourceManager & GameObjectManager
	//1. PrototypeManager (Non-GraphicResource Component)
	HRESULT		Ready_Prototype(const _tchar * pProtoTag, CComponent * pInstance);
	CComponent* Clone_Prototype(const _tchar * pProtoTag);
	CComponent* Get_Component(const _tchar * pCompTag);

	//2. GraphicResourceManager
		//컨테이너 공간을 미리 얼마나 할당할것인지 지정.
	HRESULT Reserve_Size(const _uint& wSize);
	//디바이스 정보, 인덱스 정보, 버퍼 태그명, 버퍼 아이디, 사이즈 (X,Y), 버텍스별 간격
	HRESULT Ready_Buffer(_Device pDevice, const _uint & iIndex, const _tchar * pBufferTag, BUFFERID eID, const _ulong & dwCountX = 129, const _ulong & dwCountZ = 129, const _ulong & dwVTXInterval = 1);
	HRESULT	Ready_Texture(_Device pDevice, const _ushort & wContainerIdx, const _tchar * pTextureTag, TEXTYPE eType, const _tchar * pPath, const _uint & iCnt = 1);
	HRESULT Ready_Meshes(_Device pDevice, const _ushort & wContainerIdx, const _tchar * pMeshTag, MESHTYPE eType, const _tchar * pFilePath, const _tchar * pFileName);
	HRESULT Ready_Meshes_Force(_Device pDevice, const _ushort & wContainerIdx, const _tchar * pMeshTag, MESHTYPE eType, const _tchar * pFilePath, const _tchar * pFileName);	

	CComponent* Clone_Resource(const _uint & iIndex, const _tchar * pResourceTag);

	//3. GameObjectManager - 생성시 추가할것.
	HRESULT Add_GameObject_Prototype(const std::wstring & PrototypeTag, class CGameObject* pPrototype);
	CGameObject* Clone_GameObject(const std::wstring & PrototypeTag, void* pArg = nullptr);




public:
	virtual void Free() override;
	static void Release_Engine();

private:
	CGraphicDevice*			 m_pDeviceManager	  = nullptr;
	CRenderer*				 m_pRenderer		  = nullptr;
	CRenderTargetManager*	 m_pRenderManager	  = nullptr;
	CKeyManager*			 m_pKeyManager		  = nullptr;
	CFontManager*			 m_pFontManager		  = nullptr;
	CTimeManager*			 m_pTimeManager		  = nullptr;
	CSceneManager*			 m_pSceneManager	  = nullptr;
	CGameObjectManager*		 m_pGameObjectManager = nullptr;
	CPrototypeManager*		 m_pPrototypeManager  = nullptr;	//구 ComponentManager 역할
	CGraphicResourceManager* m_pGraphicManager	  = nullptr;	//구 ComponentManager에서 "그래픽 리소스"만 분리함.

//private :
//	CScene* m_pScene;

};

END_NAMESPACE

#endif // !__MANAGEMENT_H__