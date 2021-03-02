#pragma once

#ifndef __LOGO_IMAGE_H__
#define __LOGO_IMAGE_H__

#include "GameObject.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)
class CTexture;
class CVTXRectTexture;
class CTransform;

END_NAMESPACE


class CLogoImage :  public Engine::CGameObject
{
private:
	explicit CLogoImage(_Device pDevice);
	virtual ~CLogoImage() = default;

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual HRESULT Render_GameObject(void) override;

private:
	HRESULT			Add_Component(void);
	void			Free();

public:
	static CLogoImage* Create(_Device pDevice);

private:
	Engine::CVTXRectTexture* m_pBufferCom = nullptr;
	Engine::CTexture* m_pTextureCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;


};


#endif // !__LOGO_IMAGE_H__