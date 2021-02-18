#include "framework.h"
#include "TestStage.h"

CTestStage::CTestStage(_Device pDevice)
    : Engine::CScene(pDevice)
{
}

HRESULT CTestStage::Ready_Scene(void)
{
    FAILED_CHECK_RETURN(Ready_Resource(m_pDevice), E_FAIL);

    SetWindowText(g_hWnd, L"Test Stage");

    return S_OK;
}

_int CTestStage::Update_Scene(const _float& fTimeDelta)
{
    return _int();
}

_int CTestStage::LateUpdate_Scene(const _float& fTimeDelta)
{
    return _int();
}

void CTestStage::Render_Scene(void)
{
}

HRESULT CTestStage::Ready_Resource(_Device& m_pDevice)
{
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
