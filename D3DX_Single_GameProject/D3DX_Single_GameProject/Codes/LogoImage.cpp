#include "framework.h"
#include "LogoImage.h"

#include "Texture.h"
#include "Transform.h"
#include "VTXRectTexture.h"

CLogoImage::CLogoImage(_Device pDevice)
    : CGameObject(pDevice)
{
}

HRESULT CLogoImage::Ready_GameObject(void)
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransformCom->Set_Scale(_vec3(16.f*0.7f, 9.f*0.7f, 1.f));

    return S_OK;
}

_int CLogoImage::Update_GameObject(const _float& fDeltaTime)
{
    Engine::CGameObject::Update_GameObject(fDeltaTime);

    return NO_EVENT;
}

_int CLogoImage::LateUpdate_GameObject(const _float& fDeltaTime)
{
    auto pManagement = Engine::CManagement::Get_Instance();
    if (nullptr == pManagement)
    {
        return MANAGER_OUT;
    }
    pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);

    return NO_EVENT;
}

HRESULT CLogoImage::Render_GameObject(void)
{
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

HRESULT CLogoImage::Add_Component(void)
{
    auto pManagement = Engine::CManagement::Get_Instance();
    if (nullptr == pManagement)
    {
        return MANAGER_OUT;
    }

    Engine::CComponent* pComponent = nullptr;

    // Buffer
    pComponent = m_pBufferCom = dynamic_cast<Engine::CVTXRectTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_RectTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[0].emplace(L"Com_Buffer", pComponent);

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[0].emplace(L"Com_Transform", pComponent);

    // Texture
    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_Logo"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[0].emplace(L"Com_Texture", pComponent);

    return S_OK;

}

void CLogoImage::Free()
{
    Safe_Release(m_pBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);

    CGameObject::Free();
}

CLogoImage* CLogoImage::Create(_Device pDevice)
{
    CLogoImage* pInstance = new CLogoImage(pDevice);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
    }

    return pInstance;
}

Engine::CGameObject* CLogoImage::Clone(void* pArg)
{
    CLogoImage* pClone = new CLogoImage(*this);

    if (pClone == nullptr)
    {
        return nullptr;
    }

    return pClone;
}
