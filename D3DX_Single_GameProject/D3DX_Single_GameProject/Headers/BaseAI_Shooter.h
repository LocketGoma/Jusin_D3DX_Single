#pragma once

#ifndef __AI_BASE_SHOOTER_H__
#define __AI_BASE_SHOOTER_H__

#include "BaseAI.h"

//�ΰ��� ���� ����
class CBaseAI_Shooter : public CBaseAI
{
private:
	explicit CBaseAI_Shooter(_Device pDevice);
	explicit CBaseAI_Shooter(const CBaseAI_Shooter& other);
	virtual ~CBaseAI_Shooter() = default;


public:
	// CBaseAI��(��) ���� ��ӵ�
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
	static CBaseAI_Shooter* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;


private:
	virtual void Free() override;
};


#endif // !__AI_BASE_SHOOTER_H__