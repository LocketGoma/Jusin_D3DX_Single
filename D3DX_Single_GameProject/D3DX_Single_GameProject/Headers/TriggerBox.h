#pragma once

#ifndef __TRIGGER_BOX_H__
#define __TRIGGER_BOX_H__

#include "GameObject.h"

BEGIN_NAMESPACE(Engine)
class CCollider;
class CTransform;
END_NAMESPACE

class CTriggerBox : public Engine::CGameObject
{
private:
	explicit CTriggerBox(_Device pDevice);
	explicit CTriggerBox(const CTriggerBox& other);
	virtual ~CTriggerBox() = default;

public:
	// CStaticObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;


public:
	static CTriggerBox* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	HRESULT Add_Component();
	virtual void Free() override;

	Engine::CCollider* m_pCollider;
	Engine::CTransform* m_pTransformCom;
};


#endif // !__TRIGGER_BOX_H__