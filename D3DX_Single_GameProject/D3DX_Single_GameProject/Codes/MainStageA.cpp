#include "framework.h"
#include "MainStageA.h"


#include "Management.h"

#include "MainMapA.h"
#include "SkyBox.h"
#include "Light.h"
#include "LightingManager.h"

#include "NaviMeshController.h"

#include "Player.h"
#include "MainCamera.h"
#include "BaseAI.h"
#include "BaseAI_Attacker.h"
#include "DynamicObject.h"
#include "BaseProjectile.h"

#include "BaseObject.h"
#include "StaticObject.h"
#include "DynamicObject.h"
#include "ControlSupport.h"

#include "Transform.h"
#include "TriggerBox.h"

#include "MainStageC.h"


CMainStageA::CMainStageA(_Device pDevice)
	: Engine::CScene(pDevice)
	, m_pNaviController(nullptr)
{
}

HRESULT CMainStageA::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Resource(m_pDevice), E_FAIL);

	//SetWindowText(g_hWnd, L"Stage A");

	Add_Player_Layer(L"PlayerLayer");
	//Add_Boss_Layer(L"BossLayer");
	Add_Object_Layer(L"ObjectLayer");
	Add_ColiderBox_Layer(L"ColliderLayer");
	Add_Enemy_Layer(L"EnemyLayer");
	Add_Weapon_Layer(L"WeaponLayer");
	Add_Environment_Layer(L"EnviromentLayer");

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}
	pManagement->Stop_AllSound();
	pManagement->Play_BGM(L"wind_med2.wav");

	m_bWaveTrigger[0] = false;				// 0번 트리거 : 시작시 4마리 다 잡으면 true
	m_bWaveTrigger[1] = true;				// 1번 트리거 : 중력건으로만 1마리
	m_bWaveTrigger[2] = false;				// 2번 트리거 : 샷건 2마리
											// 모든 트리거 True시 => 보스 등장
	m_bWaveTrigger[3] = false;				// 3번 트리거 : 보스 처치시

	
	//m_bWaveClear	-> 각 웨이브 클리어시 true로 변경.

	return S_OK;
}

_int CMainStageA::Update_Scene(const _float& fDeltaTime)
{
	if (m_bChangeScene == true)
	{
		return CHANGE_SCENE;
	}

	CScene::Update_Scene(fDeltaTime);

	//이동 판정
	m_pNaviController->Compare_NaviMove(Get_Layer(L"PlayerLayer"));
	m_pNaviController->Compare_Navi_MeshMove(Get_Layer(L"EnemyLayer"));
	//m_pNaviController->Compare_Navi_MeshMove(Get_Layer(L"BossLayer"));

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

	//물체 중력 적용 관련
	Engine::CLayer* targetLayer = Get_Layer(L"ObjectLayer");
	if (targetLayer != nullptr)
	{
		for (auto& iter : *targetLayer->Get_ObjectLayer())
		{
			CBaseObject* pObject = dynamic_cast<CBaseObject*>(iter.second);
			if (pObject != nullptr)
			{
				_float pfHeight = 0.f;

				if (m_pNaviController->Stand_NaviMesh(pObject, &pfHeight))
				{
					pObject->Set_ClearGSpeed(pfHeight);
				}
			}
		}
	}


	//사운드 관련
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}

	if (rand() % 200 < 1)
	{	
		pManagement->Play_Sound(L"alert1.wav", Engine::SOUND_CHANNELID::EFFECTE);;
	}
	
	return NO_EVENT;
}

_int CMainStageA::LateUpdate_Scene(const _float& fDeltaTime)
{
	if (m_bChangeScene == true)
	{
		return CHANGE_SCENE;
	}

	CScene::LateUpdate_Scene(fDeltaTime);

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
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

			if (pPlayer->Check_Attack_Collide(&(pObject->Get_Position()), pObject->Get_Radius()))
			{
				pObject->Interaction(pPlayer);
			}
		}
	}

	//플레이어 - 총알 상호작용 판정 (잡기)
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


	//플레이어 - 몬스터 충돌판정 + 총알 피격포함
	targetLayer = Get_Layer(L"EnemyLayer");
	if (targetLayer != nullptr)
		for (auto& iter : *targetLayer->Get_ObjectLayer())
		{
			CDynamicObject* pObject = dynamic_cast<CDynamicObject*>(iter.second);
			if (pObject != nullptr)
			{
				pObject->Force_Update_Animation();
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

	//몬스터 - 총알/오브젝트 충돌판정....
	Engine::CLayer* AmmoLayer = Get_Layer(L"WeaponLayer");
	Engine::CLayer* ObjectLayer = Get_Layer(L"ObjectLayer");
	targetLayer = Get_Layer(L"EnemyLayer");
	if (targetLayer != nullptr)
	{
		for (auto& iter : *targetLayer->Get_ObjectLayer())
		{
			CDynamicObject* pEnemy = dynamic_cast<CDynamicObject*>(iter.second);
			//몬스터 판단을 하고...
			if (pEnemy != nullptr)
			{
				if (AmmoLayer != nullptr)
				{
					for (auto& Aiter : *AmmoLayer->Get_ObjectLayer())
					{
						CBaseProjectile* pAObject = dynamic_cast<CBaseProjectile*>(Aiter.second);
						if (pAObject != nullptr)
						{
							if (pAObject->Get_TargetState() == eTargetState::ToEnemy)
							{
								if (pEnemy->Check_Attack_Collide(&(pAObject->Get_Position()), pAObject->Get_Radius()))
								{
									pEnemy->Hit_Attack(pAObject->Get_Damage());
									pAObject->Set_Break();
								}
							}
						}
					}
				}
				if (ObjectLayer != nullptr)
				{
					for (auto& Oiter : *ObjectLayer->Get_ObjectLayer())
					{
						CStaticObject* pObject = dynamic_cast<CStaticObject*>(Oiter.second);
						if (pObject != nullptr)
						{
							if (pObject->Get_ForceState() == eForceState::PULL)
							{
								if (pEnemy->Check_Attack_Collide(&(pObject->Get_Position()), pObject->Get_Radius()))
								{
									if (pEnemy->Get_RayPick(pObject->Get_Direction(), pObject->Get_Position()))
									{
										pObject->Set_Direction(pEnemy->Get_Reflection(pObject->Get_Direction(), pObject->Get_Position()));
										pEnemy->Hit_Attack(pObject->Get_Damage());
										//pObject->Set_Speed(pObject->Get_Speed() / 2);
									}
								}
							}
						}						
					}
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
				if (pObject->Check_Collision(&vPlayerPos, PLAYER_BASE_HITBOX)&& m_bWaveTrigger[3] == true)
				{
					m_bChangeScene = true;
					break;
				}
			}
		}
	if (m_bChangeScene == true)
	{
		pManagement->Stop_AllSound();
		Change_Scene(ESceneType::SCENE_STAGE1);

		return CHANGE_SCENE;
	}
	//웨이브 실패시 작동시키는...
	if(pManagement->Key_Down('K'))
	{
		Get_Layer(L"AILayer")->Clear_GameObject();
		Get_Layer(L"EnemyLayer")->Clear_GameObject();
	}


	//

	////웨이브 파트
	//시작 웨이브 -> 2번 웨이브
	// (1번 웨이브 추가시에는 WaveB말고 WaveA로 넣어주면 됨)
	if (m_bWaveClear[0] == false && Get_Layer(L"EnemyLayer")->Get_ObjectCount() == 0)
	{
		m_bWaveClear[0] = true;
	}
	if (m_bWaveClear[0] == true && m_bWaveTrigger[0] == false)
	{
		m_bWaveTrigger[0] = true;

		Add_Enemy_Layer_WaveC(L"EnemyLayer");
	}



	//2번 웨이브 (보스전 검사) -> 보스전
	if (m_bWaveClear[0] == true && Get_Layer(L"EnemyLayer")->Get_ObjectCount() == 0)
	{
		m_bWaveClear[2] = true;
	}

	if (m_bWaveClear[2] == true && m_bWaveTrigger[2] == false)
	{
		m_bWaveTrigger[2] = true;

		//Add_Boss_Layer(L"BossLayer");
		Add_Boss_Layer_WaveBoss(L"EnemyLayer");
	}

	//3번 웨이브 (보스전) -> 클리어
	if (m_bWaveClear[2] == true && Get_Layer(L"EnemyLayer")->Get_ObjectCount() == 0)
	{
		m_bWaveClear[3] = true;
	}

	if (m_bWaveClear[3] == true && m_bWaveTrigger[3] == false)
	{
		m_bWaveTrigger[3] = true;
	}



	return NO_EVENT;
}

void CMainStageA::Render_Scene(void)
{
	//CScene::Render_Scene();

}

HRESULT CMainStageA::Ready_Resource(_Device& pDevice)
{
	return S_OK;
}

HRESULT CMainStageA::Add_Player_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}

	pGameObject = pManagement->Clone_GameObject(L"Player");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Player", pGameObject);
	pGameObject->Set_Position(_vec3(155.f, 14.f, -240.f));

	pGameObject = pManagement->Clone_GameObject(L"PlayerCamera");
	dynamic_cast<CMainCamera*>(pGameObject)->Set_Player(dynamic_cast<CPlayer*>(pLayer->Get_GameObject(L"Player")));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"PlayerCamera", pGameObject);


	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;

}

HRESULT CMainStageA::Add_Enemy_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	Engine::CLayer* pAILayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}

	//적 스폰 파트
	//개미귀신
	pGameObject = pManagement->Clone_GameObject(L"EnemyAntlion");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->Set_Position(_vec3(116.f, 9.f, -150.f));
	dynamic_cast<CDynamicObject*>(pGameObject)->Set_Rotation(Engine::ROTATION::ROT_Y, D3DXToRadian(180.f));
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Antlion1", pGameObject), E_FAIL);
	
	pGameObject = pManagement->Clone_GameObject(L"EnemyAntlion");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->Set_Position(_vec3(140.f, 9.f, -150.f));
	dynamic_cast<CDynamicObject*>(pGameObject)->Set_Rotation(Engine::ROTATION::ROT_Y, D3DXToRadian(180.f));
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Antlion2", pGameObject), E_FAIL);

	//기본 적 AI파트
	//개미귀신
	pGameObject = CBaseAI_Attacker::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_ControlUnit(dynamic_cast<CDynamicObject*>(pLayer->Get_GameObject(L"Antlion1")));
	dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_Target(m_mapLayer.find(L"PlayerLayer")->second->Find_GameObject(L"Player"));
	FAILED_CHECK_RETURN(pAILayer->Add_GameObject(L"Antlion1AI", pGameObject), E_FAIL);

	pGameObject = CBaseAI_Attacker::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_ControlUnit(dynamic_cast<CDynamicObject*>(pLayer->Get_GameObject(L"Antlion2")));
	dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_Target(m_mapLayer.find(L"PlayerLayer")->second->Find_GameObject(L"Player"));
	FAILED_CHECK_RETURN(pAILayer->Add_GameObject(L"Antlion1A2", pGameObject), E_FAIL);








	m_mapLayer.emplace(pLayerTag, pLayer);
	m_mapLayer.emplace(L"AILayer", pAILayer);

	return S_OK;
}

HRESULT CMainStageA::Add_Boss_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	Engine::CLayer* pAILayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}

	pGameObject = pManagement->Clone_GameObject(L"BossHunter");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->Set_Position(_vec3(135.f, 12.f, -125.f));
	dynamic_cast<CDynamicObject*>(pGameObject)->Set_Rotation(Engine::ROTATION::ROT_Y, D3DXToRadian(180.f));
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BossHunter", pGameObject), E_FAIL);

	//헌터 AI
	pGameObject = CBaseAI_Attacker::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_ControlUnit(dynamic_cast<CDynamicObject*>(pLayer->Get_GameObject(L"BossHunter")));
	dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_Target(m_mapLayer.find(L"PlayerLayer")->second->Find_GameObject(L"Player"));
	FAILED_CHECK_RETURN(pAILayer->Add_GameObject(L"BossHunterAI", pGameObject), E_FAIL);

	//135,10,-125

	m_mapLayer.emplace(pLayerTag, pLayer);
	m_mapLayer.emplace(L"BossAILayer", pAILayer);

	return S_OK;
}

HRESULT CMainStageA::Add_Object_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}

	pGameObject = pManagement->Clone_GameObject(L"BaseObject_Drum");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->Set_Position(_vec3(150.f, 7.f, -160.f));
	pLayer->Add_GameObject(L"BaseObject 1", pGameObject);

	pGameObject = pManagement->Clone_GameObject(L"BaseObject_Drum");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->Set_Position(_vec3(120.f, 6.f, -130.f));
	pLayer->Add_GameObject(L"BaseObject 2", pGameObject);

	pGameObject = pManagement->Clone_GameObject(L"BaseObject_Drum");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->Set_Position(_vec3(140.f, 6.f, -130.f));
	pLayer->Add_GameObject(L"BaseObject 3", pGameObject);

	pGameObject = pManagement->Clone_GameObject(L"Item_Battery");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Item_Battery 1", pGameObject);
	pGameObject->Set_Position(_vec3(150.f, 15.f, -150.f));


	pGameObject = pManagement->Clone_GameObject(L"Item_Battery");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Item_Battery 2", pGameObject);
	pGameObject->Set_Position(_vec3(150.f, 20.f, -246.f));

	pGameObject = pManagement->Clone_GameObject(L"Item_Battery");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Item_Battery 3", pGameObject);
	pGameObject->Set_Position(_vec3(150.f, 20.f, -245.f));

	pGameObject = pManagement->Clone_GameObject(L"Item_Battery");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Item_Battery 4", pGameObject);
	pGameObject->Set_Position(_vec3(120.f, 20.f, -215.f));

	pGameObject = pManagement->Clone_GameObject(L"Item_Battery");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Item_Battery 5", pGameObject);
	pGameObject->Set_Position(_vec3(122.f, 20.f, -215.f));

	pGameObject = pManagement->Clone_GameObject(L"Item_Battery");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Item_Battery 6", pGameObject);
	pGameObject->Set_Position(_vec3(120.f, 20.f, -214.f));

	pGameObject = pManagement->Clone_GameObject(L"Item_Battery");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Item_Battery 7", pGameObject);
	pGameObject->Set_Position(_vec3(118.f, 20.f, -215.f));

	pGameObject = pManagement->Clone_GameObject(L"Item_HealthKit");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Item_HealthKit 1", pGameObject);
	pGameObject->Set_Position(_vec3(130.f, 20.f, -215.f));

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}


HRESULT CMainStageA::Add_ColiderBox_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}

	pGameObject = CTriggerBox::Create(m_pDevice, _vec3(-1.f, -2.f, -1.f), _vec3(1.f, 2.f, 1.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->Set_Position(_vec3(63.f, 12.f, -111.f));
	pLayer->Add_GameObject(L"TriggerBoxA", pGameObject);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CMainStageA::Add_Environment_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}


	pGameObject = pManagement->Clone_GameObject(L"SkyBoxA");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"SkyBoxA", pGameObject);

	pGameObject = pManagement->Clone_GameObject(L"StageALight");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DirLight", pGameObject), E_FAIL);

	pGameObject = pManagement->Clone_GameObject(L"MapA");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MapA", pGameObject), E_FAIL);

	pGameObject = m_pNaviController = CNaviMeshController::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NaviMesh", pGameObject), E_FAIL);
	
	m_pNaviController->Set_NaviMesh_From_File(L"../../Resource/Meshes/Navi/map1.json");

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CMainStageA::Add_Weapon_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CMainStageA::Add_Enemy_Layer_WaveB(const _tchar* pLayerTag)
{


	return S_OK;
}

HRESULT CMainStageA::Add_Enemy_Layer_WaveC(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Get_Layer(pLayerTag);
	Engine::CLayer* pAILayer = Get_Layer(L"AILayer");

	Engine::CGameObject* pGameObject = nullptr;

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr || pLayer == nullptr)
	{
		return E_FAIL;
	}

	//적 스폰 파트
	//개미귀신
	pGameObject = pManagement->Clone_GameObject(L"EnemyAntlion");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->Set_Position(_vec3(116.f, 8.f, -150.f));
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Antlion3", pGameObject), E_FAIL);

	pGameObject = pManagement->Clone_GameObject(L"EnemyAntlion");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->Set_Position(_vec3(140.f, 8.f, -160.f));
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Antlion4", pGameObject), E_FAIL);

	//기본 적 AI파트
	//개미귀신
	pGameObject = CBaseAI_Attacker::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_ControlUnit(dynamic_cast<CDynamicObject*>(pLayer->Get_GameObject(L"Antlion3")));
	dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_Target(m_mapLayer.find(L"PlayerLayer")->second->Find_GameObject(L"Player"));
	FAILED_CHECK_RETURN(pAILayer->Add_GameObject(L"Antlion1A3", pGameObject), E_FAIL);

	pGameObject = CBaseAI_Attacker::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_ControlUnit(dynamic_cast<CDynamicObject*>(pLayer->Get_GameObject(L"Antlion4")));
	dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_Target(m_mapLayer.find(L"PlayerLayer")->second->Find_GameObject(L"Player"));
	FAILED_CHECK_RETURN(pAILayer->Add_GameObject(L"Antlion1A4", pGameObject), E_FAIL);


	return S_OK;
}

HRESULT CMainStageA::Add_Boss_Layer_WaveBoss(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Get_Layer(pLayerTag);
	Engine::CLayer* pAILayer = Get_Layer(L"AILayer");

	Engine::CGameObject* pGameObject = nullptr;

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}

	//적 스폰 파트
	pGameObject = pManagement->Clone_GameObject(L"BossHunter");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pGameObject->Set_Position(_vec3(135.f, 12.f, -125.f));
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BossHunter", pGameObject), E_FAIL);

	//헌터 AI
	pGameObject = CBaseAI_Attacker::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_ControlUnit(dynamic_cast<CDynamicObject*>(pLayer->Get_GameObject(L"BossHunter")));
	dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_Target(m_mapLayer.find(L"PlayerLayer")->second->Find_GameObject(L"Player"));
	FAILED_CHECK_RETURN(pAILayer->Add_GameObject(L"BossHunterAI", pGameObject), E_FAIL);




	return S_OK;
}

void CMainStageA::Change_Scene(ESceneType eSceneType)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return;
	}

	pManagement->Setup_CurrentScene((_uint)ESceneType::SCENE_STAGE3, CMainStageC::Create(m_pDevice));

}

CMainStageA* CMainStageA::Create(_Device pDevice)
{
	CMainStageA* pInstance = new CMainStageA(pDevice);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}



void CMainStageA::Free()
{
	Engine::CScene::Free();
}
