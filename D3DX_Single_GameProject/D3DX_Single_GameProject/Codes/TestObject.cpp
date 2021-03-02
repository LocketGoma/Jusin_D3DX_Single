#include "framework.h"

#include "Transform.h"
#include "StaticMesh.h"

#include "TestObject.h"

CTestObject::CTestObject(_Device pDevice)
    : CGameObject(pDevice)
{
}

HRESULT CTestObject::Ready_GameObject(void)
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return S_OK;
}

_int CTestObject::Update_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::Update_GameObject(fTimeDelta);

	return NO_EVENT;
}

_int CTestObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return 0;
	}
	pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);

	return NO_EVENT;
}

HRESULT CTestObject::Render_GameObject(void)
{
	m_pTransformCom->LateUpdate_Component();

	m_pMeshCom->Render_Meshes();

	return S_OK;
}

HRESULT CTestObject::Add_Component(void)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	Engine::CComponent* pComponent = nullptr;


	// StaticMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pManagement-> Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"Mesh_Stone"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[0].emplace(L"Com_Mesh", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[0].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

CTestObject* CTestObject::Create(_Device pDevice)
{
	CTestObject* pInstance = new CTestObject(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;

}

void CTestObject::Free(void)
{
	Engine::CGameObject::Free();
}
