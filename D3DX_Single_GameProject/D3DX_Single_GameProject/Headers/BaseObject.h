#pragma once

#ifndef __BASE_OBJECT_H__
#define __BASE_OBJECT_H__

#include "GameObject.h"
BEGIN_NAMESPACE(Engine)
class CTransform;
class CControlSupportUnit;
class CSphereCollider;
END_NAMESPACE

//����ƽ ������Ʈ, ���̳��� ������Ʈ �θ�
//�߷°�, ��Ʈ�� ����Ʈ ���� ó����

//�߷°ǿ� ������ �޴� �ֵ�

//NONE : ��ȣ�ۿ� �Ұ� (�Ϲ� ź�� ��)
//PUSH : ��ġ�� ���� (�Ŵ� ������Ʈ ��)
//PULL : ���� ���� (����ź, ����/�߰�ũ�� ������Ʈ ��) ( = PUSH ����)
enum class eForceType
{
	NONE, PUSH, PULL, END
};
class CBaseObject abstract: public Engine::CGameObject
{
protected:
	explicit CBaseObject(_Device pDevice);
	explicit CBaseObject(const CBaseObject& other);
	virtual ~CBaseObject() = default;


public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) PURE;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) PURE;
	virtual _int Update_GameObject(const _float& fDeltaTime) PURE;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) PURE;
	virtual HRESULT Render_GameObject(void) PURE;

//��ȣ�ۿ�
public:
	virtual void Set_Position(_vec3 vPos);
	virtual void Set_Size(_vec3 vSize);
			void Set_Direction(_vec3 vDir);
			void Set_ObjectType(eForceType eType);			
	virtual _vec3 Get_Position();
	virtual _vec3 Get_Size();
			_vec3 Get_Direction();
			const eForceType Get_ObjectType();

//��ȣ�ۿ� 2
public:
	Engine::CControlSupportUnit* Get_SupportUnit();

	virtual _bool Check_RayCollision() PURE;
	virtual _bool Check_RayCollision_By_CollisionSphere() PURE;
	const Engine::CTransform* Get_Transform();

	//���� 0¥�� �߻� �� �Ҹ�Ÿ��
	const _float Get_LifeTime();	//0 ���� : ������
	void Set_LifeTime(_float fTime);

	//�߻� �� �ӵ�
	const _float Get_Speed();
	void Set_Speed(_float fSpeed);

protected:
	virtual void Free(void);

	_uint m_iWeight;
	_float m_fLifeTime;
	_float m_fSpeed;			//����ӵ� (or �߻� ��)
	eForceType m_eForceType;

	_vec3 m_vStartPos;
	_vec3 m_vDirection;

	_float m_fHitboxSize;		//=fRadius

	Engine::COLIDETYPE eType;

	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CControlSupportUnit* m_pSupportCom = nullptr;
	Engine::CSphereCollider* m_pColliderCom = nullptr;

};


#endif // !__BASE_OBJECT_H__