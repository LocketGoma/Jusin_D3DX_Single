#pragma once

#ifndef __BASE_EFFECT_H__
#define __BASE_EFFECT_H__

#include "GameObject.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)
class CVTXRectTexture;
class CTransform;
class CTexture;
END_NAMESPACE

class CBaseEffect : public Engine::CGameObject
{
protected:
	explicit CBaseEffect(_Device pDevice);
	explicit CBaseEffect(const CBaseEffect& other);
	virtual ~CBaseEffect() = default;

public:
	virtual HRESULT Ready_GameObject(void) PURE;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr) PURE;
	virtual _int Update_GameObject(const _float & fDeltaTime) PURE;
	virtual _int LateUpdate_GameObject(const _float & fDeltaTime) PURE;
	virtual HRESULT Render_GameObject(void) PURE;

	// CGameObject을(를) 통해 상속됨
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

	void Set_Visible(_bool bState);
	_bool Get_Visible();

public:
	void Set_Target(Engine::CDynamicMesh* pTarget);
	void Set_ParentMatrix(_mat& pMat);
	void Set_UpParentMatrix(_mat& pMat);

protected:
	virtual void Free();

public:
	// CGameObject을(를) 통해 상속됨
	virtual Engine::CGameObject* Clone(void* pArg = nullptr) PURE;


protected:
	Engine::CDynamicMesh* m_pTargetMesh = nullptr;		//다이나믹매시요?

	Engine::CVTXRectTexture* m_pBufferCom = nullptr;	
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CTexture* m_pTextureCom = nullptr;

	_mat matParent;
	_mat matUpParent;

	_bool m_bIsVisible;
};


#endif // !__BASE_EFFECT_H__