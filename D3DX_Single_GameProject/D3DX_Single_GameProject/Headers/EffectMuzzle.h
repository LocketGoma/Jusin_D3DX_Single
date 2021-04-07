#pragma once

#ifndef __EFFECT_MUZZLE_H__
#define __EFFECT_MUZZLE_H__

#include "BaseEffect.h"

#define MUZZLE_EFFECT_COUNT 4

class CEffectMuzzle : public CBaseEffect
{
protected:
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

	void Set_Ortho(_vec3 vScale, _vec3 vPos);

	void Set_EffectPosition(_vec3 vScale, _vec3 vPos);


protected:
	void Change_To_Ortho();
	void Change_To_Proj();
	
	virtual HRESULT Add_Component();
	virtual void Free();

public:
	static CEffectMuzzle* Create(_Device pDevice);
	virtual Engine::CGameObject* Clone(void* pArg = nullptr) override;

protected:
	_mat matWorld, matView, matOrtho;		//기본 행렬
	_mat matOriView, matOriProj;					//원본 행렬
	_vec3 m_vScale;
	_vec3 m_vPos;

};


#endif // !__EFFECT_MUZZLE_H__