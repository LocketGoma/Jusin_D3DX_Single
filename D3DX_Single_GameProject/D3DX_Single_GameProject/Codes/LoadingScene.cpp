#include "framework.h"
#include "LoadingScene.h"

CLoadingScene::CLoadingScene(_Device pDevice, ESceneType eSceneID)
    :CScene(pDevice)
    ,m_hLoadingThread(NULL)
    ,m_pCritSection()
    ,m_bLoadFinished(false)
{
    Safe_AddReference(pDevice);
}

HRESULT CLoadingScene::Ready_Scene(void)
{
    m_hLoadingThread = (HANDLE)_beginthreadex(0, 0, LoadingByThread, this, 0, nullptr);
    if (m_hLoadingThread == nullptr)
    {
        return E_FAIL;
    }
    InitializeCriticalSection(&m_pCritSection);


    return S_OK;
}

_int CLoadingScene::Update_Scene(const _float& fTimeDelta)
{
    return _int();
}

_int CLoadingScene::LateUpdate_Scene(const _float& fTimeDelta)
{
    return _int();
}

void CLoadingScene::Render_Scene(void)
{
}

unsigned _stdcall CLoadingScene::LoadingByThread(void* pParam)
{
    //다이나믹 캐스트 쓰면 에러나구나;
    CLoadingScene* pLoading = (CLoadingScene *)(pParam);
    if (pLoading == nullptr)
    {
        return 0;
    }
    //크리티컬 섹션 진입
    EnterCriticalSection(&pLoading->m_pCritSection);


    return 0;
}

_bool CLoadingScene::IsFinished()
{
    return m_bLoadFinished;
}

CLoadingScene* CLoadingScene::Create(_Device pDevice, ESceneType eSceneID)
{
    CLoadingScene* pInstance = new CLoadingScene(pDevice, eSceneID);
    if (FAILED(pInstance->Ready_Scene()))
    {
        PRINT_LOG(L"Error", L"Failed To Create CLoadingScene");
        Safe_Release(pInstance);
    }

    return pInstance;
}

HRESULT CLoadingScene::Load_Base_Resource()
{
    return E_NOTIMPL;
}

HRESULT CLoadingScene::Load_Stage1_Resource()
{
    return E_NOTIMPL;
}

void CLoadingScene::Free()
{
}