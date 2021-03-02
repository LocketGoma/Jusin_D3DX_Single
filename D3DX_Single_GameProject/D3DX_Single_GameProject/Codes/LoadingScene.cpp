#include "framework.h"
#include "LoadingScene.h"

//로드할 데이터들
#include "Transform.h"
#include "CameraComponent.h"


//로드 데이터 끝


CLoadingScene::CLoadingScene(_Device pDevice, LOADINGID eSceneID)
    :CScene(pDevice)
    ,m_hLoadingThread(NULL)
    ,m_pCritSection()
    ,m_bLoadFinished(false)
    ,m_eNextLoadScene(eSceneID)
{
    Safe_AddReference(pDevice);
}

HRESULT CLoadingScene::Ready_Scene(void)
{
    InitializeCriticalSection(&m_pCritSection);

    m_hLoadingThread = (HANDLE)_beginthreadex(NULL, 0, LoadingByThread, this, 0, nullptr);

    if (m_hLoadingThread == nullptr)
    {
        return E_FAIL;
    }

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

LOADINGID CLoadingScene::Get_LoadingID() const
{
    return m_eNextLoadScene;
}

CRITICAL_SECTION* CLoadingScene::Get_CriticalSection()
{
    return &m_pCritSection;
}

const _tchar* CLoadingScene::Get_String() const
{
    return m_szString;
}

HRESULT CLoadingScene::Ready_Loading(LOADINGID eLoadingID)
{
    InitializeCriticalSection(&m_pCritSection);

    m_hLoadingThread = (HANDLE)_beginthreadex(NULL, 0, LoadingByThread, this, 0, NULL);

    m_eNextLoadScene = eLoadingID;
    
    return S_OK;
}

unsigned __stdcall CLoadingScene::LoadingByThread(void* pParam)
{
    //다이나믹 캐스트 쓰면 에러나구나;
    CLoadingScene* pLoading = (CLoadingScene *)(pParam);
    if (pLoading == nullptr)
    {
        return E_FAIL;
    }
    //크리티컬 섹션 진입

    _uint iFlag = 0;

    EnterCriticalSection(pLoading->Get_CriticalSection());

    switch (pLoading->Get_LoadingID())
    {
    case LOADINGID::LOADING_TEST :
        iFlag = pLoading->Load_Base_Resource();
        break;

    case LOADINGID::LOADING_STAGE1:
        iFlag = pLoading->Load_Stage1_Resource();
        break;

    case LOADINGID::LOADING_STAGE2:
        break;

    case LOADINGID::LOADING_BOSS:
        break;

    default:
        break;
    }

    LeaveCriticalSection(pLoading->Get_CriticalSection());
    _endthreadex(0);

    return iFlag;
}

_bool CLoadingScene::IsFinished()
{
    return m_bLoadFinished;
}

CLoadingScene* CLoadingScene::Create(_Device pDevice, LOADINGID eSceneID)
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
    auto* pManagement = Engine::CManagement::Get_Instance();

    pManagement->Ready_Buffer(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_TriColor", Engine::BUFFERID::BUFFER_TRICOL);
    pManagement->Ready_Buffer(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_TerrainTex", Engine::BUFFERID::BUFFER_TERRAINTEX);
    pManagement->Ready_Buffer(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_CubeTex", Engine::BUFFERID::BUFFER_CUBETEX);

    pManagement->Ready_Texture(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_Terrain", Engine::TEXTYPE::TEX_NORMAL, L"../../Resource/TestResource/Texture/Terrain/Grass_%d.tga", 2);
    pManagement->Ready_Texture(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_Skybox", Engine::TEXTYPE::TEX_CUBE, L"../../Resource/TestResource/Texture/SkyBox/test%d.dds", 4);


    pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"Mesh_Stone", Engine::MESHTYPE::MESH_STATIC, L"../../Resource/TestResource/Mesh/StaticMesh/TombStone/",
        L"TombStone.x");


    pManagement->Ready_Prototype(L"Camera_Comp", Engine::CCameraComponent::Create(m_pDevice));

    m_bLoadFinished = true;

    return S_OK;
}

HRESULT CLoadingScene::Load_Stage1_Resource()
{
    return S_OK;
}

void CLoadingScene::Free()
{
    WaitForSingleObject(m_hLoadingThread, INFINITE);
    CloseHandle(m_hLoadingThread);

    DeleteCriticalSection(&m_pCritSection);

    Safe_Release(m_pDevice);

    CScene::Free();
}
