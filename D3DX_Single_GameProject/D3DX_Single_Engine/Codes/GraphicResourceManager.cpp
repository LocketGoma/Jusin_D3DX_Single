#include "GraphicResourceManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGraphicResourceManager)

CGraphicResourceManager::CGraphicResourceManager()
    : m_pmapResources(nullptr)
    , m_dwContainerIdx(0)
{
    Reserve_Size(10);
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
        pResource = CVTXRectTexture::Create(pDevice);       
        break;
    case BUFFERID::BUFFER_CUBETEX:
        pResource = CVTXCubeTexture::Create(pDevice);
        break;
    case BUFFERID::BUFFER_CUBECOLOR_N:
        pResource = CVTXCubeColor::Create(pDevice);
        break;
    case BUFFERID::BUFFER_TERRAINTEX:
        pResource = CVTXTerrain::Create(pDevice, dwCountX, dwCountZ, dwVTXInterval);
        break;
    }

    NULL_CHECK_RETURN(pResource, E_FAIL);

    m_pmapResources[iIndex].emplace(pBufferTag, pResource);

    return S_OK;
}

HRESULT CGraphicResourceManager::Ready_Texture(_Device pDevice, const _ushort& wContainerIdx, const _tchar* pTextureTag, TEXTYPE eType, const _tchar* pPath, const _uint& iCount)
{
    NULL_CHECK_RETURN(m_pmapResources, E_FAIL);

    CGraphicResources* pResource = Find_Resources(wContainerIdx, pTextureTag);
    if (pResource != nullptr)
    {
        PRINT_LOG(L"Warring", L"Texture already Exist!");
        return S_OK;
    }

    pResource = CTexture::Create(pDevice, pPath, eType, iCount);
    NULL_CHECK_RETURN(pResource, E_FAIL);

    m_pmapResources[wContainerIdx].emplace(pTextureTag, pResource);

    return S_OK;
}

HRESULT CGraphicResourceManager::Ready_Meshes(_Device pDevice, const _ushort& wContainerIdx, const _tchar* pMeshTag, MESHTYPE eType, const _tchar* pFilePath, const _tchar* pFileName)
{
    if (m_pmapResources == nullptr)
    {
        PRINT_LOG(L"FATAL ERROR", L"Resource Container not Reserved!");
        return E_FAIL;
    }

    //이미 준비된 리소스인가 검사
    CGraphicResources* pResource = Find_Resources(wContainerIdx, pMeshTag);

    if (pResource != nullptr)
    {
        TCHAR msg[256] = L"";
        TCHAR Omsg[] = L"Mesh < %s > is already Exist";

        swprintf_s(msg, Omsg, pMeshTag);

        PRINT_LOG(L"Warring", msg);

        return S_OK;
    }

    switch (eType)
    {
    case Engine::MESHTYPE::MESH_STATIC:
        pResource = CStaticMesh::Create(pDevice,pFilePath,pFileName);
        break;
    case Engine::MESHTYPE::MESH_DYNAMIC:
        break;
    case Engine::MESHTYPE::MESH_NAVI:
        break;
    case Engine::MESHTYPE::MESH_END:
        break;
    default:
        break;
    }

    m_pmapResources[wContainerIdx].emplace(pMeshTag, pResource);

    return S_OK;
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
