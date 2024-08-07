#include "framework.h"
#include "MainGame.h"

#include "Scene.h"
#include "Management.h"
#include "TestStage.h"
#include "BaseLogoScene.h"

#include "Transform.h"
#include "CameraComponent.h"

#include "Player.h"

#include "Shader.h"

bool g_bViewCollider = true;
bool g_bViewNavimesh = true;
bool g_bViewDebugTarget = true;
bool g_bEndingTimeDelay = false;
float g_fEndingTimeDelay = 1.f;

CMainGame::CMainGame()
    : m_pManagement(Engine::CManagement::Get_Instance())
    , pScene(nullptr) 
    , m_fTime(0.f)
    , m_fAccTimeDelta (0.f)
    , m_fCallLimit(1.f/60.f)
    , m_bLock(false)
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

    Setup_DefaultSetting();

    Setup_Shader();

    m_pManagement->Setup_SceneManager(1);
    
    //Ready_Scene();

    m_pManagement->Load_Sound("../../Resource/","gman_full.wav");

    m_pManagement->Setup_CurrentScene((_uint)ESceneType::STAGE_LOGO, CBaseLogoScene::Create(m_pDevice));

    m_pManagement->Play_Sound(L"gman_full.wav", Engine::SOUND_CHANNELID::BGM);

    return S_OK;
}

_bool CMainGame::In_TimerLock(_float fDeltaTime)
{
    m_fAccTimeDelta += fDeltaTime;

    if (m_fAccTimeDelta >= m_fCallLimit)
    {
        m_fAccTimeDelta = 0.f;

        return true;
    }
    return false;
}

_int CMainGame::Update_MainGame()
{
    _float fDeltaTime = m_pManagement->Get_DeltaTime();
    //m_fTime += fDeltaTime;
    m_fFPSTimer += fDeltaTime;

    //m_bLock = In_TimerLock(fDeltaTime);     
    //if (m_bLock == true)
    //{
    //    return NO_EVENT;
    //}
    //fDeltaTime = m_fTime;
    //m_fTime = 0.f;

    if (m_pManagement->Key_Down('O'))
    {
        g_bViewCollider = g_bViewCollider == false;
    }
    if (m_pManagement->Key_Down('P'))
    {
        g_bViewNavimesh = g_bViewNavimesh == false;
    }
    if (m_pManagement->Key_Down('L'))
    {
        g_bViewDebugTarget = g_bViewDebugTarget == false;
        m_pManagement->Set_Visualble_DebugBuffer(g_bViewDebugTarget);
    }

    if (g_bEndingTimeDelay)
    {
        g_fEndingTimeDelay += (fDeltaTime);
        fDeltaTime /= g_fEndingTimeDelay;
    }

    m_pManagement->Update_Engine(fDeltaTime);
    m_pManagement->LateUpdate_Engine(fDeltaTime);
    return _int();
}

void CMainGame::Render_MainGame()
{
    if (m_bLock == true)
    {
        return;
    }


    if (nullptr == m_pManagement)
        return;

    ++m_dwRenderCount;

    if (m_fFPSTimer >= 1.f)
    {
        _tchar m_szFPS[256];

        wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCount);
        m_fFPSTimer = 0.f;
        m_dwRenderCount = 0;

        SetWindowText(g_hWnd, m_szFPS);
    }
    

    if (FAILED(m_pManagement->Render_Engine(g_hWnd)))
    {
        PRINT_LOG(L"FATAL ERROR", L"RENDER ENGINE FAIL");
        return;
    }

}

HRESULT CMainGame::Setup_DefaultSetting()
{
    if (FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
        return E_FAIL;

    if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW)))
        return E_FAIL;

    // Font
    FAILED_CHECK_RETURN(m_pManagement->Ready_Font((m_pDevice), L"Font_BASE", L"����", 15, 15, FW_HEAVY), E_FAIL);
    FAILED_CHECK_RETURN(m_pManagement->Ready_Font((m_pDevice), L"Font_Status_MSG", L"����ü", 10, 10, FW_BOLD), E_FAIL);
    FAILED_CHECK_RETURN(m_pManagement->Ready_Font((m_pDevice), L"Font_Status_Gauge", L"halflife2", 45, 45, FW_HEAVY, true), E_FAIL);
    FAILED_CHECK_RETURN(m_pManagement->Ready_Font((m_pDevice), L"Font_Status_Gauge_Mini", L"halflife2", 25, 25, FW_HEAVY, true), E_FAIL);
    FAILED_CHECK_RETURN(m_pManagement->Ready_Font((m_pDevice), L"Font_UI_Weapon", L"halflife2", 75, 75, FW_ULTRALIGHT, true), E_FAIL);
    FAILED_CHECK_RETURN(m_pManagement->Ready_Font((m_pDevice), L"Font_UI_Cross", L"halflife2", 150, 150, FW_ULTRALIGHT, true), E_FAIL);
    FAILED_CHECK_RETURN(m_pManagement->Ready_Font((m_pDevice), L"Font_UI_Cross_Core", L"halflife2", 80, 100, FW_ULTRALIGHT, true), E_FAIL);
    //FAILED_CHECK_RETURN(m_pManagement->Ready_Font((m_pDevice), L"Font_UI_Cross", L"hl2crosshairs", 30, 30, FW_HEAVY, true), E_FAIL);


    return S_OK;
}

HRESULT CMainGame::Setup_Shader()
{
    Engine::CShader* pShader = nullptr;

    pShader = Engine::CShader::Create(m_pDevice, L"../../Reference/Headers/Shader_Dissolve.hlsl");
    NULL_CHECK_RETURN(pShader, E_FAIL);
    m_pManagement->Ready_Prototype(L"Shader_Dissolve", pShader);

    pShader = Engine::CShader::Create(m_pDevice, L"../../Reference/Headers/Shader_MixAlpha.hlsl");
    NULL_CHECK_RETURN(pShader, E_FAIL);
    m_pManagement->Ready_Prototype(L"Shader_MixAlpha", pShader);

    pShader = Engine::CShader::Create(m_pDevice, L"../../Reference/Headers/Shader_For_End.hlsl");
    NULL_CHECK_RETURN(pShader, E_FAIL);
    m_pManagement->Ready_Prototype(L"Shader_For_End", pShader);

    return S_OK;
}

//���߿� �ΰ�/�ε��� ���� �ű��
HRESULT CMainGame::Ready_Scene()
{
    m_pManagement->Ready_Buffer(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_TriColor", Engine::BUFFERID::BUFFER_TRICOL);
    m_pManagement->Ready_Buffer(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_TerrainTex", Engine::BUFFERID::BUFFER_TERRAINTEX);
    m_pManagement->Ready_Buffer(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_CubeTex", Engine::BUFFERID::BUFFER_CUBETEX);
    m_pManagement->Ready_Texture(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_Terrain", Engine::TEXTYPE::TEX_NORMAL, L"../../Resource/TestResource/Texture/Terrain/Grass_%d.tga", 2);
    m_pManagement->Ready_Texture(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_Skybox", Engine::TEXTYPE::TEX_CUBE, L"../../Resource/TestResource/Texture/SkyBox/test%d.dds", 4);
    m_pManagement->Ready_Prototype(L"Transform_Comp", Engine::CTransform::Create(m_pDevice));
    m_pManagement->Ready_Prototype(L"Camera_Comp", Engine::CCameraComponent::Create(m_pDevice));

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

void CMainGame::MouseProc(UINT message, WPARAM wParam)
{
    return CPlayer::MouseProc(message, wParam);
}
