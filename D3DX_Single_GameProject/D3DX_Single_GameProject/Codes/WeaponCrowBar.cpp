#include "framework.h"
#include "WeaponCrowBar.h"

#include "Transform.h"

CWeaponCrowbar::CWeaponCrowbar(_Device pDevice)
	: CPlayerWeapon(pDevice)
{
}

CWeaponCrowbar::CWeaponCrowbar(const CWeaponCrowbar& other)
	: CPlayerWeapon(other)
{
}

HRESULT CWeaponCrowbar::Ready_GameObject()
{
	return S_OK;
}

HRESULT CWeaponCrowbar::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.005f, 0.005f, 0.005f));
	//m_pTransformCom->Rotation(Engine::ROTATION::ROT_X, D3DXToRadian(-90.f));

	m_pMeshCom->Set_AnimationSet(2);

	return S_OK;
}

_int CWeaponCrowbar::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);



	return NO_EVENT;
}

_int CWeaponCrowbar::LateUpdate_GameObject(const _float& fDeltaTime)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}


	m_pTransformCom->Set_Scale(_vec3(0.05f, 0.05f, 0.05f));

	m_pTransformCom->Update_Component();

	m_pMeshCom->Play_AnimationSet(fDeltaTime);

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);

	return NO_EVENT;
}

HRESULT CWeaponCrowbar::Render_GameObject(void)
{


	_mat matView, matScale, matPos, matRotX;
	//_vec3 vPos;
	//_vec3 vScale = _vec3(0.05f, 0.05f, 0.05f);
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXMatrixScaling(&matScale, 0.05f, 0.05f, 0.05f);
	//D3DXMatrixTranslation(&matPos, 0.f, -10.f, 0.f);
	//D3DXMatrixRotationX(&matRotX, D3DXToRadian(-90.f));
	matView = matScale * matView;
	//matView *= matScale * matRotX * matPos;
	m_pDevice->SetTransform(D3DTS_WORLD, &matView);
	


	//m_pTransformCom->LateUpdate_Component();

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	m_pMeshCom->Render_Meshes();

	return S_OK;

}

void CWeaponCrowbar::Shoot_Weapon()
{
}

void CWeaponCrowbar::AltShoot_Weapon()
{
}

void CWeaponCrowbar::Reload_Weapon()
{
}

HRESULT CWeaponCrowbar::Add_Component(void)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"Crowbar"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	return S_OK;
}

CWeaponCrowbar* CWeaponCrowbar::Create(_Device pDevice)
{
	CWeaponCrowbar* pInstance = new CWeaponCrowbar(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CWeaponCrowbar::Clone(void* pArg)
{
	CWeaponCrowbar* pClone = new CWeaponCrowbar(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CWeaponCrowbar");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CWeaponCrowbar::Free()
{
	CPlayerWeapon::Free();
}
