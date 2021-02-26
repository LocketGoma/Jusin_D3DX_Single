#include "LightingManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CLightingManager)

CLightingManager::CLightingManager()
{
}


HRESULT CLightingManager::Ready_Light()
{
    return E_NOTIMPL;
}

HRESULT CLightingManager::Set_Light(const _uint& iIndex, _bool bSetType)
{
    return E_NOTIMPL;
}

void CLightingManager::Free(void)
{
}
