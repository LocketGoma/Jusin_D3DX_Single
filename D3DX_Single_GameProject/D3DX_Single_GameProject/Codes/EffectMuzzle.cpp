#include "framework.h"
#include "EffectMuzzle.h"

#include "VTXRectTexture.h"
#include "Transform.h"
#include "Texture.h"

CEffectMuzzle::CEffectMuzzle(_Device pDevice)
	: CBaseEffect(pDevice)
{
}

CEffectMuzzle::CEffectMuzzle(const CEffectMuzzle& other)
	: CBaseEffect(other)
{
}

HRESULT CEffectMuzzle::Ready_GameObject(void)
{
	return S_OK;
}

HRESULT CEffectMuzzle::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CEffectMuzzle::Update_GameObject(const _float& fDeltaTime)
{
	return _int();
}

_int CEffectMuzzle::LateUpdate_GameObject(const _float& fDeltaTime)
{
    auto pManagement = Engine::CManagement::Get_Instance();
    if (nullptr == pManagement)
    {
        return MANAGER_OUT;
    }       

    if (m_bIsVisible)
        pManagement->Add_RenderList(Engine::RENDERID::RENDER_EFFECT, this);

    return NO_EVENT;
}

HRESULT CEffectMuzzle::Render_GameObject(void)
{
    //Set_Ortho(m_vScale, m_vPos);

    Change_To_Ortho();

    if (FAILED(CGameObject::Render_GameObject()))
    {
        return E_FAIL;
    }

    //if (FAILED(m_pTextureCom->Set_Texture(0)))
    if (FAILED(m_pTextureCom->Set_Texture(rand() % MUZZLE_EFFECT_COUNT)))
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

void CEffectMuzzle::Set_Ortho(_vec3 vScale, _vec3 vPos)
{
    _mat matWorld, matView, matProj, matOrtho;

    
    m_pDevice->GetTransform(D3DTS_VIEW, &matOriView);
    m_pDevice->GetTransform(D3DTS_PROJECTION, &matOriProj);
    


    D3DXMatrixIdentity(&matWorld);
    D3DXMatrixIdentity(&matView);
    D3DXMatrixIdentity(&matProj);

    D3DXMatrixOrthoLH(
        &matOrtho, /* 직교투영행렬 반환 */
        WINCX, // 가로폭 (WINCX, 하드코딩한거 반드시 바꿀것)
        WINCY, // 세로폭 (WINCY)
       0, /* Near Z: 관찰자와 가장 가까운 면과의 Z거리 */
        1 /* Far Z: 관찰자와 가장 먼 편과의 Z거리 */);

    matProj = matOrtho;


    matView._11 = vScale.x;
    matView._22 = vScale.y;
    matView._33 = vScale.z;

    matView._41 = vPos.x;
    matView._42 = vPos.y;
    matView._43 = vPos.z;

    m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
    m_pDevice->SetTransform(D3DTS_VIEW, &matView);
    m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);



}

void CEffectMuzzle::Set_EffectPosition(_vec3 vScale, _vec3 vPos)
{
    m_vScale = vScale;
    m_vPos = vPos;



    D3DXMatrixIdentity(&matWorld);
    D3DXMatrixIdentity(&matView);
    D3DXMatrixIdentity(&matOrtho);

    D3DXMatrixOrthoLH(
        &matOrtho, /* 직교투영행렬 반환 */
        WINCX, // 가로폭 (WINCX, 하드코딩한거 반드시 바꿀것)
        WINCY, // 세로폭 (WINCY)
        0, /* Near Z: 관찰자와 가장 가까운 면과의 Z거리 */
        1 /* Far Z: 관찰자와 가장 먼 편과의 Z거리 */);


    matView._11 = vScale.x;
    matView._22 = vScale.y;
    matView._33 = vScale.z;

    matView._41 = vPos.x;
    matView._42 = vPos.y;
    matView._43 = vPos.z;

}

void CEffectMuzzle::Change_To_Ortho()
{
    m_pDevice->GetTransform(D3DTS_VIEW, &matOriView);
    m_pDevice->GetTransform(D3DTS_PROJECTION, &matOriProj);

    m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
    m_pDevice->SetTransform(D3DTS_VIEW, &matView);
    m_pDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
}

void CEffectMuzzle::Change_To_Proj()
{
    m_pDevice->SetTransform(D3DTS_VIEW, &matOriView);
    m_pDevice->SetTransform(D3DTS_PROJECTION, &matOriProj);
}

HRESULT CEffectMuzzle::Add_Component()
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
    m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Buffer", pComponent);

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

    // Texture
    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_Muzzle"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Texture", pComponent);

    return S_OK;
}

CEffectMuzzle* CEffectMuzzle::Create(_Device pDevice)
{
    CEffectMuzzle* pInstance = new CEffectMuzzle(pDevice);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
    }

    return pInstance;
}

Engine::CGameObject* CEffectMuzzle::Clone(void* pArg)
{
    CEffectMuzzle* pClone = new CEffectMuzzle(*this);

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

void CEffectMuzzle::Free()
{
    CBaseEffect::Free();
}