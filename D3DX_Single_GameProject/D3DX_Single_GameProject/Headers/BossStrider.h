#pragma once
#ifndef __ENEMY_STRIDER_BOSS_H__
#define __ENEMY_STRIDER_BOSS_H__

#include "DynamicObject.h"

BEGIN_NAMESPACE(Engine)
class CDynamicMesh;
class CTransform;
class CControlSupportUnit;
class CSphereCollider;
END_NAMESPACE

enum class eStriderAction
{
	Surprise_wait, Surprise, Stomp, Stand, Sleep, RagDoll, idle_low, idle_high, hitR, fastwalkW, fastwalkS, fastwalkN, fastwalkE, Crouch, END
};
//���� :
//A : ������ ���� �����				- �켱���� ���� ����
//B : �Ͼ�� ���� �Ѿ� 3�� (�÷���Ʈ 3��)
//C : �ɾƼ� ���� �Ѿ� 3 x 3�� (�޽� 9��)
//D : ���� ������ ��ȯ
//E : ���ݱ� ȣ�� - ȣ��� Stomp ��� 3�� ȣ����. - ȣ�� �� 1~4 �ٽ� �ݺ�
enum class eStriderPatton
{
	Idle, Dodge, PattonA, PattonB, PattonC, PattonD, PattonE, END
};

class CBossStrider : public CDynamicObject
{
private:
	explicit CBossStrider(_Device pDevice);
	explicit CBossStrider(const CBossStrider& other);
	virtual ~CBossStrider() = default;

public:
	// CDynamicObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

public:
	// CDynamicObject��(��) ���� ��ӵ�
	virtual void Go_Stright(_float fDeltaTime) override;
	virtual void Go_Side(_float fDeltaTime, eAlign pAlign) override;
	virtual void Do_Run(_float fDeltaTime) override;
	virtual void Do_Walk(_float fDeltaTime) override;
	virtual void Do_Rotate(_float fDeltaTime, eAlign pAlign) override;
	virtual void Do_Attack(_float fDeltaTime, _uint iPatton) override;
	virtual void Do_Idle(_float fDeltaTime) override;
	virtual void Do_Spawn(_float fDeltaTime) override;
	virtual void Do_Dead(_float fDeltaTime) override;
	virtual _bool Do_Dodge(_float fDeltatime) override;

public:
	virtual void Set_Animation(_uint iIndex) override;
	virtual _bool End_Animation_State_Force() override;
	virtual _uint Get_Patton() override;
	void Set_TargetPosition(_vec3 vPos);

private:
	void PattonA();
	void PattonB();
	void PattonC();
	void PattonD();
	void PattonE();

private:
	HRESULT Add_Component();
	virtual void Free() override;

public:
	static CBossStrider* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	eStriderAction m_eAction;
	eStriderAction m_ePrevAction;
	eStriderPatton m_ePatton;

	_bool m_bStand;				//Stand True : High / Stand False : Low

	_vec3 m_vOriPos;
	_vec3 m_vCalibrationPos;		//�޽� ��ǥ ��Ʋ������ ���� ��ġ ����

	_vec3 m_vTargetPos;				//Ÿ�� ������
	_vec3 m_vShootPos;				//�߻� ��ġ

	//�⺻ ���� ����
	_float m_fPattonInterval;		//���Ϻ� ����
	_float m_fPattonCooltime;		//���� ����ð�

	_bool m_bAttackLock;			//���� ���� ���
	_bool m_bPattonLock;			//���� ��� (��Ÿ��)

	//���� A
	_float m_fPattonAInterval;		//���� A ����
	_float m_fPattonACooltime;		//���� A ����ð�



	//3���߿�
	_float m_fTripleShootInterval;	//3���� ��� ��Ÿ�� - 3���� �� ����
	_float m_fTripleShootCooldownInterval;	//3���� ��� ��Ÿ�� - 3���� �� ����	
	_float m_fTripleShootTime;		//3���� ��� ��Ÿ�� - ���� �ð�

	_uint m_iTripleShootCount = 0;
	_uint m_iTripleShootGroupCount = 0;

	_bool m_bShootLock;				//���� ��� ��Ÿ��	- 3���� �� ����
	_bool m_bTripleShootLock;		//3���� ��� ��Ÿ�� - 3���� �� ����

	static	_uint m_iPattonBShoot;		//�����
	static	_uint m_iPattonCShoot;		//�÷���Ʈ
	static	_uint m_iPattonDShoot;		//����


};


#endif // !__ENEMY_STRIDER_BOSS_H__