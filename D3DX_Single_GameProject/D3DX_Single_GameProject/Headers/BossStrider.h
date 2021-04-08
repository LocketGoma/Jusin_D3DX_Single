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
//패턴 :
//A : 근접시 근접 충격파				- 우선순위 제일 높음
//B : 일어서서 느린 총알 3발 (플레쉐트 3발)
//C : 앉아서 빠른 총알 3 x 3발 (펄스 9발)
//D : 맨핵 여러개 소환
//E : 폭격기 호출 - 호출시 Stomp 모션 3번 호출함. - 호출 후 1~4 다시 반복
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
	// CDynamicObject을(를) 통해 상속됨
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
	// CDynamicObject을(를) 통해 상속됨
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
	_vec3 m_vCalibrationPos;		//메쉬 좌표 뒤틀림으로 인한 위치 보정

	_vec3 m_vTargetPos;				//타겟 포지션
	_vec3 m_vShootPos;				//발사 위치

	//기본 패턴 변수
	_float m_fPattonInterval;		//패턴별 간격
	_float m_fPattonCooltime;		//패턴 진행시간

	_bool m_bAttackLock;			//공격 간격 잠금
	_bool m_bPattonLock;			//패턴 잠금 (쿨타임)

	//패턴 A
	_float m_fPattonAInterval;		//패턴 A 간격
	_float m_fPattonACooltime;		//패턴 A 진행시간



	//3연발용
	_float m_fTripleShootInterval;	//3연속 사격 쿨타임 - 3연발 내 간격
	_float m_fTripleShootCooldownInterval;	//3연속 사격 쿨타임 - 3연발 간 간격	
	_float m_fTripleShootTime;		//3연속 사격 쿨타임 - 현재 시간

	_uint m_iTripleShootCount = 0;
	_uint m_iTripleShootGroupCount = 0;

	_bool m_bShootLock;				//연속 사격 쿨타임	- 3연발 간 간격
	_bool m_bTripleShootLock;		//3연속 사격 쿨타임 - 3연발 내 간격

	static	_uint m_iPattonBShoot;		//기관총
	static	_uint m_iPattonCShoot;		//플레쉐트
	static	_uint m_iPattonDShoot;		//맨핵


};


#endif // !__ENEMY_STRIDER_BOSS_H__