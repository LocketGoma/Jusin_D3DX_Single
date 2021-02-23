#include "framework.h"
#include "MainGame.h"

#include "TestStage.h"

CMainGame::CMainGame()
    : m_pManagement(Engine::CManagement::Get_Instance())
    , pScene(nullptr)
{
    Safe_AddReference(m_pManagement);
}

CMainGame::~CMainGame()
{
}

HRESULT CMainGame::Ready_MainGame()
{
    if (m_pManagement == nullptr)
    {
        return E_FAIL;
    }

    if (FAILED(m_pManagement->Ready_Engine(g_hWnd, WINCX, WINCY, Engine::WINMODE::MODE_WIN, 1)))
    {
        PRINT_LOG(L"Error", L"Failed To Ready_Engine in Client");
        return E_FAIL;
    }

    m_pDevice = m_pManagement->Get_Device();

    NULL_CHECK_RETURN_MSG(m_pDevice, E_FAIL, L"Device is Nullptr. FATAL ERROR");

    Safe_AddReference(m_pDevice);

    //Engine::CScene* pScene = nullptr;
    pScene = CTestStage::Create(m_pDevice);    

    return S_OK;
}

_int CMainGame::Update_MainGame()
{
    _float fDeltaTime = m_pManagement->Get_DeltaTime();

    m_pManagement->Update_Engine(fDeltaTime);
    m_pManagement->LateUpdate_Engine(fDeltaTime);

    return _int();
}

void CMainGame::Render_MainGame()
{
    if (FAILED(m_pManagement->Render_Engine(g_hWnd)))
    {
        PRINT_LOG(L"FATAL ERROR", L"RENDER ENGINE FAIL");
        return;
    }

}

HRESULT CMainGame::Setup_DefaultSetting()
{
    if (FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
        return E_FAIL;

    if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW)))
        return E_FAIL;


    return S_OK;
}

HRESULT CMainGame::Ready_Scene()
{
    

    return S_OK;
}

CMainGame* CMainGame::Create()
{
    CMainGame* pInstance = new CMainGame;
    if (FAILED(pInstance->Ready_MainGame()))
    {
        PRINT_LOG(L"Error", L"Failed To Create MainApp");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMainGame::Free()
{
    Safe_Release(pScene);

    Safe_Release(m_pDevice);
    Safe_Release(m_pManagement);
}
