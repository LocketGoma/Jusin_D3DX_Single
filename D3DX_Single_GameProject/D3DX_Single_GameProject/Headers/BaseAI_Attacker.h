#pragma once

#ifndef __AI_BASE_ATTACKER_H__
#define __AI_BASE_ATTACKER_H__

#include "BaseAI.h"

//헤드크랩, 개미귀신 등 단순 근접공격계
class CBaseAI_Attacker : public CBaseAI
{
private:
	explicit CBaseAI_Attacker(_Device pDevice);
	explicit CBaseAI_Attacker(const CBaseAI_Attacker& other);
	virtual ~CBaseAI_Attacker() = default;

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
	static CBaseAI_Attacker* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;


	//_bool m_bReady;			//대기모드 여부
	//_bool m_bSpawn;			//등장했는가
	//_bool m_bAppear;		//움직이는가
	//_bool m_bTrack;			//추적중인가
	//_bool m_bAttack;		//공격중인가
	//_bool m_bDodge;			//회피중인가

	//_float m_fRangeToTarget;	//타겟까지의 거리
	//_float m_fDodgeTime;		//무적시간 - 흐르는것
	//_float m_fInvinTime;		//무적시간

	//int m_iHPState;				//이전 HP상태 (상태비교용)

};

#endif // !__AI_BASE_ATTACKER_H__
