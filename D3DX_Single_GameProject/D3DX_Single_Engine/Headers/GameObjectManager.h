#pragma once

#ifndef __GAMEOBJECT_MANAGER_H__
#define __GAMEOBJECT_MANAGER_H__

#include "Engine_Include.h"
#include "Base.h"
#include "Component.h"
#include "GameObject.h"

///
/// 1. ���� �������� �ʴ� ���ӿ�����Ʈ ��ü ������ Ŭ����
/// 2. ���ӿ�����Ʈ �Ŵ����� ������Ÿ�Ը� ������ �ְ�, �䱸�� ������ clone�� ���ش�
/// 3. clone �� ������ �� ������ �ذ��Ѵ�.
/// 4. ���α׷��� ����ɶ� ������Ÿ�Ե� ���� �ذ��Ѵ�.
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
	GAMEOBJECTS m_pGameObject;			//�����迭 �ƴ�

};


END_NAMESPACE


#endif