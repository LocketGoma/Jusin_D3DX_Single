#pragma once

#ifndef __ENEMY_HUNTER_NORMAL_H__
#define __ENEMY_HUNTER_NORMAL_H__

#include "DynamicObject.h"

BEGIN_NAMESPACE(Engine)
class CDynamicMesh;
class CTransform;
class CControlSupportUnit;
class CSphereCollider;
END_NAMESPACE

//Death - ������ (�̷��� ǫ ������)
//Dodge_S, Dodge_N, Dodge_W, Dodge_E - ȸ�� ���
//Drop_Down - ���� ���
//RagDoll - �⺻ ���
//Hunter_Angry - �÷��̾� �νĽ� ���
//Walk_S, Walk_N - �⺻ �̵� ���
//Idle - �⺻ ��� ���
//Melee_L, Melee_R - ���� ����
//RangeAttack - ���Ÿ� ���� 1 : "�÷���Ʈ" �߻�
//Shoot_Minigun - ���Ÿ� ���� 2  : "�޽� �����" �߻�

enum class eHunterAction
{
	Death, Dodge_S, Dodge_N, Dodge_W, Dodge_E, Drop_Down, RagDoll, Hunter_Angry, Walk_S, Walk_N, Idle, Melee_L, Melee_R, RangeAttack, Shoot_Minigun, END
};

//IDLE : �ƹ� �ൿ�� ����
//Dodge : ���� ȸ�� (���� IDLE ���¿��� �ߵ�)
//PattonA : �÷��̾� ������ ���� ����
//PattonB : �÷��̾�� �÷���Ʈ �߻�
//PattonC : ���缭�� �÷��̾�� �޽� ����� �߻�

enum class eHunterPatton
{
	Idle,Dodge,PattonA,PattonB,PattonC,END
};


class CEnemyHunter :  public CDynamicObject
{
protected:
	explicit CEnemyHunter(_Device pDevice);
	explicit CEnemyHunter(const CEnemyHunter& other);
	virtual ~CEnemyHunter() = default;

public:
	// CDynamicObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

public:
	virtual void Go_Stright(_float fDeltaTime) override;
	virtual void Go_Side(_float fDeltaTime, eAlign pAlign) override;
	virtual void Do_Run(_float fDeltaTime) override;
	virtual void Do_Walk(_float fDeltaTime) override;
	virtual void Do_Rotate(_float fDeltaTime, eAlign pAlign) override;
	virtual void Do_Attack(_float fDeltaTime) override;
	virtual void Do_Idle(_float fDeltaTime) override;
	virtual void Do_Spawn(_float fDeltaTime) override;
	virtual void Do_Dead(_float fDeltaTime) override;

public:
	virtual void Set_Animation(_uint iIndex) override;
	virtual _bool End_Animation_State_Force() override;


private:
	void PattonA();
	void PattonB();
	void PattonC();	

private:
	HRESULT Add_Component();
	virtual void Free() override;


public:
	static CEnemyHunter* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	eHunterAction eAction;
	eHunterAction ePrevAction;

	eHunterPatton ePatton;
	_vec3 vOriPos;


};


#endif // !__ENEMY_HUNTER_NORMAL_H__