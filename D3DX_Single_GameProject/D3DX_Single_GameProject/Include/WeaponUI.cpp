#include "framework.h"
#include "WeaponUI.h"



CWeaponUI::CWeaponUI(_Device pDevice)
    : CBaseEffect(pDevice)
    , m_iWeaponNumber((_uint)eWeaponType::WEAPON_CROWBAR)
    , m_fTimer(0.f)
    , m_fViewInterval(2.f)

{
    ZeroMemory(m_bStatus, sizeof(bool) * (_uint)eWeaponType::WEAPON_END);
}

CWeaponUI::CWeaponUI(const CWeaponUI& other)
    : CBaseEffect(other)
    , m_iWeaponNumber((_uint)eWeaponType::WEAPON_CROWBAR)
    , m_fTimer(0.f)
    , m_fViewInterval(other.m_fViewInterval)
{
    m_bIsVisible = false;
    ZeroMemory(m_bStatus, sizeof(bool) * (_uint)eWeaponType::WEAPON_END);
}

HRESULT CWeaponUI::Ready_GameObject(void)
{
    return S_OK;
}

HRESULT CWeaponUI::Ready_GameObject_Clone(void* pArg)
{
    if (FAILED(Add_Component()))
    {
        return E_FAIL;
    }

    return S_OK;
}

_int CWeaponUI::Update_GameObject(const _float& fDeltaTime)
{



    return NO_EVENT;
}

_int CWeaponUI::LateUpdate_GameObject(const _float& fDeltaTime)
{
    Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
    if (nullptr == pManagement)
    {
        return MANAGER_OUT;
    }
    m_fTimer += fDeltaTime;
    if (m_fTimer > m_fViewInterval)
    {
        m_bIsVisible = false;
    }

    pManagement->Add_RenderList(Engine::RENDERID::RENDER_UI_ALPHA, this);

    return NO_EVENT;
}

HRESULT CWeaponUI::Render_GameObject(void)
{
    if (m_bIsVisible == false)
    {
        return S_OK;
    }

    Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
    if (pManagement == nullptr)
    {
        return E_FAIL;
    }

    Change_To_Ortho();

    if (FAILED(CGameObject::Render_GameObject()))
    {
        return E_FAIL;
    }

    if (FAILED(m_pTextureCom->Set_Texture()))
    {
        return E_FAIL;
    }

    _float m_fStartX = -(WINCX>>1)+(WINCX>>2);
    _float m_fStartY = (WINCY >> 2);
    
    for (_uint i = 0; i < (_uint)eWeaponType::WEAPON_END; i++)
    {
        if (m_iWeaponNumber == i)
        {
            m_fStartX += m_vOpenSize.x/3;
            Set_EffectPosition(m_vOpenSize, _vec3(m_fStartX, m_fStartY-m_vOpenSize.y/3.5f, 0.f));
            m_fStartX += m_vOpenSize.x/1.8f;


        }
        else
        {
            Set_EffectPosition(m_vCloseSize, _vec3(m_fStartX, m_fStartY, 0.f));
            m_fStartX += m_vCloseSize.x;
        }

        m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
        m_pDevice->SetTransform(D3DTS_VIEW, &matView);
        m_pDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);

        if (FAILED(m_pBufferArrCom[i]->Render_Buffer()))
        {
            return E_FAIL;
        }

        if (m_iWeaponNumber == i)
        {
            Print_Weapon(i, _vec2(m_fStartX, m_fStartY));
        }

    }

    Change_To_Proj();

    return S_OK;
}

void CWeaponUI::Set_Ortho(_vec3 vScale, _vec3 vPos)
{
    _mat matWorld, matView, matProj, matOrtho;


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

    matView._41 = vPos.x;
    matView._42 = vPos.y;
    matView._43 = vPos.z;

    m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
    m_pDevice->SetTransform(D3DTS_VIEW, &matView);
    m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CWeaponUI::Set_EffectPosition(_vec3 vScale, _vec3 vPos)
{
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

void CWeaponUI::Select_Weapon(_uint iWeaponNumber)
{
    m_iWeaponNumber = iWeaponNumber;
    m_bIsVisible = true;
    m_fTimer = 0.f;


    Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
    if (pManagement == nullptr)
    {
        return;
    }
    pManagement->Stop_Sound(Engine::SOUND_CHANNELID::UI);
    pManagement->Play_Sound(L"wpn_moveselect.wav", Engine::SOUND_CHANNELID::UI);

}

CWeaponUI* CWeaponUI::Create(_Device pDevice)
{
    CWeaponUI* pInstance = new CWeaponUI(pDevice);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
    }

    return pInstance;
}

Engine::CGameObject* CWeaponUI::Clone(void* pArg)
{
    CWeaponUI* pClone = new CWeaponUI(*this);

    if (pClone == nullptr || FAILED(pClone->Ready_GameObject_Clone()))
    {
        PRINT_LOG(L"Error", L"Failed To Clone CWeaponUI");

        return nullptr;
    }

    return pClone;
}

void CWeaponUI::Change_To_Ortho()
{
    m_pDevice->GetTransform(D3DTS_VIEW, &matOriView);
    m_pDevice->GetTransform(D3DTS_PROJECTION, &matOriProj);

    m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
    m_pDevice->SetTransform(D3DTS_VIEW, &matView);
    m_pDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
}

void CWeaponUI::Change_To_Proj()
{
    m_pDevice->SetTransform(D3DTS_VIEW, &matOriView);
    m_pDevice->SetTransform(D3DTS_PROJECTION, &matOriProj);
}

void CWeaponUI::Print_Weapon(_uint iType, _vec2 vPostion)
{
    Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
    if (pManagement == nullptr)
    {
        return;
    }

    _tchar szWeapon[8] = L"";
    switch ((eWeaponType)iType)
    {
    case eWeaponType::WEAPON_CROWBAR:
        wsprintf(szWeapon, L"c");
        break;
    case eWeaponType::WEAPON_PISTOL:
        wsprintf(szWeapon, L"d");
        break;
    case eWeaponType::WEAPON_SMG:
        wsprintf(szWeapon, L"a");
        break;
    case eWeaponType::WEAPON_RIFLE:
        wsprintf(szWeapon, L"l");
        break;
    case eWeaponType::WEAPON_SHOTGUN:
        wsprintf(szWeapon, L"b");
        break;
    case eWeaponType::WEAPON_PHYCANNON:
        wsprintf(szWeapon, L"m");
        break;
    case eWeaponType::WEAPON_RPG:
        wsprintf(szWeapon, L"i");
        break;
    case eWeaponType::WEAPON_END:
        wsprintf(szWeapon, L"@");
        break;
    default:
        break;
    }

    pManagement->Render_Font(L"Font_UI_Weapon", szWeapon, &_vec2(vPostion.x+(WINCX>>1)-(WINCX>>3) - (WINCX >> 5), (WINCY>>1)-vPostion.y), D3DXCOLOR(0.85f,0.85f,0.2f,0.8f));


}

HRESULT CWeaponUI::Add_Component()
{
    Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
    if (nullptr == pManagement)
    {
        return MANAGER_OUT;
    }

    Engine::CComponent* pComponent = nullptr;
    //Buffer 
    pComponent = m_pBufferCom = dynamic_cast<Engine::CVTXRectTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_RectTex"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Buffer", pComponent);

    for (int i = 0; i < (int)eWeaponType::WEAPON_END; i++)
    {
        TCHAR tObjName[128] = L"";
        TCHAR tObjData[] = L"Com_Buffer %d";
        swprintf_s(tObjName, tObjData, i+1);

        pComponent = m_pBufferArrCom[i] = dynamic_cast<Engine::CVTXRectTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_RectTex"));
        NULL_CHECK_RETURN(pComponent, E_FAIL);
        m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(tObjName, pComponent);
    }

    // Texture
    pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_UI_BOX"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Texture", pComponent);

    return S_OK;
}

void CWeaponUI::Free()
{
    if (m_bIsPrototype == false)
    {
        for (int i = 0; i < (int)eWeaponType::WEAPON_END; i++)
        {
            Safe_Release(m_pBufferArrCom[i]);
        }
    }

    CBaseEffect::Free();
}
