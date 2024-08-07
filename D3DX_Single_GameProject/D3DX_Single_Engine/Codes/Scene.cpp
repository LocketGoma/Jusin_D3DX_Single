#include "Scene.h"

USING(Engine)

CScene::CScene(_Device pDevice)
    : m_pDevice(pDevice)
    , m_bChangeScene(false)
{
    m_pDevice->AddRef();
}

CScene::~CScene()
{
}

HRESULT CScene::Ready_Scene(void)
{
    return S_OK;
}

_int CScene::Update_Scene(const _float& fDeltaTime)
{
    for (auto& iter : m_mapLayer)
    {
        iter.second->Update_Layer(fDeltaTime);
    }


    return 0;
}

_int CScene::LateUpdate_Scene(const _float& fDeltaTime)
{
    for (auto& iter : m_mapLayer)
    {
        iter.second->LateUpdate_Layer(fDeltaTime);
    }

    return 0;
}

void CScene::Render_Scene(void)
{
    //for (auto& iter : m_mapLayer)
    //{
    //    iter.second->Render_Layer();
    //}
}

std::map<const std::wstring, CLayer*>* CScene::Get_LayerList()
{
    return &m_mapLayer;
}

CLayer* CScene::Get_Layer(const std::wstring& pLayerName)
{
    auto& iter = m_mapLayer.find(pLayerName);

    if (iter == m_mapLayer.end())
    {
        return nullptr;
    }

    return iter->second;
}

CGameObject* CScene::Find_GameObject(const std::wstring& pLayerName, const std::wstring& pObjectTag)
{
    auto& iter = m_mapLayer.find(pLayerName);

    if (iter == m_mapLayer.end())
    {
        return nullptr;
    }
    return iter->second->Find_GameObject(pObjectTag.c_str());
}

void CScene::Free(void)
{
    for (auto& layer_iter : m_mapLayer)
    {
        layer_iter.second->Release();
    }

    for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
    m_mapLayer.clear();

    Safe_Release(m_pDevice);
}
