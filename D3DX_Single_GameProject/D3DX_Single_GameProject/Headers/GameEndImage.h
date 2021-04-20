#pragma once

#ifndef __END_IMAGE_H__
#define __END_IMAGE_H__

#include "GameObject.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)
class CTexture;
class CVTXRectTexture;
class CTransform;
class CShader;

END_NAMESPACE


class CGameEndImage : public Engine::CGameObject
{
private:
	explicit CGameEndImage(_Device pDevice);
	virtual ~CGameEndImage() = default;

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

private:
	HRESULT Setup_ConstantTable(LPD3DXEFFECT& pEffect);
	HRESULT			Add_Component(void);
	void			Free();


public:
	static CGameEndImage* Create(_Device pDevice);

private:
	Engine::CVTXRectTexture* m_pBufferCom = nullptr;
	Engine::CTexture* m_pTextureCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CShader* m_pShaderCom = nullptr;

	float m_fFadeAmount;			//페이드 인/아웃용

//이하 안쓰는 애들
private:
	// CGameObject을(를) 통해 상속됨
	virtual Engine::CGameObject* Clone(void* pArg = (void*)nullptr);


	// CGameObject을(를) 통해 상속됨
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

};


#endif // !__END_IMAGE_H__