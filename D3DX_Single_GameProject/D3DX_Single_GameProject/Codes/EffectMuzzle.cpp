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

    //if (m_bIsVisible)
    pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);

    return NO_EVENT;
}

HRESULT CEffectMuzzle::Render_GameObject(void)
{
    _mat matFWorld, matScale, matWorld, matParentBoneReduce, matParentBone, matParentTrans;
    _mat matView, matBill;

    m_pDevice->GetTransform(D3DTS_VIEW, &matWorld);
    m_pDevice->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixInverse(&matWorld,NULL,&matWorld);

    if (m_pTargetMesh != nullptr)
    {
        const Engine::D3DXFRAME_DERIVED* pFrame = m_pTargetMesh->Get_FrameByName("ValveBiped_square");
        matParentBone = pFrame->CombinedTranformationMatrix;
    }

    D3DXMatrixIdentity(&matBill);

    matBill._11 = matView._11;
    matBill._13 = matView._13;
    matBill._31 = matView._31;
    matBill._33 = matView._33;




    D3DXMatrixInverse(&matBill, NULL, &matBill);
    _vec3 vPos = _vec3(0.f,0.f,0.f);
    D3DXVec3TransformCoord(&vPos, &vPos, &matUpParent);
    _mat matPosition;
    D3DXMatrixTranslation(&matPosition, vPos.x, vPos.y, vPos.z);
    
    matFWorld = matBill * matWorld * matParentBone * matParent;
      
    matFWorld._11 = 0.1;
    matFWorld._22 = 0.1;
    matFWorld._33 = 0.1;

    //m_pTransformCom->Set_WorldMatrix(matFWorld);

    m_pDevice->SetTransform(D3DTS_WORLD, &matFWorld);

    //m_pTransformCom->LateUpdate_Component();
    
    if (FAILED(CGameObject::Render_GameObject()))
    {
        return E_FAIL;
    }

    if (FAILED(m_pTextureCom->Set_Texture(rand() % MUZZLE_EFFECT_COUNT)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pBufferCom->Render_Buffer()))
    {
        return E_FAIL;
    }


	return S_OK;
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