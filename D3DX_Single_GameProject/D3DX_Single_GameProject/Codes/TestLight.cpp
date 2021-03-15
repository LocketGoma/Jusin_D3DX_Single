#include "framework.h"
#include "TestLight.h"

CTestLight::CTestLight(_Device pDevice)
    : Engine::CGameObject(pDevice)
{
}

CTestLight::CTestLight(const CTestLight& other)
    : Engine::CGameObject(other)
{
}

HRESULT CTestLight::Ready_GameObject()
{
    return S_OK;
}

HRESULT CTestLight::Ready_GameObject_Clone(void* pArg)
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    Setup_Light();

    return S_OK;
}

_int CTestLight::Update_GameObject(const _float& fDeltaTime)
{
    return _int();
}

_int CTestLight::LateUpdate_GameObject(const _float& fDeltaTime)
{
    return _int();
}

HRESULT CTestLight::Render_GameObject()
{
    return S_OK;
}

void CTestLight::Set_Position(_vec3 vPos)
{
}

void CTestLight::Set_Size(_vec3 vSize)
{
}

_vec3 CTestLight::Get_Position()
{
    return _vec3();
}

_vec3 CTestLight::Get_Size()
{
    return _vec3();
}

HRESULT CTestLight::Add_Component(void)
{
    return S_OK;
}

HRESULT CTestLight::Setup_Light()
{
    D3DLIGHT9 light;

    ZeroMemory(&light, sizeof(D3DLIGHT9));
    light.Type = D3DLIGHT_DIRECTIONAL;
    //light.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
    light.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    light.Specular = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.6f);
    light.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.4f);
    light.Direction = D3DXVECTOR3(1.0f, -10.0f, 0.0f);
    D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, (D3DXVECTOR3*)&light.Direction);

    //요걸 제대로 안해줘서 그랬음
    m_pDevice->SetLight(0, &light);
    m_pDevice->LightEnable(0, TRUE);

    return S_OK;
}

CTestLight* CTestLight::Create(_Device pDevice)
{
    CTestLight* pInstance = new CTestLight(pDevice);

    if (FAILED(pInstance->Ready_GameObject()))
        Safe_Release(pInstance);

    return pInstance;
}

Engine::CGameObject* CTestLight::Clone(void* pArg)
{
    CTestLight* pClone = new CTestLight(*this);
    if (FAILED(pClone->Ready_GameObject_Clone()))
    {
        PRINT_LOG(L"Error", L"Failed To Clone CTestLight");
        Safe_Release(pClone);
    }

    return pClone;
}

void CTestLight::Free()
{
    Engine::CGameObject::Free();
}
