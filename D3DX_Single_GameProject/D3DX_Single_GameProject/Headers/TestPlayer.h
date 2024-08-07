#pragma once

#ifndef __TESTPLAYER_H__
#define __TESTPLAYER_H__

#include "GameObject.h"
#include "Base.h"




BEGIN_NAMESPACE(Engine)
class CVTXTriColor;
class CTransform;
class CNaviMesh;

END_NAMESPACE

class CTestPlayer : public Engine::CGameObject
{
private:
	explicit CTestPlayer(_Device pDevice);
	explicit CTestPlayer(const CTestPlayer& other);
	virtual ~CTestPlayer(void) = default;

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

public:
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

private:
	HRESULT			Add_Component(void);
	void			Key_Input(const _float& fDeltaTime);

private:
	Engine::CVTXTriColor* m_pBufferCom = nullptr;
	//Engine::CTexture* m_pTextureCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;

	Engine::CNaviMesh* m_pNaviMeshCom = nullptr;


	_vec3					m_vDir;

public:
	static CTestPlayer* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free(void) override;



};


#endif // !__TESTPLAYER_H__