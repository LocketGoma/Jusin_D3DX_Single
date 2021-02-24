#include "framework.h"
#include "TestStage.h"

#include "Player.h"

CTestStage::CTestStage(_Device pDevice)
    : Engine::CScene(pDevice)
{
}

HRESULT CTestStage::Ready_Scene(void)
{
    FAILED_CHECK_RETURN(Ready_Resource(m_pDevice), E_FAIL);

    SetWindowText(g_hWnd, L"Test Stage");

    Add_Player_Layer(L"PlayerLayer");

    return S_OK;
}

_int CTestStage::Update_Scene(const _float& fTimeDelta)
{
    D3DXMatrixLookAtLH(&m_matView, &_vec3(0.f, 0.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f));
    D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);

    m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
    m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

    CScene::Update_Scene(fTimeDelta);

    return _int();
}

_int CTestStage::LateUpdate_Scene(const _float& fTimeDelta)
{
    CScene::LateUpdate_Scene(fTimeDelta);

    return _int();
}

void CTestStage::Render_Scene(void)
{
    CScene::Render_Scene();
}

HRESULT CTestStage::Ready_Resource(_Device& m_pDevice)
{
    return S_OK;
}

HRESULT CTestStage::Add_Player_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    Engine::CGameObject* pGameObject = nullptr;

    pGameObject = CPlayer::Create(m_pDevice);
    pLayer->Add_GameObject(L"TestPlayer", pGameObject);

    m_mapLayer.emplace(pLayerTag, pLayer);

    return S_OK;
}

CTestStage* CTestStage::Create(_Device pDevice)
{
    CTestStage* pInstance = new CTestStage(pDevice);

    if (FAILED(pInstance->Ready_Scene()))
       Safe_Release(pInstance);

    return pInstance;
}

void CTestStage::Free()
{
    Engine::CScene::Free();
}
