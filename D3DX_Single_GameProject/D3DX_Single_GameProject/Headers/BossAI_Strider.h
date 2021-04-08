#pragma once

#ifndef __AI_BOSS_STRIDER_H__
#define __AI_BOSS_STRIDER_H__

#include "BaseAI.h"

//보스 : 스트라이더 AI
class CBossAI_Strider : public CBaseAI
{
private:
	explicit CBossAI_Strider(_Device pDevice);
	explicit CBossAI_Strider(const CBossAI_Strider& other);
	virtual ~CBossAI_Strider() = default;


public:
	// CBaseAI을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;


public:
	// CBaseAI을(를) 통해 상속됨
	virtual HRESULT Do_Spawn(const _float& fDeltaTime) override;
	virtual HRESULT Do_Appear(const _float& fDeltaTime) override;
	virtual HRESULT Do_Idle(const _float& fDeltaTime) override;
	virtual HRESULT Do_Movement(const _float& fDeltaTime) override;
	virtual HRESULT Do_Attack(const _float& fDeltaTime) override;

public:
	static CBossAI_Strider* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;


//protected:
//	CDynamicObject* m_pControlUnit;		//AI가 조작할 타겟
//	Engine::CGameObject* m_pTargetUnit;		//AI가 노려볼 타겟
//
////플레이어 사거리는 일단 5.f 로 계산해서 사용할것.
//protected:
//	_bool m_bReady;			//대기모드 여부
//	_bool m_bSpawn;			//등장했는가
//	_bool m_bAppear;		//움직이는가
//	_bool m_bTrack;			//추적중인가
//	_bool m_bAttack;		//공격중인가
//	_bool m_bDodge;			//회피중인가
//	_bool m_bDodgeLock;			//회피중인가
//
//	_float m_fRangeToTarget;	//타겟까지의 거리
//	_float m_fDodgeTime;		//무적시간 - 흐르는것
//	_float m_fInvinTime;		//무적시간
//	_float m_fDodgeCoolTime;	//회피 쿨타임
//	_float m_fDodgeCountTime;	//회피 쿨타임 - 흐르는것
//
//	int m_iHPState;				//이전 HP상태 (상태비교용)
//
//	eBasePatton m_eBasePatton;	//패턴 검사용


};


#endif // !__AI_BOSS_STRIDER_H__