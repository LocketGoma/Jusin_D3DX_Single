#pragma once

#ifndef __EFFECT_MUZZLE_H__
#define __EFFECT_MUZZLE_H__

#include "BaseEffect.h"

#define MUZZLE_EFFECT_COUNT 4

class CEffectMuzzle : public CBaseEffect
{
private:
	explicit CEffectMuzzle(_Device pDevice);
	explicit CEffectMuzzle(const CEffectMuzzle& other);
	virtual ~CEffectMuzzle() = default;

public:
	// CBaseEffect을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr);
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;
	HRESULT Render_GameObject(_mat& matParent);

private:
	HRESULT Add_Component();
	virtual void Free();

public:
	static CEffectMuzzle* Create(_Device pDevice);
	virtual Engine::CGameObject* Clone(void* pArg = nullptr) override;

private:


};


#endif // !__EFFECT_MUZZLE_H__