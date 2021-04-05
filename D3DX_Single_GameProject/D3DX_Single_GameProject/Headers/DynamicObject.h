#pragma once

//���� ��. AI�� ������ ���̳��� ������Ʈ��
#ifndef __DYNAMIC_OBJECT_H__
#define __DYNAMIC_OBJECT_H__

#include "GameObject.h"

BEGIN_NAMESPACE(Engine)
class CDynamicMesh;
class CTransform;
class CControlSupportUnit;
class CSphereCollider;
END_NAMESPACE

enum class eAlign
{
	LEFT, RIGHT,  END
};

class CDynamicObject abstract : public Engine::CGameObject
{
protected:
	explicit CDynamicObject(_Device pDevice);
	explicit CDynamicObject(const CDynamicObject& other);
	virtual ~CDynamicObject() = default;

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) PURE;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) PURE;
	virtual _int Update_GameObject(const _float& fDeltaTime) PURE;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) PURE;
	virtual HRESULT Render_GameObject(void) PURE;

public:
	virtual void Set_Position(_vec3 vPos) PURE;
	virtual void Set_Size(_vec3 vSize) PURE;
	virtual _vec3 Get_Position() PURE;
	virtual _vec3 Get_Size() PURE;

//�⺻ �׼ǵ� - AI���� ����
public:
	virtual void Go_Stright(_float fDeltaTime) PURE;				//����
	virtual void Go_Side(_float fDeltaTime, eAlign pAlign) PURE;	//���̵� �̵�
	virtual void Do_Run(_float fDeltaTime) PURE;				//�޸�
	virtual void Do_Walk(_float fDeltaTime) PURE;				//����
	virtual void Do_Rotate(_float fDeltaTime, eAlign pAlign) PURE;				//ȸ��
	virtual void Do_Attack(_float fDeltaTime) PURE;				//����
	virtual void Do_Idle(_float fDeltaTime) PURE;				//��� + ����
	virtual void Do_Spawn(_float fDeltaTime) PURE;				//���� (���� ����)
	virtual void Do_Dead(_float fDeltaTime) PURE;				//���
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
	_uint Get_Damage();
	_uint Get_NowHP();
	bool Hit_Attack(_uint iDamage);

//�ν� ���� ����
public:
	const _float Get_RecogRange();
	const _float Get_MoveRange();
	const _float Get_AttackRange();
	const _float Get_CollideRange();
	const _vec3 Get_CorePos();
	const Engine::CTransform* Get_Transform();

public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;

	virtual void Free(void);

protected:
	//������
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
	_float m_fHitboxSize;

	_float m_fTime;				//��Ÿ Ÿ�� ������

//������Ʈ��
protected:
	Engine::COLIDETYPE eType;
	Engine::CDynamicMesh* m_pMeshCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CControlSupportUnit* m_pSupportCom = nullptr;
	Engine::CSphereCollider* m_pColliderCom = nullptr;
	//Engine::CSphereCollider* m_pAttackColliderCom = nullptr;

};


#endif // !__DYNAMIC_OBJECT_H__