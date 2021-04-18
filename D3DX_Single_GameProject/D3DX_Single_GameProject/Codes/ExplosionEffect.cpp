#include "framework.h"
#include "ExplosionEffect.h"

#include "VTXRectTexture.h"
#include "Transform.h"
#include "Texture.h"


CExplosionEffect::CExplosionEffect(_Device pDevice)
    : CBaseEffect(pDevice)
{
}

CExplosionEffect::CExplosionEffect(const CExplosionEffect& other)
    : CBaseEffect(other)
{
}

HRESULT CExplosionEffect::Ready_GameObject(void)
{
    return S_OK;
}

HRESULT CExplosionEffect::Ready_GameObject_Clone(void* pArg = nullptr)
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    return S_OK;
}

_int CExplosionEffect::Update_GameObject(const _float& fDeltaTime)
{
    return NO_EVENT;
}

_int CExplosionEffect::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (m_fNowTime >= m_fLifetime)
	{
		Set_Size(_vec3(5.f, 5.f, 5.f));

		m_fNowTime = 0.f;
		return OBJ_DEAD;
	}
	m_fNowTime += fDeltaTime;
	Set_Size(Get_Size() * 1.15f);


	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	m_pTransformCom->Update_Component();

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_EFFECT, this);

	return NO_EVENT;
}

HRESULT CExplosionEffect::Render_GameObject(void)
{
	_mat		matBill, matView, matWorld;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	matWorld = m_pTransformCom->Get_TransformDescription().matWorld;

	m_pTransformCom->Set_WorldMatrix((matBill * matWorld));

	m_pTransformCom->LateUpdate_Component();

	if (FAILED(CGameObject::Render_GameObject()))
	{
		return E_FAIL;
	}


	if (FAILED(m_pTextureCom->Set_Texture()))
	{
		return E_FAIL;
	}

	if (FAILED(m_pBufferCom->Render_Buffer()))
	{
		return E_FAIL;
	}

	return S_OK;
}

CExplosionEffect* CExplosionEffect::Create(_Device pDevice)
{
	CExplosionEffect* pInstance = new CExplosionEffect(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CExplosionEffect::Clone(void* pArg = nullptr)
{
	CExplosionEffect* pClone = new CExplosionEffect(*this);

	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CExplosionEffect");

		return nullptr;
	}
	if (FAILED(pClone->Ready_GameObject_Clone()))
	{
		Safe_Release(pClone);
	}


	return pClone;
}

HRESULT CExplosionEffect::Add_Component()
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	Engine::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CVTXRectTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_BreakAmmo"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Texture", pComponent);

	return S_OK;
}

void CExplosionEffect::Free()
{
	CBaseEffect::Free();
}
