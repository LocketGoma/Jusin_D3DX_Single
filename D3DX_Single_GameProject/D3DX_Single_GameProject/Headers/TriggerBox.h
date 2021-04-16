#pragma once

#ifndef __TRIGGER_BOX_H__
#define __TRIGGER_BOX_H__

#include "GameObject.h"

BEGIN_NAMESPACE(Engine)
class CCollider;
class CTransform;
class CControlSupportUnit;
END_NAMESPACE

enum class ePassType
{
	COL_PASSED,COL_CLOSED,END
};

class CTriggerBox : public Engine::CGameObject
{
private:
	explicit CTriggerBox(_Device pDevice);
	explicit CTriggerBox(const CTriggerBox& other);
	virtual ~CTriggerBox() = default;

public:
	// CStaticObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject(_vec3 vMin, _vec3 vMax);
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr);
	virtual HRESULT Ready_GameObject_Clone(_vec3 vMin, _vec3 vMax);
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

public:
	// CGameObject을(를) 통해 상속됨
	// 아마 안씀
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;
	
public:
	Engine::CControlSupportUnit* Get_SupportUnit();
	_bool Check_Collision(const _vec3* pSourcePos, const _float fSourceRadius);
	_vec3* Check_Passed(_vec3* pSourcePos);
	const Engine::CTransform* Get_Transform();

	ePassType Get_PassType();
	void Set_PassType(ePassType eType);

public:
	static CTriggerBox* Create(_Device pDevice);
	static CTriggerBox* Create(_Device pDevice, _vec3 vMin, _vec3 vMax);
	virtual CGameObject* Clone(_vec3 vMin, _vec3 vMax);
	virtual CGameObject* Clone(void* pArg = nullptr);

private:
	HRESULT Add_Component();
	virtual void Free() override;

	Engine::COLIDETYPE m_eType;
	ePassType m_ePassType;

	Engine::CCollider* m_pColliderCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CControlSupportUnit* m_pSupportCom;

	_vec3 m_vPos, m_vMin, m_vMax;	

	_float m_fLength;

};


#endif // !__TRIGGER_BOX_H__