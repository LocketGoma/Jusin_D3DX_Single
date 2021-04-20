#include "framework.h"
#include "GameEndImage.h"

#include "Texture.h"
#include "Transform.h"
#include "VTXRectTexture.h"
#include "Shader.h"
CGameEndImage::CGameEndImage(_Device pDevice)
    : CGameObject(pDevice)
    , m_fFadeAmount(0.f)
{
}

HRESULT CGameEndImage::Ready_GameObject(void)
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pTransformCom->Set_Scale(_vec3(16.f * 0.6f, 9.f * 0.6f, 1.f));
    m_pTransformCom->Set_Pos(_vec3(0.f,0.f, 0.0));

    return S_OK;
}

_int CGameEndImage::Update_GameObject(const _float& fDeltaTime)
{
    Engine::CGameObject::Update_GameObject(fDeltaTime* g_fEndingTimeDelay);

    return NO_EVENT;
}

_int CGameEndImage::LateUpdate_GameObject(const _float& fDeltaTime)
{
    Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
    if (nullptr == pManagement)
    {
        return MANAGER_OUT;
    }
    pManagement->Add_RenderList(Engine::RENDERID::RENDER_FINAL_ALPHA, this);

    m_fFadeAmount += ((fDeltaTime*g_fEndingTimeDelay) / 15.f);
    //m_fFadeAmount += (fDeltaTime* g_fEndingTimeDelay )*180.f;

    pManagement->Set_FinalTrigger(g_bEndingTimeDelay);
    pManagement->Set_FinalTimer(m_fFadeAmount>1.f?1.f: m_fFadeAmount);

    return NO_EVENT;
}

HRESULT CGameEndImage::Render_GameObject(void)
{
    m_pTransformCom->LateUpdate_Component();
    //직교투영
    _mat matWorld, matView, matProj, matOrtho , matOriView, matOriProj;
    _vec3 vScale = _vec3(200.f, 200.f, 0.f);

    m_pDevice->GetTransform(D3DTS_VIEW, &matOriView);
    m_pDevice->GetTransform(D3DTS_PROJECTION, &matOriProj);

    D3DXMatrixIdentity(&matWorld);
    D3DXMatrixIdentity(&matView);
    D3DXMatrixIdentity(&matProj);

    D3DXMatrixOrthoLH(
        &matOrtho, /* 직교투영행렬 반환 */
        WINCX, // 가로폭 (WINCX)
        WINCY, // 세로폭 (WINCY)
        0, /* Near Z: 관찰자와 가장 가까운 면과의 Z거리 */
        1 /* Far Z: 관찰자와 가장 먼 편과의 Z거리 */);

    matProj = matOrtho;


    matView._11 = vScale.x;
    matView._22 = vScale.y;
    matView._33 = vScale.z;

    matView._41 = 0.f;
    matView._42 = 0.f;
    matView._43 = 0.f;

    m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
    m_pDevice->SetTransform(D3DTS_VIEW, &matView);
    m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
    ///



    if (FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
    {
        return E_FAIL;
    }


    if (FAILED(CGameObject::Render_GameObject()))
    {
        return E_FAIL;
    }
    //쉐이더 처리
    LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
    NULL_CHECK_RETURN(pEffect, E_FAIL);
    pEffect->AddRef();
    
    _uint	iPassMax = 0;

    FAILED_CHECK_RETURN(Setup_ConstantTable(pEffect), E_FAIL);

    pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
    pEffect->BeginPass(0);

   // pEffect->SetFloat("g_FadeAmount", sin(D3DXToRadian(m_fFadeAmount)));
    pEffect->SetFloat("g_FadeAmount", m_fFadeAmount);
    pEffect->SetTexture("g_BaseTexture", m_pTextureCom->Get_Texture());
    pEffect->CommitChanges();
    if (FAILED(m_pBufferCom->Render_Buffer()))
    {
        return E_FAIL;
    }

    pEffect->EndPass();
    pEffect->End();

    Safe_Release(pEffect);

    //쉐이더 처리 끝

    if (FAILED(m_pTextureCom->Set_Texture()))
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
    {
        return E_FAIL;
    }


    m_pDevice->SetTransform(D3DTS_VIEW, &matOriView);
    m_pDevice->SetTransform(D3DTS_PROJECTION, &matOriProj);


    return S_OK;
}

HRESULT CGameEndImage::Add_Component(void)
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
    m_mapComponent[0].emplace(L"Com_Buffer", pComponent);

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[0].emplace(L"Com_Transform", pComponent);

    // Texture
    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_End"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[0].emplace(L"Com_Texture", pComponent);

    //쉐이더
    pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(pManagement->Clone_Prototype(L"Shader_For_End"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Shader", pComponent);

    return S_OK;

}

HRESULT CGameEndImage::Setup_ConstantTable(LPD3DXEFFECT& pEffect)
{

    if (pEffect == nullptr)
    {
        return E_FAIL;
    }

    _matrix			matWorld, matView, matProj;

    matWorld = m_pTransformCom->Get_TransformDescription().matWorld;
    m_pDevice->GetTransform(D3DTS_VIEW, &matView);
    m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

    pEffect->SetMatrix("g_matWorld", &matWorld);
    pEffect->SetMatrix("g_matView", &matView);
    pEffect->SetMatrix("g_matProj", &matProj);

    return S_OK;
}


void CGameEndImage::Free()
{
    Safe_Release(m_pBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);

    CGameObject::Free();
}

CGameEndImage* CGameEndImage::Create(_Device pDevice)
{
    CGameEndImage* pInstance = new CGameEndImage(pDevice);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
    }

    return pInstance;
}

Engine::CGameObject* CGameEndImage::Clone(void* pArg)
{
    CGameEndImage* pClone = new CGameEndImage(*this);

    if (pClone == nullptr)
    {
        return nullptr;
    }

    return pClone;
}

void CGameEndImage::Set_Position(_vec3 vPos)
{
}

void CGameEndImage::Set_Size(_vec3 vSize)
{
}

_vec3 CGameEndImage::Get_Position()
{
    return _vec3();
}

_vec3 CGameEndImage::Get_Size()
{
    return _vec3();
}
