#pragma once

#ifndef __EFFECT_UI_WEAPON_H__
#define __EFFECT_UI_WEAPON_H__

#include "BaseEffect.h"

//= Weapon Select UI
class CWeaponUI : public CBaseEffect
{
private:
	explicit CWeaponUI(_Device pDevice);
	explicit CWeaponUI(const CWeaponUI& other);
	virtual ~CWeaponUI() = default;

public:
	// CBaseEffect��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr);
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

public:
	void Set_Ortho(_vec3 vScale, _vec3 vPos);
	void Set_EffectPosition(_vec3 vScale, _vec3 vPos);
	void Select_Weapon(_uint iWeaponNumber);

	static CWeaponUI* Create(_Device pDevice);
	virtual Engine::CGameObject* Clone(void* pArg = nullptr) override;

private:
	void Change_To_Ortho();
	void Change_To_Proj();
	void Print_Weapon(_uint iType, _vec2 vPostion);

	HRESULT Add_Component();
	virtual void Free();

private:
	_mat matWorld, matView, matOrtho;		//�⺻ ���
	_mat matOriView, matOriProj;					//���� ���

	_vec3 m_vCloseSize = _vec3(70.f, 70.f, 0.f);
	_vec3 m_vOpenSize = _vec3(260.f, 150.f, 0.f);

	_bool m_bStatus[(_uint)eWeaponType::WEAPON_END];
	Engine::CVTXRectTexture* m_pBufferArrCom[(_uint)eWeaponType::WEAPON_END];
	_uint m_iWeaponNumber;
	_float m_fTimer;			//���� ������
	_float m_fViewInterval;		//�������� �ð�
};


#endif // !__EFFECT_UI_WEAPON_H__