#pragma once

#ifndef __STATIC_OBJECT_H__
#define __STATIC_OBJECT_H__

#include "BaseObject.h"

BEGIN_NAMESPACE(Engine)
class CStaticMesh;

END_NAMESPACE

class CStaticObject abstract : public CBaseObject
{
protected:
	explicit CStaticObject(_Device pDevice);
	explicit CStaticObject(const CStaticObject& other);
	virtual ~CStaticObject() = default;

public:
// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) PURE;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) PURE;
	virtual _int Update_GameObject(const _float& fDeltaTime) PURE;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) PURE;
	virtual HRESULT Render_GameObject(void) PURE;

protected:
	virtual HRESULT Setup_ConstantTable(LPD3DXEFFECT& pEffect, _bool bDissolve = false) override;

//기본 상호작용 값
public:
	const _float Get_Weight();		//물체 무게
	void Set_Weight(_float fWeight);

	virtual _bool Check_RayCollision() override;
	virtual _bool Check_RayCollision_By_CollisionSphere() override;
	
	//죽음
	virtual _vec3 Get_Reflection(_vec3 vDir, _vec3 vPos) override;

	virtual HRESULT Interaction(Engine::CGameObject* pTarget) PURE;


public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;
	virtual void Free(void);

protected:
	Engine::CStaticMesh* m_pMeshCom = nullptr;

};


#endif // !__STATIC_OBJECT_H__