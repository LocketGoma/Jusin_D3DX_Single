#pragma once

#ifndef __BASE_AI_H__
#define __BASE_AI_H__

//적들 AI

#include "Base.h"
#include "GameObject.h"
#include "DynamicObject.h"

class CBaseAI : public Engine::CGameObject
{
private:
	explicit CBaseAI(_Device pDevice);
	explicit CBaseAI(const CBaseAI& other);
	virtual ~CBaseAI() = default;

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

public:
// CGameObject을(를) 통해 상속됨... 근데 안써요.
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

public:
	HRESULT Set_ControlUnit(CDynamicObject* pUnit);
	HRESULT Set_Target(Engine::CGameObject* pTarget);

//내부 판단 함수들
private:
	HRESULT Do_Spawn(const _float& fDeltaTime);
	HRESULT Do_Appear(const _float& fDeltaTime);
	HRESULT Do_Idle(const _float& fDeltaTime);
	HRESULT Do_Movement(const _float& fDeltaTime);
	HRESULT Do_Attack(const _float& fDeltaTime);

	_bool Check_HP_Change();

public:
	static CBaseAI* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

	virtual void Free() override;

private:
			CDynamicObject*	m_pControlUnit;		//AI가 조작할 타겟
	Engine::CGameObject*	m_pTargetUnit;		//AI가 노려볼 타겟

//플레이어 사거리는 일단 5.f 로 계산해서 사용할것.
private:
	_bool m_bReady;			//대기모드 여부
	_bool m_bSpawn;			//등장했는가
	_bool m_bAppear;		//움직이는가
	_bool m_bTrack;			//추적중인가
	_bool m_bAttack;		//공격중인가
	_bool m_bDodge;			//회피중인가

	_float m_fRangeToTarget;	//타겟까지의 거리
	_float m_fDodgeTime;		//무적시간 - 흐르는것
	_float m_fInvinTime;		//무적시간

	int m_iHPState;				//이전 HP상태 (상태비교용)


};

#endif