#pragma once

#ifndef __BASE_OBJECT_H__
#define __BASE_OBJECT_H__

#include "GameObject.h"
BEGIN_NAMESPACE(Engine)
class CTransform;
class CControlSupportUnit;
class CSphereCollider;
class CShader;
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
//�߷°� ����
enum class eForceState
{
	NONE, GRAP, PULL, END
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
	virtual void Set_Direction(_vec3 vDir);
	const _float Get_Radius();
			void Set_ObjectType(eForceType eType);		
			void Set_SpeedLockState(_bool bLock);
			void Set_Rotation(Engine::ROTATION eRotate, float fRadian);

	virtual _vec3 Get_Position();
	virtual _vec3 Get_Size();
			_vec3 Get_Direction();
			const eForceType Get_ObjectType();
			_bool Get_SpeedLockState();
			_uint Get_Damage();

//��ȣ�ۿ� 2
public:
	Engine::CControlSupportUnit* Get_SupportUnit();

	virtual _bool Check_RayCollision() PURE;
	virtual _bool Check_RayCollision_By_CollisionSphere() PURE;
	const Engine::CTransform* Get_Transform();

	//���� 0¥�� �߻� �� �Ҹ�Ÿ��
	const _float Get_LifeTime();	//0 ���� : ������
	void Set_LifeTime(_float fTime);
	void Add_LifeTime(_float fTime);

	//�߻� �� �ӵ�
	const _float Get_Speed();
	virtual void Set_Speed(_float fSpeed);

	//�߷� ���� ����
	void Set_GravitionPower(_float fGravition);
	void Set_ClearGSpeed(_float fClearHeight = 0.f);

	virtual HRESULT Interaction(Engine::CGameObject* pTarget) PURE;

	//�浹 �ݻ簢
	virtual _vec3 Get_Reflection(_vec3 vDir, _vec3 vPos) PURE;
	eForceState Get_ForceState();
	void Set_ForceState(eForceState eState);

protected :
	virtual HRESULT Setup_ConstantTable(LPD3DXEFFECT& pEffect, _bool bDissolve = false) PURE;

protected:
	virtual void Free(void);

	_float m_fWeight;
	_float m_fLifeTime;
	_float m_fSpeed;			//����ӵ� (or �߻� ��)
	_float m_fGravitionSpeed;	//�߷� ������ ���� �ӵ�
	_float m_fGravitionPower;	//�߷°�	
	_bool m_bSpeedLock;			//�ӵ��� ��ȭ ���� ����

	_vec3 m_vStartPos;
	_vec3 m_vDirection;

	_float m_fHitboxSize;		//=fRadius

	//�߷°� ��ȣ�ۿ� ����
	_vec3 m_vImForceDirection;	//�ܺ� �� �־����� ����
	_float m_fImForcePower;		//�ܺ� ��
	eForceType m_eForceType;	//�߷°� ���� ����
	eForceState m_eForceState;	//�߷°� ������ ����

	_uint m_iDamage;			//���ݷ� (�浹 ����)
	_bool m_bAttackHitEnable;	//������ ��ȿ�Ѱ� (�Ѵ� ������ �ٷ� ��ȿ ������)



	Engine::COLIDETYPE eType;

	Engine::CControlSupportUnit* m_pSupportCom = nullptr;
	Engine::CSphereCollider* m_pColliderCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CShader* m_pShaderCom = nullptr;			//���̴�

	Engine::CManagement* m_pManagement = nullptr;
	

};


#endif // !__BASE_OBJECT_H__