#include "framework.h"

#include "Transform.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"

#include "TestObject.h"

CTestObject::CTestObject(_Device pDevice)
    : CGameObject(pDevice)
{
}

CTestObject::CTestObject(const CTestObject& other)
	: CGameObject(other)	
	, m_pTransformCom(other.m_pTransformCom)
	, m_pMeshCom(other.m_pMeshCom)
{
	Safe_AddReference(m_pMeshCom);
	Safe_AddReference(m_pTransformCom);
}

HRESULT CTestObject::Ready_GameObject(void)
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.05f, 0.05f, 0.05f));

    return S_OK;
}

HRESULT CTestObject::Ready_GameObject_Clone(void* vArg)
{

	if (vArg != nullptr) {
		_vec3 pVec = *((_vec3*)vArg);
		m_pTransformCom->Set_Scale(pVec);
	}
	else
	{
		m_pTransformCom->Set_Scale(_vec3(0.0005f, 0.0005f, 0.0005f));
	}
	m_pMeshCom->Set_AnimationSet(1);
	m_pTransformCom->Update_Component();

	return S_OK;
}

_int CTestObject::Update_GameObject(const _float& fDeltaTime)
{
    Engine::CGameObject::Update_GameObject(fDeltaTime);

	return NO_EVENT;
}

_int CTestObject::LateUpdate_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::LateUpdate_GameObject(fDeltaTime);

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return 0;
	}
	m_pMeshCom->Play_AnimationSet(fDeltaTime);

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);

	return NO_EVENT;
}

HRESULT CTestObject::Render_GameObject(void)
{

	m_pTransformCom->LateUpdate_Component();

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pMeshCom->Render_Meshes();

	return S_OK;
}

void CTestObject::Set_Position(_vec3 vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CTestObject::Set_Size(_vec3 vSize)
{
	m_pTransformCom->Set_Scale(vSize);
}

_vec3 CTestObject::Get_Position()
{
	return m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
}

_vec3 CTestObject::Get_Size()
{
	return m_pTransformCom->Get_TransformDescription().vScale;
}

HRESULT CTestObject::Add_Component(void)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	Engine::CComponent* pComponent = nullptr;

	//// StaticMesh
	//pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pManagement-> Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"Mesh_HL2Dog"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[0].emplace(L"Com_Mesh", pComponent);

	//pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"Mesh_Ant"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[0].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"Mesh_HL2Dog"));
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

Engine::CGameObject* CTestObject::Clone(void* pArg)
{
	CTestObject* pClone = new CTestObject(*this);

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CTestObject");
	}
	return pClone;
}

void CTestObject::Free(void)
{
	
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pTransformCom);

	Engine::CGameObject::Free();
}
