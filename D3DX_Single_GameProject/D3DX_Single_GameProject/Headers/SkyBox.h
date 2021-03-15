#pragma once

#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "GameObject.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)

class CTransform;
class CVTXCubeTexture;
class CTexture;

END_NAMESPACE


class CSkyBox : public Engine::CGameObject
{
private:
	explicit CSkyBox(_Device pDevice);
	explicit CSkyBox(const CSkyBox& other);
	virtual ~CSkyBox() = default;

public:
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0);
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr)override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

public:
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

private:
	HRESULT	Add_Component();


public:
	static CSkyBox* Create(_Device pDevice, _uint iTexnumber = 0);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;

	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CVTXCubeTexture* m_pBufferCom = nullptr;
	Engine::CTexture* m_pTextureCom = nullptr;
	
	_uint m_iTexNumber;
};


#endif // !__SKYBOX_H__