#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameObject.h"
#include "Base.h"



BEGIN_NAMESPACE(Engine)

class CVTXTriColor;
class CTransform;

END_NAMESPACE

class CPlayer : public Engine::CGameObject
{
private:
	explicit CPlayer(_Device pDevice);
	virtual ~CPlayer(void) = default;

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

private:
	HRESULT			Add_Component(void);
	void			Key_Input(const _float& fDeltaTime);

private:
	Engine::CVTXTriColor* m_pBufferCom = nullptr;
	//Engine::CTexture* m_pTextureCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;


	_vec3					m_vDir;

public:
	static CPlayer* Create(_Device pDevice);
private:
	virtual void Free(void) override;



};


#endif // !__PLAYER_H__