#include "..\Headers\GraphicResourceManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGraphicResourceManager)

CGraphicResourceManager::CGraphicResourceManager()
    : m_pmapResources(nullptr)
    , m_dwContainerIdx(0)
{
}

CGraphicResourceManager::~CGraphicResourceManager()
{
    Free();
}

HRESULT CGraphicResourceManager::Reserve_Size(const _uint& wSize)
{
    if (nullptr != m_pmapResources)
        return E_FAIL;

    m_pmapResources = new MAPRESOURCES[wSize];

    m_dwContainerIdx = (_ulong)wSize;

    return S_OK;
}

HRESULT CGraphicResourceManager::Ready_Buffer(_Device pDevice, const _uint& iIndex, const _tchar* pBufferTag, BUFFERID eID, const _ulong& dwCountX, const _ulong& dwCountZ, const _ulong& dwVTXInterval)
{
    
    if (m_pmapResources == nullptr)
    {
        PRINT_LOG(L"FATAL ERROR", L"Resource Container not Reserved!");
        return E_FAIL;
    }

    //이미 준비된 리소스인가 검사
    CGraphicResources* pResource = Find_Resources(iIndex,pBufferTag);

    if (pResource != nullptr)
    {
        TCHAR msg[256] = L"";
        TCHAR Omsg[] = L"Buffer < %s > is already Exist";

        swprintf_s(msg, Omsg, pBufferTag);

        PRINT_LOG(L"Warring", msg);

        return S_OK;
    }

    //들어온 스위치 타입에 맞게 생성
    switch (eID)
    {
    case BUFFERID::BUFFER_TRICOL:
        pResource = CVTXTriColor::Create(pDevice);
        break;

    case BUFFERID::BUFFER_RCTEX:
       //
        break;
    }

    NULL_CHECK_RETURN(pResource, E_FAIL);

    m_pmapResources[iIndex].emplace(pBufferTag, pResource);

    return S_OK;
}

void CGraphicResourceManager::Render_Buffer(const _uint& iIndex, const _tchar* pBufferTag)
{
    auto	iter = find_if(m_pmapResources[iIndex].begin(), m_pmapResources[iIndex].end(), CTag_Finder(pBufferTag));

    if (iter != m_pmapResources[iIndex].end())
    {    
        dynamic_cast<CVIBuffer*>(iter->second)->Render_Buffer();
    }
    else
    {
        TCHAR msg[256] = L"";
        TCHAR Omsg[] = L"Can not Render Buffer < %s > ";

        swprintf_s(msg, Omsg, pBufferTag);

        PRINT_LOG(L"Warring", msg);
    }
}

CComponent* CGraphicResourceManager::Clone_Resource(const _uint& iIndex, const _tchar* pResourceTag)
{
    if (nullptr == m_pmapResources)
        return nullptr;

    CGraphicResources* pPrototype = Find_Resources(iIndex, pResourceTag);

    return pPrototype->Clone();
}

CGraphicResources* CGraphicResourceManager::Find_Resources(const _uint& iIndex, const _tchar* pResourceTag)
{
    auto	iter = find_if(m_pmapResources[iIndex].begin(), m_pmapResources[iIndex].end(), CTag_Finder(pResourceTag));

    if (iter == m_pmapResources[iIndex].end())
    {
        return nullptr;
    }

    return iter->second;
}

void CGraphicResourceManager::Free(void)
{
    for (_ulong i = 0; i < m_dwContainerIdx; ++i)
    {
        for_each(m_pmapResources[i].begin(), m_pmapResources[i].end(), CDeleteMap());
        m_pmapResources[i].clear();
    }

    Safe_Delete_Array(m_pmapResources);
}
