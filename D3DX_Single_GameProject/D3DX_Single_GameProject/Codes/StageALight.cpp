#include "framework.h"
#include "StageALight.h"

CStageALight::CStageALight(_Device pDevice)
	: CGameObject(pDevice)
{
}

CStageALight::CStageALight(const CStageALight& other)
	: CGameObject(other)
{
}

HRESULT CStageALight::Ready_GameObject()
{
	return S_OK;
}

HRESULT CStageALight::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Setup_Light();

	return S_OK;
}

_int CStageALight::Update_GameObject(const _float& fDeltaTime)
{
	return _int();
}

_int CStageALight::LateUpdate_GameObject(const _float& fDeltaTime)
{
	return _int();
}

HRESULT CStageALight::Render_GameObject()
{
	return S_OK;
}

void CStageALight::Set_Position(_vec3 vPos)
{
}

void CStageALight::Set_Size(_vec3 vSize)
{
}

_vec3 CStageALight::Get_Position()
{
	return _vec3();
}

_vec3 CStageALight::Get_Size()
{
	return _vec3();
}

HRESULT CStageALight::Add_Component(void)
{
	return S_OK;
}

HRESULT CStageALight::Setup_Light()
{
	return E_NOTIMPL;
}

CStageALight* CStageALight::Create(_Device pDevice)
{
	CStageALight* pInstance = new CStageALight(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CGameObject* CStageALight::Clone(void* pArg)
{
	CStageALight* pClone = new CStageALight(*this);
	if (FAILED(pClone->Ready_GameObject_Clone()))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CStageALight");
		Safe_Release(pClone);
	}

	return pClone;
}

void CStageALight::Free()
{
	Engine::CGameObject::Free();
}
