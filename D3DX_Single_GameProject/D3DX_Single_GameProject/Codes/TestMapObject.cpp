#include "framework.h"
#include "TestMapObject.h"

#include "StaticMesh.h"
#include "Transform.h"

CTestMapObject::CTestMapObject(_Device pDevice)
	: CMapObject(pDevice)
{
}

CTestMapObject::CTestMapObject(const CTestMapObject& other)
	: CMapObject(other)
{
}

HRESULT CTestMapObject::Ready_GameObject(void)
{
	return S_OK;
}

HRESULT CTestMapObject::Ready_GameObject_Clone(void* pArg = nullptr)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (pArg != nullptr) {
		_vec3 pVec = *((_vec3*)pArg);
		m_pTransformCom->Set_Scale(pVec);
	}

	m_pTransformCom->Update_Component();

	return S_OK;
}

_int CTestMapObject::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	return NO_EVENT;
}

_int CTestMapObject::LateUpdate_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::LateUpdate_GameObject(fDeltaTime);

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return 0;
	}
	
	pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);

	return NO_EVENT;
}

HRESULT CTestMapObject::Render_GameObject(void)
{
	m_pTransformCom->LateUpdate_Component();	

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pMeshCom->Render_Meshes();

	return S_OK;
}

HRESULT CTestMapObject::Add_Component()
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"TestMapX"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Mesh", pComponent);


	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;


}

CTestMapObject* CTestMapObject::Create(_Device pDevice, const _tchar* pMapName)
{
	CTestMapObject* pInstance = new CTestMapObject(pDevice);

	pInstance->Set_MapName(pMapName);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CTestMapObject::Clone(void* pArg = nullptr)
{
	CTestMapObject* pClone = new CTestMapObject(*this);

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CTestMapObject");
	}
	return pClone;
}

void CTestMapObject::Free()
{
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pTransformCom);

	CMapObject::Free();
}
