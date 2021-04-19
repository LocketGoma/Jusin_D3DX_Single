#include "framework.h"
#include "MainMapA.h"

#include "StaticMesh.h"
#include "Transform.h"
#include "Shader.h"


CMainMapA::CMainMapA(_Device pDevice)
	: CMapObject(pDevice)
{
}

CMainMapA::CMainMapA(const CMainMapA& other)
	: CMapObject(other)
{
}

HRESULT CMainMapA::Ready_GameObject(void)
{
	return S_OK;
}

HRESULT CMainMapA::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (pArg != nullptr) {
		_vec3 pVec = *((_vec3*)pArg);
		m_pTransformCom->Set_Scale(pVec);
	}

	m_pTransformCom->Update_Component();

	return S_OK;
}

_int CMainMapA::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	return NO_EVENT;
}

_int CMainMapA::LateUpdate_GameObject(const _float& fDeltaTime)
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

HRESULT CMainMapA::Render_GameObject(void)
{
	//m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pTransformCom->LateUpdate_Component();
	FAILED_CHECK_RETURN(SetUp_Material(), E_FAIL);
	
	m_pMeshCom->Render_Meshes();
	
	
	//쉐이더 처리
	//LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	//NULL_CHECK_RETURN(pEffect, E_FAIL);
	//pEffect->AddRef();

	//FAILED_CHECK_RETURN(Setup_ConstantTable(pEffect), E_FAIL);

	//_uint	iPassMax = 0;

	//pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	////if (m_bClearDead)
	//pEffect->BeginPass(1);


	//m_pMeshCom->Render_Meshes(pEffect);

	//pEffect->EndPass();
	//pEffect->End();

	//Safe_Release(pEffect);

	//쉐이더 처리 끝
	
	
	
	//m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

HRESULT CMainMapA::Add_Component()
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"MapA_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Mesh", pComponent);


	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//쉐이더
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(pManagement->Clone_Prototype(L"Shader_Dissolve"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CMainMapA::SetUp_Material()
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

CMainMapA* CMainMapA::Create(_Device pDevice, const _tchar* pMapName)
{
	CMainMapA* pInstance = new CMainMapA(pDevice);

	pInstance->Set_MapName(pMapName);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CMainMapA::Clone(void* pArg)
{
	CMainMapA* pClone = new CMainMapA(*this);

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CMainMapA");
	}
	return pClone;
}

void CMainMapA::Free()
{
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pTransformCom);

	CMapObject::Free();
}
