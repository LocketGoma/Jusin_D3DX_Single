#include "framework.h"
#include "TestLight.h"

CTestLight::CTestLight(_Device pDevice)
    : Engine::CGameObject(pDevice)
{
}

HRESULT CTestLight::Ready_GameObject()
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

    //��� ����� �����༭ �׷���
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

void CTestLight::Free()
{
    Engine::CGameObject::Free();
}