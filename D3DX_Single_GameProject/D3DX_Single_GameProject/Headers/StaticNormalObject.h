#pragma once

#include "StaticObject.h"

class CStaticNormalObject : public CStaticObject
{
private:
	explicit CStaticNormalObject(_Device pDevice, const _tchar* pMeshName, _float fHitBoxSize, _float iWeight);
	explicit CStaticNormalObject(const CStaticNormalObject& other);
	virtual ~CStaticNormalObject() = default;

public:
	// CStaticObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

	virtual HRESULT Interaction(Engine::CGameObject* pTarget) override;
private:
	HRESULT Add_Component();

public:
	static CStaticNormalObject* Create(_Device pDevice, const _tchar* pMeshName = nullptr, _float fHitBoxSize = 2.5f, _float iWeight = 1.0f);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free();
	_tchar m_pMeshName[128];		//메시 이름

};

