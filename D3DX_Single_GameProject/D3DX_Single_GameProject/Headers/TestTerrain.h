#pragma once

#ifndef __TEXT_TERRAIN_H__
#define __TEXT_TERRAIN_H__

#include "GameObject.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)

class CVTXTerrain;
class CTexture;
class CTransform;

END_NAMESPACE

class CTestTerrain : public Engine::CGameObject
{
private :
	explicit CTestTerrain(_Device pDevice);
	virtual ~CTestTerrain() = default;

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int	Update_GameObject(const _float& fDeltaTime) override;
	virtual _int	LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

private:
	HRESULT			Add_Component(void);
	HRESULT			SetUp_Material();
	virtual void Free() override;

public:
	static CTestTerrain* Create(_Device pDevice);

private:
	Engine::CVTXTerrain* m_pBufferCom = nullptr;
	Engine::CTexture* m_pTextureCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;



};


#endif // !__TEXT_TERRAIN_H__