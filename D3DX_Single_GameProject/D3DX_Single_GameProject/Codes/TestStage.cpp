#include "framework.h"
#include "TestStage.h"

#include "TestPlayer.h"
#include "TestCamera.h"
#include "SkyBox.h"
#include "TestTerrain.h"
#include "TestLight.h"
#include "TestObject.h"
#include "TestMapObject.h"
#include "WeaponCrowBar.h"

#include "NaviMeshController.h"

#include "Player.h"
#include "BaseAI.h"
#include "DynamicObject.h"

CTestStage::CTestStage(_Device pDevice)
    : Engine::CScene(pDevice)
{
}

HRESULT CTestStage::Ready_Scene(void)
{
    FAILED_CHECK_RETURN(Ready_Resource(m_pDevice), E_FAIL);

    SetWindowText(g_hWnd, L"Test Stage");

    //Add_Test_Layer(L"PlayerLayer");
    Add_Player_Layer(L"PlayerLayer");
    //Add_Object_Layer(L"ObjectLayer");
    Add_Camera_Layer(L"CameraLayer");
    Add_Environment_Layer(L"MapLayer");
    Add_Enemy_Layer(L"EnemyLayer");
    //Add_Enemy_Control_Layer(L"EnemyControlLayer");

    

    return S_OK;
}

_int CTestStage::Update_Scene(const _float& fDeltaTime)
{
    //D3DXMatrixLookAtLH(&m_matView, &_vec3(0.f, 0.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f));
    //D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);

    //m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
    //m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
    CScene::Update_Scene(fDeltaTime);

    m_pNaviController->Compare_NaviMove(Get_Layer(L"PlayerLayer"));




    return _int();
}

_int CTestStage::LateUpdate_Scene(const _float& fDeltaTime)
{
    CScene::LateUpdate_Scene(fDeltaTime);

    auto pManagement = Engine::CManagement::Get_Instance();
    if (pManagement == nullptr)
    {
        return E_FAIL;
    }

    CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObject_From_Layer(L"PlayerLayer", L"Player"));

    

    auto* targetLayer = Get_Layer(L"EnemyLayer");
    for (auto& iter : *targetLayer->Get_ObjectLayer())
    {
        CDynamicObject* pObject = dynamic_cast<CDynamicObject*>(iter.second);
        if (pObject != nullptr)
        {
            pObject->Force_Update_Animation();
            pObject->Check_Hit(false, pPlayer->Get_WeaponDamage());
        }
    }


        



    return _int();
}

void CTestStage::Render_Scene(void)
{
 //라이트 세팅
 // 
    //CScene::Render_Scene();
}

HRESULT CTestStage::Ready_Resource(_Device& m_pDevice)
{
    return S_OK;
}

HRESULT CTestStage::Add_Test_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    Engine::CGameObject* pGameObject = nullptr;

    auto pManagement = Engine::CManagement::Get_Instance();
    if (pManagement == nullptr)
    {
        return E_FAIL;
    }

    pGameObject = pManagement->Clone_GameObject(L"TestPlayer");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pLayer->Add_GameObject(L"TestPlayer", pGameObject);

    pGameObject = pManagement->Clone_GameObject(L"WeaponCrowbar");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pLayer->Add_GameObject(L"WeaponCrowbar", pGameObject);

    m_mapLayer.emplace(pLayerTag, pLayer);

    return S_OK;
}

HRESULT CTestStage::Add_Player_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    Engine::CGameObject* pGameObject = nullptr;

    auto pManagement = Engine::CManagement::Get_Instance();
    NULL_CHECK_RETURN(pManagement, E_FAIL);

    pGameObject = pManagement->Clone_GameObject(L"Player");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pLayer->Add_GameObject(L"Player", pGameObject);

    pGameObject = pManagement->Clone_GameObject(L"PlayerCamera");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pLayer->Add_GameObject(L"PlayerCamera", pGameObject);



    m_mapLayer.emplace(pLayerTag, pLayer);

    return S_OK;
}

HRESULT CTestStage::Add_Object_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    Engine::CGameObject* pGameObject = nullptr;


    auto pManagement = Engine::CManagement::Get_Instance();
    if (pManagement == nullptr)
    {
        return E_FAIL;
    }

    pGameObject = pManagement->Clone_GameObject(L"TestObject");    
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pLayer->Add_GameObject(L"TestObject", pGameObject);
    
    m_mapLayer.emplace(pLayerTag, pLayer);

    return S_OK;
}

HRESULT CTestStage::Add_Camera_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    Engine::CGameObject* pGameObject = nullptr;

    auto pManagement = Engine::CManagement::Get_Instance();
    if (pManagement == nullptr)
    {
        return E_FAIL;
    }

    //pGameObject = pManagement->Clone_GameObject(L"TestCamera");
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //pLayer->Add_GameObject(L"TestCamera", pGameObject);

    pGameObject = pManagement->Clone_GameObject(L"SkyBoxA");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pLayer->Add_GameObject(L"SkyBoxA", pGameObject);
        
    m_mapLayer.emplace(pLayerTag, pLayer);

    return S_OK;
}

HRESULT CTestStage::Add_Environment_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    Engine::CGameObject* pGameObject = nullptr;

    auto pManagement = Engine::CManagement::Get_Instance();
    if (pManagement == nullptr)
    {
        return E_FAIL;
    }

    pGameObject = pManagement->Clone_GameObject(L"TestTerrain");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pLayer->Add_GameObject(L"TestTerrain", pGameObject);

    pGameObject = pManagement->Clone_GameObject(L"TestLight");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DirLight", pGameObject), E_FAIL);

    //pGameObject = pManagement->Clone_GameObject(L"MapA");
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestMap", pGameObject), E_FAIL);

    pGameObject = m_pNaviController = CNaviMeshController::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestNavi", pGameObject), E_FAIL);

    //m_pNaviController->Set_NaviMesh_From_File(L"../../Resource/TestResource/Navi/Test.json");
    m_pNaviController->Set_NaviMesh_From_File(L"../../Resource/TestResource/Navi/testfile.json");


    m_mapLayer.emplace(pLayerTag, pLayer);

    return S_OK;
}

HRESULT CTestStage::Add_Enemy_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    Engine::CGameObject* pGameObject = nullptr;

    auto pManagement = Engine::CManagement::Get_Instance();
    if (pManagement == nullptr)
    {
        return E_FAIL;
    }
    pGameObject = pManagement->Clone_GameObject(L"EnemyAntlion");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pGameObject->Set_Position(_vec3(10.f, 0.f, 15.f));
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Antlion1", pGameObject), E_FAIL);

    pGameObject = pManagement->Clone_GameObject(L"EnemyAntlion");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pGameObject->Set_Position(_vec3(16.f, 0.f, 15.f));
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Antlion2", pGameObject), E_FAIL);

    pGameObject = CBaseAI::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    dynamic_cast<CBaseAI*>(pGameObject)->Set_ControlUnit(dynamic_cast<CDynamicObject*>(pLayer->Get_GameObject(L"Antlion1")));
    dynamic_cast<CBaseAI*>(pGameObject)->Set_Target(m_mapLayer.find(L"PlayerLayer")->second->Find_GameObject(L"Player"));
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Antlion1AI", pGameObject), E_FAIL);

    //pGameObject = CBaseAI::Create(m_pDevice);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //dynamic_cast<CBaseAI*>(pGameObject)->Set_ControlUnit(dynamic_cast<CDynamicObject*>(pLayer->Get_GameObject(L"Antlion2")));
    //dynamic_cast<CBaseAI*>(pGameObject)->Set_Target(m_mapLayer.find(L"PlayerLayer")->second->Find_GameObject(L"Player"));
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Antlion2AI", pGameObject), E_FAIL);

    m_mapLayer.emplace(pLayerTag, pLayer);

    return S_OK;
}

HRESULT CTestStage::Add_Enemy_Control_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    Engine::CGameObject* pGameObject = nullptr;

    auto pManagement = Engine::CManagement::Get_Instance();
    if (pManagement == nullptr)
    {
        return E_FAIL;
    }

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
