#pragma once

#ifndef __EFFECT_UI_CROSSHAIR_H__
#define __EFFECT_UI_CROSSHAIR_H__

#include "BaseEffect.h"

class CCrossHairUI : public CBaseEffect
{
protected:
	explicit CCrossHairUI(_Device pDevice);
	explicit CCrossHairUI(const CCrossHairUI& other);
	virtual ~CCrossHairUI() = default;

public:
	// CBaseEffectÀ»(¸¦) ÅëÇØ »ó¼ÓµÊ
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr);
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;


	void Set_HP(_uint iHP);
	void Set_MagAmmo(_int iMagAmmo);
	void Set_MagFullAmmo(_int iFullAmmo);

private:
	virtual void Free();

public:
	static CCrossHairUI* Create(_Device pDevice);
	virtual Engine::CGameObject* Clone(void* pArg = nullptr) override;

private:
	_uint m_iHP;							//ÇöÀç Ã¼·Â
	_uint m_iMagAmmo;						//ÇöÀç ÅºÃ¢ Åº¾à
	_uint m_iMagFullAmmo;					//ÅºÃ¢ ÃÖ´ë Åº¾à


	_tchar* m_szLeft_HP_Empty = L"{\0";
	_tchar* m_szLeft_HP_Full = L"[\0";
	_tchar* m_szRight_Ammo_Empty = L"}\0";
	_tchar* m_szRight_Ammo_Full = L"]\0";
	_tchar* m_szCenter_CrossHair = L"Q\0";


	_bool m_bLowAmmoAlert;

};


#endif // !__EFFECT_UI_CROSSHAIR_H__