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

//Death - 사망모션 (이러고 푹 쓰러짐)
//Dodge_S, Dodge_N, Dodge_W, Dodge_E - 회피 모션
//Drop_Down - 등장 모션
//RagDoll - 기본 모션
//Hunter_Angry - 플레이어 인식시 모션
//Walk_S, Walk_N - 기본 이동 모션
//Idle - 기본 대기 모션
//Melee_L, Melee_R - 근접 공격
//RangeAttack - 원거리 공격 1 : "플레쉐트" 발사
//Shoot_Minigun - 원거리 공격 2  : "펄스 기관총" 발사

enum class eHunterAction
{
	Death, Dodge_S, Dodge_N, Dodge_W, Dodge_E, Drop_Down, RagDoll, Hunter_Angry, Walk_S, Walk_N, Idle, Melee_L, Melee_R, RangeAttack, Shoot_Minigun, END
};

//IDLE : 아무 행동도 안함
//Dodge : 공격 회피 (보통 IDLE 상태에서 발동)
//PattonA : 플레이어 근접시 근접 공격
//PattonB : 플레이어에게 플레쉐트 발사
//PattonC : 멈춰서서 플레이어에게 펄스 기관총 발사

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
	// CDynamicObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

public:
	virtual void Go_Stright(_float fDeltaTime) override;
	virtual void Go_Side(_float fDeltaTime, eAlign pAlign) override;
	virtual void Do_Run(_float fDeltaTime) override;
	virtual void Do_Walk(_float fDeltaTime) override;
	virtual void Do_Rotate(_float fDeltaTime, eAlign pAlign) override;
	virtual void Do_Attack(_float fDeltaTime, _uint iPatton = 0) override;
	virtual void Do_Idle(_float fDeltaTime) override;
	virtual void Do_Spawn(_float fDeltaTime) override;
	virtual void Do_Dead(_float fDeltaTime) override;
	virtual _bool Do_Dodge(_float fDeltatime) override;

public:
	// CDynamicObject을(를) 통해 상속됨
	virtual void Set_Animation(_uint iIndex) override;
	virtual _bool End_Animation_State_Force() override;
	virtual _uint Get_Patton() override;

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
	eHunterAction m_eAction;
	eHunterAction m_ePrevAction;

	eHunterPatton m_ePatton;
	_vec3 m_vOriPos;

	_float m_fPattonInterval;		//패턴별 간격
	_float m_fPattonCooltime;		//패턴 진행시간


	_bool m_bAttackLock;			//공격 간격 잠금
	_bool m_bPattonLock;			//패턴 잠금 (쿨타임)
	
	_bool m_bShootLock;				//연속 사격 쿨타임

static	_uint m_iPattonAShoot;		//기관총
static	_uint m_iPattonBShoot;		//플레쉐트

	// CDynamicObject을(를) 통해 상속됨





};


#endif // !__ENEMY_HUNTER_NORMAL_H__