#include "framework.h"
#include "SkyBox.h"
#include "Transform.h"
#include "VTXCubeTexture.h"
#include "Texture.h"

CSkyBox::CSkyBox(_Device pDevice)
    : CGameObject(pDevice)
    , m_iTexNumber(0)
{
}

HRESULT CSkyBox::Ready_GameObject(_uint iTexNumber)
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


    m_pTransformCom->Set_Scale(_vec3(45.f,45.f,45.f));

    m_iTexNumber = iTexNumber;

    return S_OK;
}

_int CSkyBox::Update_GameObject(const _float& fDeltaTime)
{
    Engine::CGameObject::Update_GameObject(fDeltaTime);

    _mat matCamWorld;
    m_pDevice->GetTransform(D3DTS_VIEW, &matCamWorld);
    D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

    m_pTransformCom->Set_Pos(_vec3(matCamWorld._41, matCamWorld._42, matCamWorld._43));

    return NO_EVENT;
}

_int CSkyBox::LateUpdate_GameObject(const _float& fDeltaTime)
{
    m_pTransformCom->Update_Component(fDeltaTime);

    auto pManagement = Engine::CManagement::Get_Instance();
    if (nullptr == pManagement)
    {
        return MANAGER_OUT;
    }
    pManagement->Add_RenderList(Engine::RENDERID::RENDER_PRIORITY, this);

    return NO_EVENT;
}

HRESULT CSkyBox::Render_GameObject(void)
{
    m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
    m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    m_pTransformCom->LateUpdate_Component();
    
    if (FAILED(CGameObject::Render_GameObject()))
        return E_FAIL;
    
    if (FAILED(m_pTextureCom->Set_Texture(m_iTexNumber)))
    {
        return E_FAIL;
    }
    if (FAILED(m_pBufferCom->Render_Buffer()))
    {
        return E_FAIL;
    }

    m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
    m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

    return S_OK;
}

HRESULT CSkyBox::Add_Component()
{
    auto pManagement = Engine::CManagement::Get_Instance();
    if (nullptr == pManagement)
    {
        return E_FAIL;
    }

    Engine::CComponent* pComponent = nullptr;

    // Buffer
    pComponent = m_pBufferCom = dynamic_cast<Engine::CVTXCubeTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_CubeTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[0].emplace(L"Com_Buffer", pComponent);

    // Texture
    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_Skybox"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[0].emplace(L"Com_Texture", pComponent);

    // Transform
    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[0].emplace(L"Com_Transform", pComponent);



    return S_OK;
}

CSkyBox* CSkyBox::Create(_Device pDevice, _uint iTexNumber)
{
    CSkyBox* pInstance = new CSkyBox(pDevice);
    if (FAILED(pInstance->Ready_GameObject(iTexNumber)))
    {
        Safe_Release(pInstance);
    }
    
    return pInstance;
}

Engine::CGameObject* CSkyBox::Clone(void* pArg)
{
    CSkyBox* pClone = new CSkyBox(*this);
    if (pClone == nullptr)
    {
        PRINT_LOG(L"Error", L"Failed To Clone CSkyBox");
    }

    return pClone;
}

void CSkyBox::Free()
{

    Safe_Release(m_pBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);

    Engine::CGameObject::Free();
}
