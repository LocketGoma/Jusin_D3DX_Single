#pragma once

#ifndef __AI_BOSS_HUNTER_H__
#define __AI_BOSS_HUNTER_H__

#include "BaseAI.h"

//헌터 전용
class CBossAI_Hunter : public CBaseAI
{
private:
	explicit CBossAI_Hunter(_Device pDevice);
	explicit CBossAI_Hunter(const CBossAI_Hunter& other);
	virtual ~CBossAI_Hunter() = default;


public:
	// CBaseAI을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;
	
public:
	virtual HRESULT Do_Spawn(const _float& fDeltaTime) override;
	virtual HRESULT Do_Appear(const _float& fDeltaTime) override;
	virtual HRESULT Do_Idle(const _float& fDeltaTime) override;
	virtual HRESULT Do_Movement(const _float& fDeltaTime) override;
	virtual HRESULT Do_Attack(const _float& fDeltaTime) override;


public:
	static CBossAI_Hunter* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;
};

#endif // !__AI_BOSS_HUNTER_H__
