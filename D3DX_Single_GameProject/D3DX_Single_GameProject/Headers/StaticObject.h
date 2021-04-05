#pragma once

#ifndef __STATIC_OBJECT_H__
#define __STATIC_OBJECT_H__


#include "GameObject.h"

BEGIN_NAMESPACE(Engine)
class CStaticMesh;
class CTransform;
class CControlSupportUnit;
class CSphereCollider;
END_NAMESPACE

//�߷°ǿ� ������ �޴� �ֵ�

//NONE : ��ȣ�ۿ� �Ұ� (�Ϲ� ź�� ��)
//PUSH : ��ġ�� ���� (�Ŵ� ������Ʈ ��)
//PULL : ���� ���� (����ź, ����/�߰�ũ�� ������Ʈ ��)
enum class eForceType
{
	NONE, PUSH, PULL, END
};

class CStaticObject abstract : public Engine::CGameObject
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

public:
	virtual void Set_Position(_vec3 vPos);
	virtual void Set_Size(_vec3 vSize);
	virtual _vec3 Get_Position();
	virtual _vec3 Get_Size();

//�⺻ ��ȣ�ۿ� ��
public:
	const _uint Get_Weight();		//��ü ����
	void Set_Weight(_uint iWeight);

	const eForceType Get_ObjectType();
	void Set_ObjectType(eForceType eType);

	//���� 0¥�� �߻� �� �Ҹ�Ÿ��
	const _float Get_LifeTime();	//0 ���� : ������
	void Set_LifeTime(_float fTime);

	//�߻� �� �ӵ�
	const _float Get_Speed();
	void Set_Speed(_float fSpeed);

public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;

	virtual void Free(void);

protected:
	_uint m_iWeight;
	_float m_fLifeTime;
	_float m_fSpeed;
	eForceType m_eForceType;

	Engine::CStaticMesh* m_pMeshCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CControlSupportUnit* m_pSupportCom = nullptr;
	Engine::CSphereCollider* m_pColliderCom = nullptr;

	_float m_fHitboxSize;		//=fRadius
};


#endif // !__STATIC_OBJECT_H__