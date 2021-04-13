#pragma once

#ifndef __EFFECT_UI_STATUS_H__
#define __EFFECT_UI_STATUS_H__

#include "BaseEffect.h"

class CPlayer;

enum class eStatusType
{
	HP,SUIT,AMMO,END
};

class CStatusUI :  public CBaseEffect
{
protected:
	explicit CStatusUI(_Device pDevice);
	explicit CStatusUI(const CStatusUI& other);
	virtual ~CStatusUI() = default;

public:
	// CBaseEffect��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr);
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

	void Set_Ortho(_vec3 vScale, _vec3 vPos);
	void Set_EffectPosition(_vec3 vScale, _vec3 vPos);
	void Set_EffectVisualble(eStatusType eType, bool bState);

	void Set_HP(_uint iHP);
	void Set_SuitHP(_uint iSuitHP);
	void Set_MagAmmo(_uint iMagAmmo);
	void Set_Ammo(_uint iAmmo);
	void Set_AltAmmo(int iAltAmmo);

private:
	void Change_To_Ortho();
	void Change_To_Proj();

	virtual HRESULT Add_Component();
	virtual void Free();

public:
	static CStatusUI* Create(_Device pDevice);
	virtual Engine::CGameObject* Clone(void* pArg = nullptr) override;

private:
	bool m_bStatus[(_uint)eStatusType::END];
	_mat matWorld, matView, matOrtho;		//�⺻ ���
	_mat matOriView, matOriProj;					//���� ���
	_vec3 m_vScale;
	_vec3 m_vPos;

	_uint m_iHP;							//���� ü��
	_uint m_iSuitHP;						//��Ʈ ü��
	_uint m_iMagAmmo;						//���� źâ ź��
	_uint m_iAmmo;							//��ü ź��
	int m_iAltAmmo;						//2�� ź��

	_tchar m_szHPmsg[128] = L"HEALTH";
	_tchar m_szHP[128];
	_tchar m_szAmmoMsg[128] = L"AMMO";
	_tchar m_szAmmo[128];
	_tchar m_szSuitMsg[128] = L"SUIT";
	_tchar m_szSuitHP[128];

	//Engine::CVTXRectTexture* m_pBufferCom = nullptr;		<-ź�� ��
	Engine::CVTXRectTexture* m_pBufferCom_HP = nullptr;
	Engine::CVTXRectTexture* m_pBufferCom_SUIT = nullptr;

};


#endif // !__EFFECT_UI_STATUS_H__