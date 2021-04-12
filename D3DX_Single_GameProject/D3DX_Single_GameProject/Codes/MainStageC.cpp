#include "framework.h"
#include "MainStageC.h"

#include "MainMapC.h"
#include "SkyBox.h"
#include "Light.h"
#include "LightingManager.h"

#include "NaviMeshController.h"

#include "NaviMeshController.h"

#include "Player.h"
#include "BaseAI.h"
#include "BaseAI_Attacker.h"
#include "BaseAI_Flyer.h"
#include "BossAI_Strider.h"
#include "BaseProjectile.h"

#include "BaseObject.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "ControlSupport.h"

CMainStageC::CMainStageC(_Device pDevice)
	: Engine::CScene(pDevice)
	, m_pNaviController(nullptr)
{
}

HRESULT CMainStageC::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Resource(m_pDevice), E_FAIL);

	SetWindowText(g_hWnd, L"Stage C");

	Add_Player_Layer(L"PlayerLayer");
	Add_Boss_Layer(L"BossLayer");
	Add_Object_Layer(L"ObjectLayer");
	Add_Enemy_Layer(L"EnemyLayer");
	Add_Environment_Layer(L"EnviromentLayer");
	Add_Weapon_Layer(L"WeaponLayer");

	return S_OK;
}

_int CMainStageC::Update_Scene(const _float& fDeltaTime)
{
	CScene::Update_Scene(fDeltaTime);

	//이동 판정
	m_pNaviController->Compare_NaviMove(Get_Layer(L"PlayerLayer"));
	m_pNaviController->Compare_Navi_MeshMove(Get_Layer(L"EnemyLayer"));
	m_pNaviController->Compare_Navi_MeshMove(Get_Layer(L"BossLayer"));

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Get_Layer(L"PlayerLayer")->Get_GameObject(L"Player"));
	if (pPlayer == nullptr)
	{
		return E_FAIL;
	}

	if (m_pNaviController->Stand_NaviMesh(pPlayer))
	{
		pPlayer->Jump_Cancel();
	}

	return _int();
}

_int CMainStageC::LateUpdate_Scene(const _float& fDeltaTime)
{
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


    return NO_EVENT;
}

void CMainStageC::Render_Scene(void)
{
}

HRESULT CMainStageC::Ready_Resource(_Device& pDevice)
{
	return S_OK;
}

HRESULT CMainStageC::Add_Player_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    Engine::CGameObject* pGameObject = nullptr;

    auto pManagement = Engine::CManagement::Get_Instance();
    NULL_CHECK_RETURN(pManagement, E_FAIL);

    pGameObject = pManagement->Clone_GameObject(L"Player");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pGameObject->Set_Position(_vec3(-66.f, 1.f, -3.f));
    pLayer->Add_GameObject(L"Player", pGameObject);


    pGameObject = pManagement->Clone_GameObject(L"PlayerCamera");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pLayer->Add_GameObject(L"PlayerCamera", pGameObject);



    m_mapLayer.emplace(pLayerTag, pLayer);

    return S_OK;
}

HRESULT CMainStageC::Add_Enemy_Layer(const _tchar* pLayerTag)
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
    ////헌터
    //pGameObject = pManagement->Clone_GameObject(L"EnemyHunter");
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //pGameObject->Set_Position(_vec3(20.f, 0.f, 15.f));
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Hunter A1", pGameObject), E_FAIL);
    
    //pGameObject = pManagement->Clone_GameObject(L"EnemyHunter");
    //NULL_CHECK_RETURN(pGameObject, E_FAIL);
    //pGameObject->Set_Position(_vec3(40.f, 0.f, 15.f));
    //FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Hunter A2", pGameObject), E_FAIL);



	return S_OK;
}

HRESULT CMainStageC::Add_Boss_Layer(const _tchar* pLayerTag)
{
	return S_OK;
}

HRESULT CMainStageC::Add_Object_Layer(const _tchar* pLayerTag)
{
	return S_OK;
}

HRESULT CMainStageC::Add_Environment_Layer(const _tchar* pLayerTag)
{
    Engine::CLayer* pLayer = Engine::CLayer::Create();

    Engine::CGameObject* pGameObject = nullptr;

    auto pManagement = Engine::CManagement::Get_Instance();
    if (pManagement == nullptr)
    {
        return E_FAIL;
    }

    pGameObject = pManagement->Clone_GameObject(L"TestLight");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DirLight", pGameObject), E_FAIL);

    pGameObject = pManagement->Clone_GameObject(L"SkyBoxA");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    pLayer->Add_GameObject(L"SkyBoxA", pGameObject);

    pGameObject = pManagement->Clone_GameObject(L"MapC");
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MainMapC", pGameObject), E_FAIL);

    pGameObject = m_pNaviController = CNaviMeshController::Create(m_pDevice);
    NULL_CHECK_RETURN(pGameObject, E_FAIL);
    FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MapCNavi", pGameObject), E_FAIL);

    m_pNaviController->Set_NaviMesh_From_File(L"../../Resource/Meshes/Navi/map3.json");


    m_mapLayer.emplace(pLayerTag, pLayer);

    return S_OK;
}

HRESULT CMainStageC::Add_Weapon_Layer(const _tchar* pLayerTag)
{
	return S_OK;
}

CMainStageC* CMainStageC::Create(_Device pDevice)
{
	CMainStageC* pInstance = new CMainStageC(pDevice);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMainStageC::Free()
{
	Engine::CScene::Free();
}
