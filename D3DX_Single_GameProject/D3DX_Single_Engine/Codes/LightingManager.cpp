#include "LightingManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CLightingManager)

CLightingManager::CLightingManager()
{
}

HRESULT CLightingManager::Ready_Light(_Device pDevice, const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
    CLight* pLight = CLight::Create(pDevice, pLightInfo, iIndex);

    NULL_CHECK_RETURN(pLight, E_FAIL);

    m_LightVector.push_back(pLight);

    return S_OK;
}

HRESULT CLightingManager::Set_Light(const _uint& iIndex, _bool bSetType)
{
    if (iIndex >= m_LightVector.size())
    {
        return E_FAIL;
    }

  
    return S_OK;
}

void CLightingManager::Free(void)
{
    for (auto& pLight : m_LightVector)
    {
        Safe_Release(pLight);
    }
    m_LightVector.clear();
    m_LightVector.shrink_to_fit();
}
