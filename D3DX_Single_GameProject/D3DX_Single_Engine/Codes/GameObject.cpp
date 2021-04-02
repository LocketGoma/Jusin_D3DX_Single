#include "GameObject.h"

USING(Engine)

CGameObject::CGameObject(_Device pDevice)
    : m_pDevice(pDevice)
    , m_bDead(false)
{
    m_bIsPrototype = true;
    m_pDevice->AddRef();
}

CGameObject::CGameObject(const CGameObject& other)
    : m_pDevice(other.m_pDevice)
    , m_bDead(other.m_bDead)
{
    m_bIsPrototype = false;
    Safe_AddReference(m_pDevice);
}

_float CGameObject::Get_ViewZ()
{
    return m_fViewZ;
}


CComponent* CGameObject::Get_Component(const std::wstring& pComponentTag, Engine::COMPONENT_ID eID)
{
    CComponent* pComponent = Find_Component(pComponentTag, eID);

    if (nullptr == pComponent)
    {
        return nullptr;
    }

    return pComponent;
}

CComponent* CGameObject::Find_Component(const std::wstring& pComponentTag, Engine::COMPONENT_ID eID)
{
    auto iter = m_mapComponent[(_uint)eID].find(pComponentTag);

    if (iter == m_mapComponent[(_uint)eID].end())
    {
        return nullptr;
    }

    return iter->second;
}


HRESULT CGameObject::Ready_GameObject()
{
    return S_OK;
}

HRESULT CGameObject::Ready_GameObject_Clone(void* pArg)
{
    return S_OK;
}

_int CGameObject::Update_GameObject(const _float& fDeltaTime)
{
    for (auto& iter : m_mapComponent[(_uint)(COMPONENT_ID::ID_DYNAMIC)])
    {
        iter.second->Update_Component(fDeltaTime);
    }

    return 0;
}

_int CGameObject::LateUpdate_GameObject(const _float& fDeltaTime)
{
    for (auto& iter : m_mapComponent[(_uint)(COMPONENT_ID::ID_DYNAMIC)])
    {
        iter.second->LateUpdate_Component(fDeltaTime);
    }

    return 0;
}

HRESULT CGameObject::Render_GameObject()
{
    return S_OK;
}

void CGameObject::Set_Dead()
{
    m_bDead = true;
}

bool CGameObject::Is_Dead()
{
    return m_bDead;
}

void CGameObject::Free(void)
{
    for (_uint i = 0; i < (_uint)(COMPONENT_ID::ID_END); ++i)
    {
        for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
        m_mapComponent[i].clear();
    }

    Safe_Release(m_pDevice);
}
