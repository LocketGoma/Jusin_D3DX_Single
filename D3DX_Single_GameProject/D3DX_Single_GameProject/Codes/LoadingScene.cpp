#include "framework.h"
#include "LoadingScene.h"

//�ε��� ������ (������Ʈ) ��
#include "Transform.h"
#include "CameraComponent.h"


//�ε��� Ŭ���� (���ӿ�����Ʈ) ��
//�׽�Ʈ�� Ŭ������
#include "TestObject.h"
#include "TestLight.h"
#include "TestCamera.h"
#include "TestTerrain.h"
#include "SkyBox.h"
#include "TestMapObject.h"
#include "TestPlayer.h"


///��¥����
#include "MainMapA.h"

#include "StaticObject.h"
#include "DynamicObject.h"

#include "Player.h"
#include "MainCamera.h"

//�� ��
#include "EnemyAntLion.h"
#include "EnemyHunter.h"

#include "BossStrider.h"


//�����
#include "PlayerWeapon.h"
#include "WeaponCrowBar.h"
#include "WeaponPistol.h"
#include "WeaponSMG.h"
#include "WeaponRifle.h"
#include "WeaponShotgun.h"
#include "WeaponPhysCannon.h"
#include "WeaponRPG.h"

//ź���
#include "ProjFlechette.h"
#include "ProjCoreBall.h"

//�ε� ������ ��


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

_int CLoadingScene::Update_Scene(const _float& fDeltaTime)
{
    return _int();
}

_int CLoadingScene::LateUpdate_Scene(const _float& fDeltaTime)
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

    m_bLoadFinished = false;

    m_hLoadingThread = (HANDLE)_beginthreadex(NULL, 0, LoadingByThread, this, 0, NULL);

    m_eNextLoadScene = eLoadingID;   


    return S_OK;
}

unsigned __stdcall CLoadingScene::LoadingByThread(void* pParam)
{
    //���̳��� ĳ��Ʈ ���� ����������;
    CLoadingScene* pLoading = (CLoadingScene *)(pParam);
    if (pLoading == nullptr)
    {
        return E_FAIL;
    }
    //ũ��Ƽ�� ���� ����

    _uint iFlag = 0;

    EnterCriticalSection(pLoading->Get_CriticalSection());

    iFlag = pLoading->Load_Base_Resource();
    iFlag = pLoading->Load_GameObject_Resource();

    switch (pLoading->Get_LoadingID())
    {
    case LOADINGID::LOADING_TEST :
        iFlag = pLoading->Load_Test_Resource();
        iFlag = pLoading->Load_Test_GameObject_Resource();
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
    if (m_bLoadFinished == true)
    {
        lstrcpy(m_szString, L"Loading End...");
    }

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

HRESULT CLoadingScene::Load_Test_Resource()
{
    auto* pManagement = Engine::CManagement::Get_Instance();
    NULL_CHECK_RETURN(pManagement, E_FAIL);

    pManagement->Ready_Texture(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_Terrain", Engine::TEXTYPE::TEX_NORMAL, L"../../Resource/TestResource/Texture/Terrain/Grass_%d.tga", 2);

    pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"Mesh_Stone", Engine::MESHTYPE::MESH_STATIC, L"../../Resource/TestResource/Mesh/StaticMesh/TombStone/", L"TombStone.x");

    pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"Mesh_HL2Dog", Engine::MESHTYPE::MESH_DYNAMIC, L"../../Resource/TestResource/Unpack/Dog/", L"Dog.x");

    //pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"Mesh_HL2Dog", Engine::MESHTYPE::MESH_STATIC, L"../../Resource/TestResource/Unpack/Dog/", L"Dog.x");

    //pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"TestMapX", Engine::MESHTYPE::MESH_STATIC, L"../../Resource/Meshes/Static/Map/Map01/", L"Map01_Final_Edited.x");    

    pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"Mesh_Ant", Engine::MESHTYPE::MESH_DYNAMIC, L"../../Resource/TestResource/Unpack/Antlion/", L"antlion.x");


    return S_OK;
}

HRESULT CLoadingScene::Load_Test_GameObject_Resource()
{
    auto* pManagement = Engine::CManagement::Get_Instance();
    NULL_CHECK_RETURN(pManagement, E_FAIL);

    Engine::CGameObject* pGameObject = nullptr;

    //�׽�Ʈ ������Ʈ
    pGameObject = CTestObject::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"TestObject", pGameObject);

    //�׽�Ʈ �÷��̾�
    pGameObject = CTestPlayer::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"TestPlayer", pGameObject);

    //�׽�Ʈ ī�޶�
    pGameObject = CTestCamera::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"TestCamera", pGameObject);

    //�׽�Ʈ ����
    pGameObject = CTestTerrain::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"TestTerrain", pGameObject);

    //�׽�Ʈ ����Ʈ
    pGameObject = CTestLight::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"TestLight", pGameObject);

    //�׽�Ʈ ��
    pGameObject = CTestMapObject::Create(m_pDevice, L"TestMapX");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"TestMap", pGameObject);

    //�׽�Ʈ ��ī�̹ڽ�
    pManagement->Ready_Texture(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_Skybox", Engine::TEXTYPE::TEX_CUBE, L"../../Resource/TestResource/Texture/SkyBox/test%d.dds", 4);

    m_bLoadFinished = true;

    return S_OK;
}


HRESULT CLoadingScene::Load_Base_Resource()
{
    lstrcpy(m_szString, L"Loading...............");

    auto* pManagement = Engine::CManagement::Get_Instance();
    NULL_CHECK_RETURN(pManagement, E_FAIL);

    ///�⺻ ������Ÿ�� ���ҽ���
    pManagement->Ready_Buffer(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_TriColor", Engine::BUFFERID::BUFFER_TRICOL);
    pManagement->Ready_Buffer(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_TerrainTex", Engine::BUFFERID::BUFFER_TERRAINTEX);
    pManagement->Ready_Buffer(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_CubeTex", Engine::BUFFERID::BUFFER_CUBETEX);

    pManagement->Ready_Prototype(L"Camera_Comp", Engine::CCameraComponent::Create(m_pDevice));



    pManagement->Ready_Texture(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_SkyboxA", Engine::TEXTYPE::TEX_CUBE, L"../../Resource/Image/Skybox/SkyboxA.dds", 1);


    pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"Mesh_Navi", Engine::MESHTYPE::MESH_NAVI, L"", L"");


    ///�Ʒ����� ��¥��� �޽���

    //��
    pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"MapA", Engine::MESHTYPE::MESH_STATIC, L"../../Resource/Meshes/Static/Map/Map01/", L"Max01_Fina_Edited.x");

   // pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"MapB", Engine::MESHTYPE::MESH_STATIC, L"../../Resource/Meshes/Static/Map/Map02/", L"Map02_Edited.x");

    //�� �� �̻���
    //pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"MapC", Engine::MESHTYPE::MESH_STATIC, L"../../Resource/Meshes/Static/Map/Map03/", L"Map03_test.x");

    //��
    pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"Antlion", Engine::MESHTYPE::MESH_DYNAMIC, L"../../Resource/Meshes/Dynamic/Enemy/Antlion/", L"Antlion_Final.x");

    pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"Hunter", Engine::MESHTYPE::MESH_DYNAMIC, L"../../Resource/Meshes/Dynamic/Enemy/Hunter/", L"Hunter_Final.x");

    pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"Strider", Engine::MESHTYPE::MESH_DYNAMIC, L"../../Resource/Meshes/Dynamic/Enemy/Strider/", L"Strider_Final.x");


    //�÷��̾� (����)
    pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"Crowbar", Engine::MESHTYPE::MESH_DYNAMIC, L"../../Resource/Meshes/Dynamic/Weapon/Crowbar/", L"Crowbar_Final.x");

    pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"Pistol", Engine::MESHTYPE::MESH_DYNAMIC, L"../../Resource/Meshes/Dynamic/Weapon/Pistol/", L"Pistol_Final.x");

    pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"PlayerSMG", Engine::MESHTYPE::MESH_DYNAMIC, L"../../Resource/Meshes/Dynamic/Weapon/SMG/", L"SMG_Final.x");

    pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"PlayerRifle", Engine::MESHTYPE::MESH_DYNAMIC, L"../../Resource/Meshes/Dynamic/Weapon/Rifle/", L"Rifle_Final.x");

    pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"PlayerShotgun", Engine::MESHTYPE::MESH_DYNAMIC, L"../../Resource/Meshes/Dynamic/Weapon/Shotgun/", L"Shotgun_Final.x");

    pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"Physcannon", Engine::MESHTYPE::MESH_DYNAMIC, L"../../Resource/Meshes/Dynamic/Weapon/Physcannon/", L"physcannon_Final.x");

    pManagement->Ready_Meshes(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_MESH, L"RocketLauncher", Engine::MESHTYPE::MESH_DYNAMIC, L"../../Resource/Meshes/Dynamic/Weapon/RocketLauncher/", L"RocketLauncher_Final.x");

    return S_OK;
}



HRESULT CLoadingScene::Load_Stage1_Resource()
{
    return S_OK;
}

HRESULT CLoadingScene::Load_GameObject_Resource()
{
    auto* pManagement = Engine::CManagement::Get_Instance();
    NULL_CHECK_RETURN(pManagement, E_FAIL);

    Engine::CGameObject* pGameObject = nullptr;

    ///���� ������
    //��
    pGameObject = CMainMapA::Create(m_pDevice, L"MapA");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"MapA", pGameObject);

    //��ī�̹ڽ�
    pGameObject = CSkyBox::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"SkyBoxA", pGameObject);


    //�� (�⺻)
    pGameObject = CEnemyAntLion::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"EnemyAntlion", pGameObject);

    pGameObject = CEnemyHunter::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"EnemyHunter", pGameObject);


    //�� (������)
    pGameObject = CBossStrider::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"BossStrider", pGameObject);



    //�÷��̾� �⺻

    pGameObject = CPlayer::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"Player", pGameObject);

    pGameObject = CMainCamera::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"PlayerCamera", pGameObject);

    //����
    pGameObject = CWeaponCrowbar::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"WeaponCrowbar", pGameObject);

    pGameObject = CWeaponPistol::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"WeaponPistol", pGameObject);

    pGameObject = CWeaponSMG::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"WeaponSMG", pGameObject);

    pGameObject = CWeaponRifle::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"WeaponRifle", pGameObject);

    pGameObject = CWeaponShotgun::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"WeaponShotgun", pGameObject);

    pGameObject = CWeaponPhysCannon::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"WeaponPhysCannon", pGameObject);

    pGameObject = CWeaponRPG::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"WeaponRPG", pGameObject);


    //ź��
    pGameObject = CProjFlechette::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"Projectile_Flechette", pGameObject);

    pGameObject = CProjCoreBall::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pManagement->Add_GameObject_Prototype(L"Projectile_CoreBall", pGameObject);

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
