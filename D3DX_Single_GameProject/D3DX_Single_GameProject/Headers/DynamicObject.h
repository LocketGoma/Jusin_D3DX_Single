#pragma once

//���� ��. AI�� ������ ���̳��� ������Ʈ��
#ifndef __DYNAMIC_OBJECT_H__
#define __DYNAMIC_OBJECT_H__

#include "BaseObject.h"

BEGIN_NAMESPACE(Engine)
class CDynamicMesh;
END_NAMESPACE

enum class eAlign
{
	LEFT, RIGHT, DOWN, UP, IDLE, END
};

enum class eBasePatton
{
	Idle, Dodge, PattonA, PattonB, PattonC, PattonD, PattonE, END
};

class CDynamicObject abstract : public CBaseObject
{
protected:
	explicit CDynamicObject(_Device pDevice);
	explicit CDynamicObject(const CDynamicObject& other);
	virtual ~CDynamicObject() = default;


public:
	_vec3 Calc_Position_At_Mesh();
	_vec3 Get_Position_At_Mesh();

//�⺻ �׼ǵ� - AI���� ����
public:
	virtual void Go_Stright(_float fDeltaTime) PURE;				//����
	virtual void Go_Side(_float fDeltaTime, eAlign pAlign) PURE;	//���̵� �̵�
	virtual void Do_Run(_float fDeltaTime) PURE;				//�޸�
	virtual void Do_Walk(_float fDeltaTime) PURE;				//����
	virtual void Do_Rotate(_float fDeltaTime, eAlign pAlign) PURE;				//ȸ��
	virtual void Do_Attack(_float fDeltaTime, _uint iPatton = 0) PURE;				//����
	virtual void Do_Idle(_float fDeltaTime) PURE;				//��� + ����
	virtual void Do_Spawn(_float fDeltaTime) PURE;				//���� (���� ����)
	virtual void Do_Dead(_float fDeltaTime) PURE;				//���
	virtual _bool Do_Dodge(_float fDeltatime)PURE;				//ȸ��
	void Do_Anichange(_uint iAnimation);
	void Check_Hit(_bool bForce, _uint iDamage);				//�ϴ� ���� �ݶ��̴� ���� �۾�...
	//virtual void Do_Tracking() PURE;			//�� ����

//�ܺ� �ִϸ��̼� ���ۿ�
public:
	virtual void Set_Animation(_uint iIndex) PURE;
	virtual _bool End_Animation_State_Force() PURE;
	void Force_Update_Animation();
	_bool End_Animation_State();

//���� ���� �Լ�
public:
	virtual _uint Get_Patton() PURE;
	_uint Get_Damage();
	_uint Get_NowHP();
	bool Hit_Attack(_uint iDamage);

//�ν� ���� ����
public:
	void Set_StartMove();
	const _float Get_RecogRange();
	const _float Get_MoveRange();
	const _float Get_AttackRange();
	const _float Get_CollideRange();
	const _vec3 Get_CorePos();

	virtual _bool Check_RayCollision() override;
	virtual _bool Check_RayCollision_By_CollisionSphere() override;

public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;

	virtual void Free(void);

protected:
	//������
	_bool m_bStartMove;			//�����̱� �����ϴ��� (�����ߴ��� ����)

	_float m_fMoveSpeed;
	_float m_fAnimationSpeed;
	_float m_fRotate;
	_float m_fRotateSpeed;
	_vec3  m_vCorePos;			//�޽� ��ǥ ������
	_bool  m_bEndChecker;		//�ִϸ��̼� ���� üũ

	_int m_iHP;					//���� ü��
	_uint m_iMaxHP;				//�ִ� ü��
	_uint m_iDamage;			//���ݷ�

	//�⺻ ���� ����
	_float m_fAttackInterval;	//���ݰ����� ��� �Ǵ°�
	_float m_fNowAttackTime;	//������� �󸶳� �������ΰ�
	_bool m_bAttackHitEnable;	//������ ��ȿ�Ѱ� (�Ѵ� ������ �ٷ� ��ȿ ������)

	//�ν� ���� - ����� �� Ÿ�Ժ��� ������. Setter �ʿ����
	_float m_fRecognizeRange;	//�ν� ���� (�ν� �� ������ ƴ or ����)
	_float m_fMoveRange;		//�̵� ���� (������ ���� �ٰ����� �Ÿ�)
	_float m_fAttackRange;		//���� ���� (���� ��Ÿ�)

	_float m_fTime;				//��Ÿ Ÿ�� ������

//������Ʈ��
protected:
	Engine::CDynamicMesh* m_pMeshCom = nullptr;
	//Engine::CTransform* m_pTransformCom = nullptr;
	//Engine::CControlSupportUnit* m_pSupportCom = nullptr;
	//Engine::CSphereCollider* m_pColliderCom = nullptr;
	//Engine::CSphereCollider* m_pAttackColliderCom = nullptr;

};


#endif // !__DYNAMIC_OBJECT_H__