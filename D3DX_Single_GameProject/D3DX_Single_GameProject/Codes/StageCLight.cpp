#include "framework.h"
#include "StageCLight.h"


CStageCLight::CStageCLight(_Device pDevice)
	: CGameObject(pDevice)
{
}

CStageCLight::CStageCLight(const CStageCLight& other)
	: CGameObject(other)
{
}

HRESULT CStageCLight::Ready_GameObject()
{
	return S_OK;
}

HRESULT CStageCLight::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Setup_Light();

	return S_OK;
}

_int CStageCLight::Update_GameObject(const _float& fDeltaTime)
{
	return _int();
}

_int CStageCLight::LateUpdate_GameObject(const _float& fDeltaTime)
{
	return _int();
}

HRESULT CStageCLight::Render_GameObject()
{
	return S_OK;
}


void CStageCLight::Set_Position(_vec3 vPos)
{
}

void CStageCLight::Set_Size(_vec3 vSize)
{
}

_vec3 CStageCLight::Get_Position()
{
	return _vec3();
}

_vec3 CStageCLight::Get_Size()
{
	return _vec3();
}

HRESULT CStageCLight::Add_Component(void)
{
	return S_OK;
}

HRESULT CStageCLight::Setup_Light()
{
	D3DLIGHT9 light;

	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;	
	light.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	light.Specular = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.6f);
	light.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f);
	light.Direction = D3DXVECTOR3(-5.0f, -10.0f, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, (D3DXVECTOR3*)&light.Direction);

	//요걸 제대로 안해줘서 그랬음
	m_pDevice->SetLight(0, &light);
	m_pDevice->LightEnable(0, TRUE);

	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	light.Specular = D3DXCOLOR(0.6f, 0.6f, 0.6f, 0.6f);
	light.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f);
	light.Direction = D3DXVECTOR3(5.0f, -10.0f, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, (D3DXVECTOR3*)&light.Direction);

	//요걸 제대로 안해줘서 그랬음
	m_pDevice->SetLight(1, &light);
	m_pDevice->LightEnable(1, TRUE);




	return S_OK;
}

CStageCLight* CStageCLight::Create(_Device pDevice)
{
	CStageCLight* pInstance = new CStageCLight(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CGameObject* CStageCLight::Clone(void* pArg)
{
	CStageCLight* pClone = new CStageCLight(*this);
	if (FAILED(pClone->Ready_GameObject_Clone()))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CStageCLight");
		Safe_Release(pClone);
	}

	return pClone;
}

void CStageCLight::Free()
{
	Engine::CGameObject::Free();
}

