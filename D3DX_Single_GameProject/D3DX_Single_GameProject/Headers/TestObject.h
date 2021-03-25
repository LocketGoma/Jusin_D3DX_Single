#pragma once

#ifndef __TEST_OBJECT_H__

#include "GameObject.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)
class CStaticMesh;
class CDynamicMesh;
class CTransform;

END_NAMESPACE

class CTestObject : public Engine::CGameObject
{
private:
	explicit CTestObject(_Device pDevice);
	explicit CTestObject(const CTestObject& other);
	virtual ~CTestObject() = default;

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr);
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

private:
	//Engine::CStaticMesh* m_pMeshCom = nullptr;
	Engine::CDynamicMesh* m_pMeshCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;

public:
	static CTestObject* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;
private:
	virtual void Free(void) override;

};


#endif // !__TEST_OBJECT_H__