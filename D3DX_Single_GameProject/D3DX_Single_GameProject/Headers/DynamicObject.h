#pragma once

//몬스터 등. AI를 따르는 다이나믹 오브젝트들
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

//기본 액션들 - AI에서 쓸것
public:
	virtual void Go_Stright(_float fDeltaTime) PURE;				//전진
	virtual void Go_Side(_float fDeltaTime, eAlign pAlign) PURE;	//사이드 이동
	virtual void Do_Run(_float fDeltaTime) PURE;				//달림
	virtual void Do_Walk(_float fDeltaTime) PURE;				//걸음
	virtual void Do_Rotate(_float fDeltaTime, eAlign pAlign) PURE;				//회전
	virtual void Do_Attack(_float fDeltaTime, _uint iPatton = 0) PURE;				//공격
	virtual void Do_Idle(_float fDeltaTime) PURE;				//대기 + 멈춤
	virtual void Do_Spawn(_float fDeltaTime) PURE;				//등장 (생략 가능)
	virtual void Do_Dead(_float fDeltaTime) PURE;				//사망
	virtual _bool Do_Dodge(_float fDeltatime)PURE;				//회피
	void Do_Anichange(_uint iAnimation);
	void Check_Hit(_bool bForce, _uint iDamage);				//일단 어택 콜라이더 빼고 작업...
	//virtual void Do_Tracking() PURE;			//적 추적

//외부 애니메이션 조작용
public:
	virtual void Set_Animation(_uint iIndex) PURE;
	virtual _bool End_Animation_State_Force() PURE;
	void Force_Update_Animation();
	_bool End_Animation_State();

//공통 상태 함수
public:
	virtual _uint Get_Patton() PURE;
	_uint Get_Damage();
	_uint Get_NowHP();
	bool Hit_Attack(_uint iDamage);

//인식 범위 관련
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
	//움직임
	_bool m_bStartMove;			//움직이기 시작하는지 (스폰했는지 여부)

	_float m_fMoveSpeed;
	_float m_fAnimationSpeed;
	_float m_fRotate;
	_float m_fRotateSpeed;
	_vec3  m_vCorePos;			//메쉬 좌표 기준점
	_bool  m_bEndChecker;		//애니메이션 종료 체크

	_int m_iHP;					//현재 체력
	_uint m_iMaxHP;				//최대 체력
	_uint m_iDamage;			//공격력

	//기본 공격 간격
	_float m_fAttackInterval;	//공격간격은 어떻게 되는가
	_float m_fNowAttackTime;	//현재까지 얼마나 공격중인가
	_bool m_bAttackHitEnable;	//공격이 유효한가 (한대 때리면 바로 유효 빼버림)

	//인식 범위 - 얘들은 적 타입별로 동일함. Setter 필요없음
	_float m_fRecognizeRange;	//인식 범위 (인식 시 방향을 틈 or 리젠)
	_float m_fMoveRange;		//이동 범위 (공격을 위해 다가오는 거리)
	_float m_fAttackRange;		//공격 범위 (공격 사거리)

	_float m_fTime;				//델타 타임 보관용

//컴포넌트들
protected:
	Engine::CDynamicMesh* m_pMeshCom = nullptr;
	//Engine::CTransform* m_pTransformCom = nullptr;
	//Engine::CControlSupportUnit* m_pSupportCom = nullptr;
	//Engine::CSphereCollider* m_pColliderCom = nullptr;
	//Engine::CSphereCollider* m_pAttackColliderCom = nullptr;

};


#endif // !__DYNAMIC_OBJECT_H__