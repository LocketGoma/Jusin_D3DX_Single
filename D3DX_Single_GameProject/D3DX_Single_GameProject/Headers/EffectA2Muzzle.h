#pragma once

#ifndef __EFFECT_A2_MUZZLE_H__
#define __EFFECT_A2_MUZZLE_H__

#include "BaseEffect.h"
#include "EffectMuzzle.h"

#define A2_MUZZLE_EFFECT_COUNT 2

class CEffectA2Muzzle : public CEffectMuzzle
{
private:
	explicit CEffectA2Muzzle(_Device pDevice);
	explicit CEffectA2Muzzle(const CEffectA2Muzzle& other);
	virtual ~CEffectA2Muzzle() = default;

public:
	// CBaseEffect을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr);
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;


private:
	
	virtual	HRESULT Add_Component() override;
	virtual void Free();

public:
	static CEffectA2Muzzle* Create(_Device pDevice);
	virtual Engine::CGameObject* Clone(void* pArg = nullptr) override;


};


#endif // !__EFFECT_MUZZLE_H__