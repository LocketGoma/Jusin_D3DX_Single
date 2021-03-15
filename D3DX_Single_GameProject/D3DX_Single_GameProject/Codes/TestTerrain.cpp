#include "framework.h"
#include "TestTerrain.h"

#include "VTXTerrain.h"
#include "Transform.h"
#include "Texture.h"

CTestTerrain::CTestTerrain(_Device pDevice)
	: CGameObject(pDevice)
{
}

CTestTerrain::CTestTerrain(const CTestTerrain& other)
	: CGameObject(other)
{
}

HRESULT CTestTerrain::Ready_GameObject(void)
{
	return S_OK;
}

HRESULT CTestTerrain::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CTestTerrain::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);


	return 0;
}

_int CTestTerrain::LateUpdate_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::LateUpdate_GameObject(fDeltaTime);

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return 0;
	}
	pManagement->Add_RenderList(Engine::RENDERID::RENDER_PRIORITY, this);
		
	return NO_EVENT;
}

HRESULT CTestTerrain::Render_GameObject(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	//텍스쳐 종류 선택 -> Set_Texture에다가 값 넣으면 됨.

	m_pTransformCom->LateUpdate_Component();

	if (FAILED(m_pTextureCom->Set_Texture(1)))
	{
		return E_FAIL;
	}	

	FAILED_CHECK_RETURN(SetUp_Material(), );

	if (FAILED(m_pBufferCom->Render_Buffer()))
	{
		return E_FAIL;
	}

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

void CTestTerrain::Set_Position(_vec3 vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CTestTerrain::Set_Size(_vec3 vSize)
{
	m_pTransformCom->Set_Pos(vSize);
}

_vec3 CTestTerrain::Get_Position()
{
	return m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
}

_vec3 CTestTerrain::Get_Size()
{
	return m_pTransformCom->Get_TransformDescription().vScale;
}

HRESULT CTestTerrain::Add_Component(void)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	Engine::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CVTXTerrain*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_TerrainTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[0].emplace(L"Com_Buffer", pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[0].emplace(L"Com_Texture", pComponent);


	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[0].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

HRESULT CTestTerrain::SetUp_Material()
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

Engine::CGameObject* CTestTerrain::Clone(void* pArg)
{
	CTestTerrain* pClone = new CTestTerrain(*this);

	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CTestTerrain");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}
