#pragma once
//그냥 맵만 불러옴
//스태틱 메쉬 로더하고 다른게 없음
//네비메쉬는 따로 관리할예정

#ifndef __MAP_OBJECT_H__
#define __MAP_OBJECT_H__
#include "GameObject.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)
class CStaticMesh;
class CTransform;

END_NAMESPACE


class CMapObject : public Engine::CGameObject
{
public:
	explicit CMapObject(_Device pDevice);
	explicit CMapObject(const CMapObject& other);
	virtual ~CMapObject() = default;

public:
	virtual HRESULT Ready_GameObject(void) PURE;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr) PURE;
	virtual _int Update_GameObject(const _float& fDeltaTime) PURE;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) PURE;
	virtual HRESULT Render_GameObject(void) PURE;

public:
	HRESULT Set_MapName(const _tchar* pMapName);
	const _tchar* Get_MapName();

public:
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

protected:
	Engine::CStaticMesh* m_pMeshCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	_tchar m_pMapName[256];
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;
	virtual void Free(void) override;

};


#endif // !__MAP_OBJECT_H__