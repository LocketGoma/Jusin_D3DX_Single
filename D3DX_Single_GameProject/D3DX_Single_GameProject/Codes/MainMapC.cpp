#include "framework.h"
#include "MainMapC.h"

#include "StaticMesh.h"
#include "Transform.h"

CMainMapC::CMainMapC(_Device pDevice)
	: CMapObject(pDevice)
{
}

CMainMapC::CMainMapC(const CMainMapC& other)
	: CMapObject(other)
{
}

HRESULT CMainMapC::Ready_GameObject(void)
{
	return S_OK;
}

HRESULT CMainMapC::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (pArg != nullptr) {
		_vec3 pVec = *((_vec3*)pArg);
		m_pTransformCom->Set_Scale(pVec);
	}

	m_pTransformCom->Update_Component();

	return S_OK;
}

_int CMainMapC::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	return NO_EVENT;
}

_int CMainMapC::LateUpdate_GameObject(const _float& fDeltaTime)
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

HRESULT CMainMapC::Render_GameObject(void)
{
	//m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pTransformCom->LateUpdate_Component();
	FAILED_CHECK_RETURN(SetUp_Material(), E_FAIL);		//얘 빠져서 조명 안나오는중이었음..
	m_pMeshCom->Render_Meshes();

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);	

	return S_OK;
}

CMainMapC* CMainMapC::Create(_Device pDevice, const _tchar* pMapName)
{
	CMainMapC* pInstance = new CMainMapC(pDevice);

	pInstance->Set_MapName(pMapName);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CMainMapC::Clone(void* pArg)
{
	CMainMapC* pClone = new CMainMapC(*this);

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CMainMapC");
	}
	return pClone;
}

HRESULT CMainMapC::Add_Component()
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"MapC_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Mesh", pComponent);


	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

HRESULT CMainMapC::SetUp_Material()
{
	D3DMATERIAL9			tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(tMtrlInfo));


	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 0.f;

	m_pDevice->SetMaterial(&tMtrlInfo);

	return S_OK;
}


void CMainMapC::Free()
{
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pTransformCom);

	CMapObject::Free();
}
