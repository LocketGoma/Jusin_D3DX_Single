#include "Light.h"

USING(Engine)

CLight::CLight(_Device pDevice)
	: m_pDevice(pDevice)
	, m_iIndex(0)
{
	ZeroMemory(&m_tLightInfo, sizeof(D3DLIGHT9));
	Safe_AddReference(m_pDevice);
}
CLight::CLight(const CLight& other)
	: m_pDevice(other.m_pDevice)
	, m_iIndex(other.m_iIndex)
	, m_tLightInfo(other.m_tLightInfo)
{
	Safe_AddReference(m_pDevice);
}


HRESULT CLight::Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
	memcpy(&m_tLightInfo, pLightInfo, sizeof(D3DLIGHT9));

	m_iIndex = iIndex;

	m_pDevice->SetLight(iIndex, pLightInfo);
	m_pDevice->LightEnable(iIndex, TRUE);


	return S_OK;
}

CLight* CLight::Create(_Device pDevice, const D3DLIGHT9* pLightInfo, const _uint& iLight)
{
	CLight* pInstance = new CLight(pDevice);

	if (FAILED(pInstance->Ready_Light(pLightInfo, iLight)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CLight::Clone(void* pArg)
{
	return new CLight(*this);
}

void CLight::Free(void)
{
	Safe_Release(m_pDevice);
}
