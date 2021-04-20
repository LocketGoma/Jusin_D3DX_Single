#pragma once

//몬스터 등. AI를 따르는 다이나믹 오브젝트들
#ifndef __DYNAMIC_OBJECT_H__
#define __DYNAMIC_OBJECT_H__

#include "BaseObject.h"

BEGIN_NAMESPACE(Engine)
class CDynamicMesh;
END_NAMESPACE

class CBaseEffect;

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

public:
	virtual _int Update_GameObject(const _float& fDeltaTime) PURE;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) PURE;

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
	
	_uint Get_NowHP();
	bool Hit_Attack(_uint iDamage);

	bool Get_Clear_Dead_State();

	//쓸일없음 버림.
	virtual HRESULT Interaction(Engine::CGameObject* pTarget) override;

//인식 범위 관련
public:
	void Set_StartMove();
	const _float Get_RecogRange();
	const _float Get_MoveRange();
	const _float Get_AttackRange();
	const _float Get_CollideRange();
	const _vec3 Get_CorePos();

	virtual _bool Check_RayCollision() override;
			_bool Check_Attack_Collide(const _vec3* pSourcePos, const _float fSourceRadius);
	virtual _bool Check_RayCollision_By_CollisionSphere() override;

	_bool Get_State();

//중력건 관련
public:
	virtual void Set_Speed(_float fSpeed) override;
	virtual void Set_Direction(_vec3 vDir) override;
	virtual _vec3 Get_Reflection(_vec3 vDir, _vec3 vPos) override;
	_bool Get_RayPick(_vec3 vDir, _vec3 vPos);

public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;

	virtual void Free(void);

protected:
	virtual HRESULT Setup_ConstantTable(LPD3DXEFFECT& pEffect, _bool bDissolve = false) override;

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

	_bool m_bHurt;				//피해 입었는지 여부 (이펙트 출력용)
	_bool m_bUseBaseEffect;		//기본 이펙트 사용 여부

	//기본 공격 간격
	_float m_fAttackInterval;	//공격간격은 어떻게 되는가
	_float m_fNowAttackTime;	//현재까지 얼마나 공격중인가

	//인식 범위 - 얘들은 적 타입별로 동일함. Setter 필요없음
	_float m_fRecognizeRange;	//인식 범위 (인식 시 방향을 틈 or 리젠)
	_float m_fMoveRange;		//이동 범위 (공격을 위해 다가오는 거리)
	_float m_fAttackRange;		//공격 범위 (공격 사거리)

	_float m_fTime;				//델타 타임 보관용

	_bool m_bState;

	Engine::SOUND_CHANNELID m_eChannel;	//사운드 채널 (다중 몬스터 처리시)

	_bool m_bClearDead;			//진짜 죽은건지 파악 (이거 인식되면 SetDead)
	_bool m_bDeadTrigger;		//사망 이펙트용

	_float m_fDeadTime;			//사망시간 돌리기

//컴포넌트들
protected:
	Engine::CTexture* m_pDesolveTextureCom = nullptr;	//디졸브

	Engine::CDynamicMesh* m_pMeshCom = nullptr;
	CBaseEffect* m_pEffect = nullptr;			//피튀는 이펙트
	//Engine::CTransform* m_pTransformCom = nullptr;
	//Engine::CControlSupportUnit* m_pSupportCom = nullptr;
	//Engine::CSphereCollider* m_pColliderCom = nullptr;
	//Engine::CSphereCollider* m_pAttackColliderCom = nullptr;

};


#endif // !__DYNAMIC_OBJECT_H__