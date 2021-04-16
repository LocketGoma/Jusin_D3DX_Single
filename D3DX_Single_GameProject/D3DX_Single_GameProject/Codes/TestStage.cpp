#include "framework.h"
#include "TestStage.h"

#include "TestPlayer.h"
#include "TestCamera.h"
#include "SkyBox.h"
#include "TestTerrain.h"

#include "TestObject.h"
#include "TestMapObject.h"
#include "WeaponCrowBar.h"

#include "NaviMeshController.h"

#include "Player.h"
#include "MainCamera.h"
#include "BaseAI.h"
#include "BaseAI_Attacker.h"
#include "BaseAI_Flyer.h"
#include "BossAI_Strider.h"
#include "BaseProjectile.h"

#include "BaseObject.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "TriggerBox.h"
#include "ControlSupport.h"

#include "Transform.h"
#include <MainStageA.h>

CTestStage::CTestStage(_Device pDevice)
    : Engine::CScene(pDevice)
{
}

HRESULT CTestStage::Ready_Scene(void)
{
    FAILED_CHECK_RETURN(Ready_Resource(m_pDevice), E_FAIL);

    //ShowCursor(false);
    SetWindowText(g_hWnd, L"Test Stage");

    //Add_Test_Layer(L"TestLayer");
    Add_Player_Layer(L"PlayerLayer");
    Add_Object_Layer(L"ObjectLayer");
    Add_ColiderBox_Layer(L"ColliderLayer");
    Add_Camera_Layer(L"CameraLayer");
    Add_Environment_Layer(L"MapLayer");
    //Add_Enemy_Layer(L"EnemyLayer");
    Add_Weapon_Layer(L"WeaponLayer");    
    //Add_Enemy_Control_Layer(L"EnemyControlLayer");    

    return S_OK;
}

_int CTestStage::Update_Scene(const _float& fDeltaTime)
{
    if (bChangeScene == true)
    {
        return CHANGE_SCENE;
    }

    CScene::Update_Scene(fDeltaTime);

    //이동 판정
    m_pNaviController->Compare_NaviMove(Get_Layer(L"PlayerLayer"));
  //  m_pNaviController->Compare_Navi_MeshMove(Get_Layer(L"EnemyLayer"));

    CPlayer* pPlayer = dynamic_cast<CPlayer*>(Get_Layer(L"PlayerLayer")->Get_GameObject(L"Player"));
    if (pPlayer == nullptr)
    {
        return E_FAIL;
    }
    m_pNaviController->Get_NowIndex(&pPlayer->Get_Position());

    if (m_pNaviController->Stand_NaviMesh(pPlayer))
    {
        pPlayer->Jump_Cancel();
    }

    Engine::CLayer* targetLayer = Get_Layer(L"ObjectLayer");
    if (targetLayer != nullptr)
    {
        for (auto& iter : *targetLayer->Get_ObjectLayer())
        {
            CBaseObject* pObject = dynamic_cast<CBaseObject*>(iter.second);
            if (pObject != nullptr)
            {
                if (m_pNaviController->Stand_NaviMesh(pObject))
                {
                    pObject->Set_ClearGSpeed();
                }
            }
        }
    }

    return NO_EVENT;
}

_int CTestStage::LateUpdate_Scene(const _float& fDeltaTime)
{
    if (bChangeScene == true)
    {
        return CHANGE_SCENE;
    }

    CScene::LateUpdate_Scene(fDeltaTime);

    auto pManagement = Engine::CManagement::Get_Instance();
    if (pManagement == nullptr)
    {
        return E_FAIL;
    }

    CPlayer* pPlayer = dynamic_cast<CPlayer*>(pManagement->Get_GameObject_From_Layer(L"PlayerLayer", L"Player"));
    ///중력건 포착용 상호작용 판정부

    _bool bPick = false;
    //플레이어 - 오브젝트 상호작용 판정
    Engine::CLayer* targetLayer = Get_Layer(L"ObjectLayer");
    if (targetLayer != nullptr)
    {
        _float fBestRange = 9999.f;
        for (auto& iter : *targetLayer->Get_ObjectLayer())
        {
            CBaseObject* pObject = dynamic_cast<CBaseObject*>(iter.second);
            if (pObject != nullptr && pObject->Get_ObjectType() != eForceType::NONE)
            {
                if (pObject->Check_RayCollision_By_CollisionSphere())                //충돌했으면
                {
                    if (fBestRange >= pObject->Get_SupportUnit()->Get_Distance())   //물체 거리 재고
                    {
                        fBestRange = pObject->Get_SupportUnit()->Get_Distance();    //더 가까우면 갱신
                        pPlayer->Get_Pick_Object(pObject, fBestRange);
                        bPick = true;
                    }
                }
            }        
        }
    }

    //플레이어 - 총알 상호작용 판정
    targetLayer = Get_Layer(L"WeaponLayer");
    if (targetLayer != nullptr)
    {
        _float fBestRange = 9999.f;
        for (auto& iter : *targetLayer->Get_ObjectLayer())
        {
            CBaseObject* pObject = dynamic_cast<CBaseObject*>(iter.second);
            if (pObject != nullptr && pObject->Get_ObjectType() != eForceType::NONE)
            {
                if (pObject->Check_RayCollision_By_CollisionSphere())                //충돌했으면
                {
                    if (fBestRange >= pObject->Get_SupportUnit()->Get_Distance())   //물체 거리 재고
                    {
                        fBestRange = pObject->Get_SupportUnit()->Get_Distance();    //더 가까우면 갱신
                        pPlayer->Get_Pick_Object(pObject, fBestRange);
                        bPick = true;
                    }
                }
            }
        }
    }

    //플레이어 - 몬스터 상호작용 판정 (일부에만 해당)


    //최종 picking 판정
    if (bPick == false)
    {
        pPlayer->Get_Pick_Object(nullptr, -1.f);
    }
    //상호작용 판정부 끝


    //플레이어 - 몬스터 충돌판정
    targetLayer = Get_Layer(L"EnemyLayer");
    if (targetLayer != nullptr)
    for (auto& iter : *targetLayer->Get_ObjectLayer())
    {
        CDynamicObject* pObject = dynamic_cast<CDynamicObject*>(iter.second);
        if (pObject != nullptr)
        {
            pObject->Check_Hit(false, pPlayer->Get_WeaponDamage());
            
            if (pPlayer->Check_Attack_Collide(&(pObject->Get_CorePos()), pObject->Get_CollideRange()))
            {
                pPlayer->Hit_Attack(pObject->Get_Damage());
            }
        }
    }

    //플레이어 - 총알 충돌판정
    targetLayer = Get_Layer(L"WeaponLayer");
    if (targetLayer != nullptr)
    for (auto& iter : *targetLayer->Get_ObjectLayer())
    {
        CBaseProjectile* pObject = dynamic_cast<CBaseProjectile*>(iter.second);
        if (pObject != nullptr)
        {
            if (pObject->Get_TargetState() == eTargetState::ToPlayer)
            {
                if (pPlayer->Check_Attack_Collide(&(pObject->Get_Position()), pObject->Get_Radius()))
                {
                    pPlayer->Hit_Attack(pObject->Get_Damage());
                    pObject->Set_Dead();
                }
            }
        }
    }

    //플레이어 - 트리거박스 충돌 판정
    targetLayer = Get_Layer(L"ColliderLayer");
    if (targetLayer != nullptr)
        for (auto& iter : *targetLayer->Get_ObjectLayer())
        {
            CTriggerBox* pObject = dynamic_cast<CTriggerBox*>(iter.second);
            if (pObject != nullptr)
            {
                _vec3 vPlayerPos = pPlayer->Get_Position();
                if (pObject->Check_Collision(&vPlayerPos, PLAYER_BASE_HITBOX))
                {
                    bChangeScene = true;
                    break;
                }
            }
        }

    if (bChangeScene == true) 
    {
        Change_Scene(ESceneType::SCENE_STAGE1);
    }
    return NO_EVENT;
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

    pGameObject = pManagement->Clone_GameObject(L"TestCamera");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pLayer->Add_GameObject(L"TestCamera", pGameObject);

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
  // pGameObject->Set_Position(_vec3(155.f, 14.f, -240.f));
  //  pGameObject->Set_Position(_vec3(-66.f, 1.f, -3.f));
   //pGameObject->Set_Position(_vec3(45.f, 0.f, 10.f));
    //dynamic_cast<Engine::CTransform*>(pGameObject->Get_Component(L"Com_Transform", Engine::COMPONENT_ID::ID_DYNAMIC))->Rotation(Engine::ROTATION::ROT_Y, D3DXToRadian(90));

    pGameObject = pManagement->Clone_GameObject(L"PlayerCamera");
    dynamic_cast<CMainCamera*>(pGameObject)->Set_Player(dynamic_cast<CPlayer*>(pLayer->Get_GameObject(L"Player")));
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pLayer->Add_GameObject(L"PlayerCamera", pGameObject);

    //pGameObject = pManagement->Clone_GameObject(L"StatusUI");
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //pLayer->Add_GameObject(L"PlayerStatusUI", pGameObject);

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

    pGameObject = pManagement->Clone_GameObject(L"BaseObject_Drum");    
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pLayer->Add_GameObject(L"BaseObject", pGameObject);
    
    m_mapLayer.emplace(pLayerTag, pLayer);

    return S_OK;
}

HRESULT CTestStage::Add_ColiderBox_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    Engine::CGameObject* pGameObject = nullptr;

    auto pManagement = Engine::CManagement::Get_Instance();
    if (pManagement == nullptr)
    {
        return E_FAIL;
    }

    pGameObject = CTriggerBox::Create(m_pDevice, _vec3(-1.f, -2.f, -1.f), _vec3(1.f, 2.f, 1.f));
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pGameObject->Set_Position(_vec3(10.f, 0.f, 10.f));
    pLayer->Add_GameObject(L"TriggerBoxA", pGameObject);

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

    pGameObject = pManagement->Clone_GameObject(L"StageCLight");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DirLight", pGameObject), E_FAIL);

    pGameObject = pManagement->Clone_GameObject(L"SkyBoxA");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pLayer->Add_GameObject(L"SkyBoxA", pGameObject);

    //pGameObject = pManagement->Clone_GameObject(L"MapC");
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestMap", pGameObject), E_FAIL);

    pGameObject = m_pNaviController = CNaviMeshController::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"TestNavi", pGameObject), E_FAIL);

    m_pNaviController->Set_NaviMesh_From_File(L"../../Resource/TestResource/Navi/testfile.json");
   // m_pNaviController->Set_NaviMesh_From_File(L"../../Resource/Meshes/Navi/map1.json");
   //m_pNaviController->Set_NaviMesh_From_File(L"../../Resource/Meshes/Navi/map3.json");


    m_mapLayer.emplace(pLayerTag, pLayer);

    return S_OK;
}

HRESULT CTestStage::Add_Enemy_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();
    Engine::CLayer* pAILayer = Engine::CLayer::Create();

    Engine::CGameObject* pGameObject = nullptr;

    auto pManagement = Engine::CManagement::Get_Instance();
    if (pManagement == nullptr)
    {
        return E_FAIL;
    }
    ///적 스폰 파트
    ////개미귀신
    //pGameObject = pManagement->Clone_GameObject(L"EnemyAntlion");
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //pGameObject->Set_Position(_vec3(10.f, 0.f, 15.f));
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Antlion1", pGameObject), E_FAIL);

    //pGameObject = pManagement->Clone_GameObject(L"EnemyAntlion");
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //pGameObject->Set_Position(_vec3(16.f, 0.f, 15.f));
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Antlion2", pGameObject), E_FAIL);

    ////헌터
    //pGameObject = pManagement->Clone_GameObject(L"EnemyHunter");
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //pGameObject->Set_Position(_vec3(20.f, 0.f, 15.f));
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Hunter A1", pGameObject), E_FAIL);

    pGameObject = pManagement->Clone_GameObject(L"EnemyHunter");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pGameObject->Set_Position(_vec3(40.f, 0.f, 15.f));
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Hunter A2", pGameObject), E_FAIL);

    //스트라이더
    pGameObject = pManagement->Clone_GameObject(L"BossStrider");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pGameObject->Set_Position(_vec3(25.f, 0.f, 20.f));
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Strider", pGameObject), E_FAIL);

    //맨핵
    //pGameObject = pManagement->Clone_GameObject(L"EnemyManhack");
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //pGameObject->Set_Position(_vec3(25.f, 10.f, 20.f));
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Manhack", pGameObject), E_FAIL);


    ///기본 적 AI파트
    ////개미귀신
    //pGameObject = CBaseAI_Attacker::Create(m_pDevice);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_ControlUnit(dynamic_cast<CDynamicObject*>(pLayer->Get_GameObject(L"Antlion1")));
    //dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_Target(m_mapLayer.find(L"PlayerLayer")->second->Find_GameObject(L"Player"));
    //FAILED_CHECK_RETURN(pAILayer->Add_GameObject(L"Antlion1AI", pGameObject), E_FAIL);

    //pGameObject = CBaseAI_Attacker::Create(m_pDevice);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //dynamic_cast<CBaseAI*>(pGameObject)->Set_ControlUnit(dynamic_cast<CDynamicObject*>(pLayer->Get_GameObject(L"Antlion2")));
    //dynamic_cast<CBaseAI*>(pGameObject)->Set_Target(m_mapLayer.find(L"PlayerLayer")->second->Find_GameObject(L"Player"));
    //FAILED_CHECK_RETURN(pAILayer->Add_GameObject(L"Antlion2AI", pGameObject), E_FAIL);

    ////헌터
    //pGameObject = CBaseAI_Attacker::Create(m_pDevice);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_ControlUnit(dynamic_cast<CDynamicObject*>(pLayer->Get_GameObject(L"Hunter A1")));
    //dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_Target(m_mapLayer.find(L"PlayerLayer")->second->Find_GameObject(L"Player"));
    //FAILED_CHECK_RETURN(pAILayer->Add_GameObject(L"EnemyHunterAI1", pGameObject), E_FAIL);

    pGameObject = CBaseAI_Attacker::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_ControlUnit(dynamic_cast<CDynamicObject*>(pLayer->Get_GameObject(L"Hunter A2")));
    dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_Target(m_mapLayer.find(L"PlayerLayer")->second->Find_GameObject(L"Player"));
    FAILED_CHECK_RETURN(pAILayer->Add_GameObject(L"EnemyHunterAI2", pGameObject), E_FAIL);

    //스트라이더
    pGameObject = CBossAI_Strider::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    dynamic_cast<CBossAI_Strider*>(pGameObject)->Set_ControlUnit(dynamic_cast<CDynamicObject*>(pLayer->Get_GameObject(L"Strider")));
    dynamic_cast<CBossAI_Strider*>(pGameObject)->Set_Target(m_mapLayer.find(L"PlayerLayer")->second->Find_GameObject(L"Player"));
    FAILED_CHECK_RETURN(pAILayer->Add_GameObject(L"EnemyStriderAI", pGameObject), E_FAIL);

    ////맨핵
    //pGameObject = CBaseAI_Flyer::Create(m_pDevice);
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //dynamic_cast<CBaseAI_Flyer*>(pGameObject)->Set_ControlUnit(dynamic_cast<CDynamicObject*>(pLayer->Get_GameObject(L"Manhack")));
    //dynamic_cast<CBaseAI_Flyer*>(pGameObject)->Set_Target(m_mapLayer.find(L"PlayerLayer")->second->Find_GameObject(L"Player"));
    //FAILED_CHECK_RETURN(pAILayer->Add_GameObject(L"EnemyManhackAI", pGameObject), E_FAIL);



    m_mapLayer.emplace(pLayerTag, pLayer);
    m_mapLayer.emplace(L"AILayer", pAILayer);

    return S_OK;
}
//탄들이 들어갈 레이어 * 피아구별 없음 *
HRESULT CTestStage::Add_Weapon_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

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

void CTestStage::Change_Scene(ESceneType eType)
{
    Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
    if (pManagement == nullptr)
    {
        return;
    }

    pManagement->Setup_CurrentScene((_uint)ESceneType::SCENE_STAGE1, CMainStageA::Create(m_pDevice));



}

void CTestStage::Free()
{

    Engine::CScene::Free();
}
