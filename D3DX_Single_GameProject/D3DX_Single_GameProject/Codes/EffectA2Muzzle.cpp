#include "framework.h"
#include "EffectA2Muzzle.h"

#include "VTXRectTexture.h"
#include "Transform.h"
#include "Texture.h"

CEffectA2Muzzle::CEffectA2Muzzle(_Device pDevice)
	: CEffectMuzzle(pDevice)
{
}

CEffectA2Muzzle::CEffectA2Muzzle(const CEffectA2Muzzle& other)
	: CEffectMuzzle(other)
{
}

HRESULT CEffectA2Muzzle::Ready_GameObject(void)
{
	return S_OK;
}

HRESULT CEffectA2Muzzle::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CEffectA2Muzzle::Update_GameObject(const _float& fDeltaTime)
{
	return _int();
}

_int CEffectA2Muzzle::LateUpdate_GameObject(const _float& fDeltaTime)
{
    Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
    if (nullptr == pManagement)
    {
        return MANAGER_OUT;
    }       

    if (m_bIsVisible)
        pManagement->Add_RenderList(Engine::RENDERID::RENDER_EFFECT, this);

    return NO_EVENT;
}

HRESULT CEffectA2Muzzle::Render_GameObject(void)
{
    //Set_Ortho(m_vScale, m_vPos);

    Change_To_Ortho();

    if (FAILED(CGameObject::Render_GameObject()))
    {
        return E_FAIL;
    }

    //if (FAILED(m_pTextureCom->Set_Texture(0)))
    if (FAILED(m_pTextureCom->Set_Texture(rand() % A2_MUZZLE_EFFECT_COUNT)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pBufferCom->Render_Buffer()))
    {
        return E_FAIL;
    }

    Change_To_Proj();

	return S_OK;
}


HRESULT CEffectA2Muzzle::Add_Component()
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
    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_Ar2_Muzzle"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Texture", pComponent);

    return S_OK;
}

CEffectA2Muzzle* CEffectA2Muzzle::Create(_Device pDevice)
{
    CEffectA2Muzzle* pInstance = new CEffectA2Muzzle(pDevice);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
    }

    return pInstance;
}

Engine::CGameObject* CEffectA2Muzzle::Clone(void* pArg)
{
    CEffectA2Muzzle* pClone = new CEffectA2Muzzle(*this);

    if (pClone == nullptr)
    {
        PRINT_LOG(L"Error", L"Failed To Clone CEnemyHunter");

        return nullptr;
    }
    if (FAILED(pClone->Ready_GameObject_Clone()))
    {
        Safe_Release(pClone);
    }


    return pClone;
}

void CEffectA2Muzzle::Free()
{
    CBaseEffect::Free();
}