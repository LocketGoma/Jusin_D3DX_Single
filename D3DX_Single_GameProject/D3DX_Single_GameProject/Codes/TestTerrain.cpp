#include "framework.h"
#include "TestTerrain.h"

#include "VTXTerrain.h"
#include "Transform.h"
#include "Texture.h"

CTestTerrain::CTestTerrain(_Device pDevice)
	: CGameObject(pDevice)
{
}

HRESULT CTestTerrain::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CTestTerrain::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);


	return 0;
}

_int CTestTerrain::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return 0;
	}
	pManagement->Add_RenderList(Engine::RENDERID::RENDER_PRIORITY, this);
		
	return m_pTransformCom->LateUpdate_Component(fTimeDelta);
}

HRESULT CTestTerrain::Render_GameObject(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	return S_OK;
}

HRESULT CTestTerrain::Add_Component(void)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return 0;
	}

	Engine::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CVTXTerrain*>(pManagement->Clone_Resource(0, L"Buffer_TerrainTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[0].emplace(L"Com_Buffer", pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(pManagement->Clone_Resource(1, L"Texture_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[0].emplace(L"Com_Texture", pComponent);


	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[0].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

void CTestTerrain::Free()
{
	Engine::CGameObject::Free();
}

CTestTerrain* CTestTerrain::Create(_Device pDevice)
{
	CTestTerrain* pInstance = new CTestTerrain(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}