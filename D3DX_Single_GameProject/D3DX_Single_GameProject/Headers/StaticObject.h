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
// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) PURE;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) PURE;
	virtual _int Update_GameObject(const _float& fDeltaTime) PURE;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) PURE;
	virtual HRESULT Render_GameObject(void) PURE;

//�⺻ ��ȣ�ۿ� ��
public:
	const _uint Get_Weight();		//��ü ����
	void Set_Weight(_uint iWeight);

	virtual _bool Check_RayCollision() override;
	virtual _bool Check_RayCollision_By_CollisionSphere() override;


public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;
	virtual void Free(void);

protected:
	Engine::CStaticMesh* m_pMeshCom = nullptr;
};


#endif // !__STATIC_OBJECT_H__