#pragma once

#ifndef __BASE_AI_H__
#define __BASE_AI_H__

//적들 AI

#include "Base.h"
#include "GameObject.h"
#include "DynamicObject.h"

//Nospawn : 미등장
//Spawn : 등장
//Ready : 준비상태 (컷씬용. 다른 AI 기능 작동안시킴)
//Idle : 등장 모션 후 대기
//Recog : 적 포착
//Track : 적 추적
//Attack : 적 공격
//Dodge : 회피
//Dead : 주금
//Disappear : 사라짐 (주금 이후)
enum class eAIStatus
{
	NOSPAWN,SPAWN, READY,IDLE, RECOG, TRACK, ATTACK, DODGE, DEAD,DISAPPEAR, END
};

class CBaseAI : public Engine::CGameObject
{
protected:
	explicit CBaseAI(_Device pDevice);
	explicit CBaseAI(const CBaseAI& other);
	virtual ~CBaseAI() = default;

public:
	virtual HRESULT Ready_GameObject() PURE;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) PURE;
	virtual _int Update_GameObject(const _float& fDeltaTime) PURE;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) PURE;
	virtual HRESULT Render_GameObject(void) PURE;

public:
// CGameObject을(를) 통해 상속됨... 근데 안써요.
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

public:
	eAIStatus Get_State();
	void Set_State(eAIStatus eState);

public:
	HRESULT Set_ControlUnit(CDynamicObject* pUnit);
	HRESULT Set_Target(Engine::CGameObject* pTarget);

	void Set_StartSpawn();

//내부 판단 함수들
protected:
	virtual HRESULT Do_Spawn(const _float& fDeltaTime) PURE;
	virtual HRESULT Do_Appear(const _float& fDeltaTime) PURE;
	virtual HRESULT Do_Idle(const _float& fDeltaTime) PURE;
	virtual HRESULT Do_Movement(const _float& fDeltaTime) PURE;
	virtual HRESULT Do_Attack(const _float& fDeltaTime) PURE;

	_bool Check_HP_Change();

public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;

	virtual void Free() override;

protected:
			CDynamicObject*	m_pControlUnit;		//AI가 조작할 타겟
	Engine::CGameObject*	m_pTargetUnit;		//AI가 노려볼 타겟

//플레이어 사거리는 일단 5.f 로 계산해서 사용할것.
protected:
	_bool m_bStartSpawn;	//등장 트리거 (이거 전까지는 아무것도 안함) <- Ready 기능 아냐?
	_bool m_bReady;			//대기모드 여부
	_bool m_bSpawn;			//등장했는가
	_bool m_bAppear;		//움직이는가
	_bool m_bTrack;			//추적중인가
	_bool m_bAttack;		//공격중인가
	_bool m_bDodge;			//회피중인가
	_bool m_bDodgeLock;			//회피중인가

	_float m_fRangeToTarget;	//타겟까지의 거리
	_float m_fDodgeTime;		//무적시간 - 흐르는것
	_float m_fInvinTime;		//무적시간
	_float m_fDodgeCoolTime;	//회피 쿨타임
	_float m_fDodgeCountTime;	//회피 쿨타임 - 흐르는것

	int m_iHPState;				//이전 HP상태 (상태비교용)

	eBasePatton m_eBasePatton;	//패턴 검사용
	eAIStatus	m_eState;		//AI 현재 상태 검사 (리스폰 등에 사용)

};

#endif