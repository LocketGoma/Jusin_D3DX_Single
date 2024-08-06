#pragma once

#ifndef __GAMEOBJECT_MANAGER_H__
#define __GAMEOBJECT_MANAGER_H__

#include "Engine_Include.h"
#include "Base.h"
#include "Component.h"
#include "GameObject.h"

///
/// 1. 씬에 관여하지 않는 게임오브젝트 객체 보관용 클래스
/// 2. 게임오브젝트 매니저는 프로토타입만 가지고 있고, 요구가 있을시 clone을 해준다
/// 3. clone 의 해제는 각 씬에서 해결한다.
/// 4. 프로그램이 종료될때 프로토타입도 같이 해결한다.
/// 


BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CGameObjectManager : public CBase
{
	DECLARE_SINGLETON(CGameObjectManager)

private:
	explicit CGameObjectManager();
	virtual ~CGameObjectManager() = default;


public:
	class CGameObject* Get_GameObject(_uint iSceneIndex, const std::wstring& LayerTag, _uint iIndex = 0);

public:
	HRESULT Ready_GameObject_Manager();
	HRESULT Add_GameObject_Prototype(const std::wstring& PrototypeTag, class CGameObject* pPrototype);
	CGameObject* Clone_GameObject(const std::wstring& PrototypeTag, void* pArg = nullptr);
	//HRESULT Clone_GameObject_ToLayer(_uint iFromSceneIndex, const std::wstring& PrototypeTag, _uint iToSceneIndex, const std::wstring& LayerTag, void* pArg = nullptr);

public:
	virtual void Free(void);

private:
	typedef std::map<std::wstring, class CGameObject*> GAMEOBJECTS;
	//GAMEOBJECTS* m_pGameObjects = nullptr;
	GAMEOBJECTS m_pGameObject;			//동적배열 아님

};


END_NAMESPACE


#endif