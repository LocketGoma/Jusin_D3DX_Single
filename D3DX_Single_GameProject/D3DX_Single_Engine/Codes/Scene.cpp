#include "Scene.h"

USING(Engine)

CScene::CScene(_Device pDevice)
    : m_pDevice(pDevice)
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

_int CScene::Update_Scene(const _float& fTimeDelta)
{
    for (auto& iter : m_mapLayer)
    {
        iter.second->Update_Layer(fTimeDelta);
    }


    return 0;
}

_int CScene::LateUpdate_Scene(const _float& fTimeDelta)
{
    for (auto& iter : m_mapLayer)
    {
        iter.second->LateUpdate_Layer(fTimeDelta);
    }

    return 0;
}

void CScene::Render_Scene(void)
{
    for (auto& iter : m_mapLayer)
    {
        iter.second->Render_Layer();
    }



}

void CScene::Free(void)
{
    for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
    m_mapLayer.clear();

    Safe_Release(m_pDevice);
}
