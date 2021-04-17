#include "framework.h"
#include "EnemyHunter.h"

#include "DynamicMesh.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "ControlSupport.h"

#include "ProjFlechette.h"
#include "ProjPulseAmmo.h"

_uint CEnemyHunter::m_iPattonBShoot = 0;

CEnemyHunter::CEnemyHunter(_Device pDevice)
	: CDynamicObject(pDevice)
	, m_fPattonCooltime(0.f)
	, m_fPattonInterval(2.f)
{
	m_fRecognizeRange = 155.f;
	m_fMoveRange = 95.f;
	m_fAttackRange = 30.f;

	m_fHitboxSize = 6.25;

	m_fMoveSpeed = 6.f;
	m_fRotateSpeed = 2.5f;	//multiple

	m_iMaxHP = 100;

	m_iDamage = 3;

	m_eAction = eHunterAction::Idle;
	m_ePrevAction = m_eAction;	

	m_ePatton = eHunterPatton::Idle;

}

CEnemyHunter::CEnemyHunter(const CEnemyHunter& other)
	: CDynamicObject(other)
	, m_fPattonCooltime(other.m_fPattonCooltime)
	, m_fPattonInterval(other.m_fPattonInterval)
{
	m_eAction = eHunterAction::Idle;
	m_ePrevAction = m_eAction;

	m_ePatton = eHunterPatton::Idle;

	m_bAttackLock = false;
	m_bPattonLock = false;
	m_bShootLock = false;

}

HRESULT CEnemyHunter::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CEnemyHunter::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(BASE_ENEMY_REDUCION_VECTOR);

	m_pMeshCom->Set_AnimationSet((_uint)m_eAction);

	m_vCorePos = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);

	return S_OK;
}

_int CEnemyHunter::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);



	return NO_EVENT;
}

_int CEnemyHunter::LateUpdate_GameObject(const _float& fDeltaTime)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	m_pTransformCom->Rotation(Engine::ROTATION::ROT_Y, m_fRotate);
	m_fRotate = 0.f;

	eHunterAction eActon = (eHunterAction)m_pMeshCom->Get_NowAnimationNumber();

	if ((m_pMeshCom->Get_NowAnimationNumber() == (_uint)eHunterAction::Walk_N)
		&& m_pMeshCom->End_Animation_Sequence())
	{
		m_vOriPos = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
		m_bEndChecker = true;
		m_vCorePos.y = m_vOriPos.y;
		m_pTransformCom->Set_Pos(m_vCorePos);

		m_eAction = eHunterAction::Walk_N;
		m_pMeshCom->Force_Change_AnimationSet((_uint)m_eAction);

	}
	else if (m_eAction != m_ePrevAction && m_ePrevAction == eHunterAction::Walk_N)
	{
		m_pMeshCom->Force_Change_AnimationSet((_uint)m_eAction);
	} 

	m_pTransformCom->Update_Component();

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);

	m_fTime = fDeltaTime;

	return NO_EVENT;
}

HRESULT CEnemyHunter::Render_GameObject(void)
{
	eHunterAction eAction = (eHunterAction)m_pMeshCom->Get_NowAnimationNumber();

	if ((_uint)m_eAction != m_pMeshCom->Get_NowAnimationNumber())
		m_pMeshCom->Set_AnimationSet((_uint)m_eAction);
	
	m_pTransformCom->LateUpdate_Component(0.f);

	m_pMeshCom->Play_AnimationSet(m_fTime * m_fAnimationSpeed);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	m_pMeshCom->Render_Meshes();


	//아래 눈깔 기준 정점
	m_vCorePos = _vec3(0.f, 0.f, 0.f);
	
	_mat matWorld = m_pMeshCom->Get_FrameByName("MiniStrider_low_eye_bone")->CombinedTranformationMatrix;

	matWorld = matWorld * m_pTransformCom->Get_TransformDescription().matWorld;

	D3DXVec3TransformCoord(&m_vCorePos, &m_vCorePos, &matWorld);
	//정점계산 끝

	m_pColliderCom->Render_Collider(eType, &m_vCorePos, g_bViewCollider);

	m_ePrevAction = m_eAction;

	return S_OK;
}

CEnemyHunter* CEnemyHunter::Create(_Device pDevice)
{
	CEnemyHunter* pInstance = new CEnemyHunter(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CEnemyHunter::Clone(void* pArg)
{
	CEnemyHunter* pClone = new CEnemyHunter(*this);
	if (pClone == nullptr ||(FAILED(pClone->Ready_GameObject_Clone(pArg))))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CEnemyHunter");		
	
		Safe_Release(pClone);

		return nullptr;
	}

	return pClone;
}

_uint CEnemyHunter::Get_Patton()
{
	return _uint(m_ePatton);
}


void CEnemyHunter::Set_Animation(_uint iIndex)
{
	m_pMeshCom->Set_AnimationSet(iIndex);
}

_bool CEnemyHunter::End_Animation_State_Force()
{
	return (m_pMeshCom->Get_NowAnimationNumber() == (_uint)m_eAction && m_pMeshCom->End_AnimationSet());
}


void CEnemyHunter::Go_Stright(_float fDeltaTime)
{
	m_pManagement->Play_Sound(L"ol01_hunterdoublestep.wav", m_eChannel);
	m_eAction = eHunterAction::Walk_N;
}

void CEnemyHunter::Go_Side(_float fDeltaTime, eAlign pAlign)
{
	if (pAlign == eAlign::LEFT)
		m_eAction = eHunterAction::Dodge_W;
	if (pAlign == eAlign::RIGHT)
		m_eAction = eHunterAction::Dodge_E;
}

void CEnemyHunter::Do_Run(_float fDeltaTime)
{
	m_fMoveSpeed = 9.f;
	m_fAnimationSpeed = 1.5f;
}

void CEnemyHunter::Do_Walk(_float fDeltaTime)
{
	m_fMoveSpeed = 6.f;
	m_fAnimationSpeed = 1.f;
}

void CEnemyHunter::Do_Rotate(_float fDeltaTime, eAlign pAlign)
{
	m_fRotate = (((int)pAlign - 0.5f) * 2.f) * fDeltaTime;
}

void CEnemyHunter::Do_Attack(_float fDeltaTime, _uint iPatton)
{
	if (m_bAttackLock == false && m_bPattonLock == false)
	{
		m_fNowAttackTime = 0.f;

		m_bAttackLock = true;
	}

	if ((eHunterPatton)iPatton == eHunterPatton::PattonA)
	{
		m_ePatton = eHunterPatton::PattonA;
		PattonA();
	}

	else if (m_fPattonCooltime <= 0.f)
	{		
		if ((eHunterPatton)iPatton == eHunterPatton::PattonB)
		{
			m_ePatton = eHunterPatton::PattonB;
			PattonB();
		}
		else if ((eHunterPatton)iPatton == eHunterPatton::PattonC)
		{
			m_ePatton = eHunterPatton::PattonC;
			PattonC();
		}
	}
	else if (m_eAction != eHunterAction::Hunter_Angry && m_bPattonLock)
	{
		Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
		if (pManagement == nullptr)
		{
			return;
		}
		pManagement->Stop_Sound(m_eChannel);
		pManagement->Play_Sound(L"ol01_hunter_preattackvx.wav", m_eChannel);

		m_eAction = eHunterAction::Hunter_Angry;
		m_ePatton = eHunterPatton::Idle;
	}

	if (m_bPattonLock == true)
	{
		m_fPattonCooltime += fDeltaTime;
	}

	if (m_fPattonCooltime >= m_fPattonInterval)
	{
		m_fPattonCooltime = 0.f;
		m_bPattonLock = false;
	}
}

void CEnemyHunter::Do_Idle(_float fDeltaTime)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return;
	}
	pManagement->Play_Sound(L"ol01_hunter_preattackvx02.wav", m_eChannel);


	m_eAction = eHunterAction::Idle;
	m_ePatton = eHunterPatton::Idle;
}

void CEnemyHunter::Do_Spawn(_float fDeltaTime)
{
	m_eAction = eHunterAction::Drop_Down;
}

void CEnemyHunter::Do_Dead(_float fDeltaTime)
{
	m_eAction = eHunterAction::Death;

	CDynamicObject::Do_Dead(fDeltaTime);
}

_bool CEnemyHunter::Do_Dodge(_float fDeltatime)
{
	if ((m_pMeshCom->Get_NowAnimationNumber() == (_uint)eHunterAction::Dodge_E ||
		m_pMeshCom->Get_NowAnimationNumber() == (_uint)eHunterAction::Dodge_W
		)
		&& m_pMeshCom->End_AnimationSet())
	{
		m_vOriPos = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
		m_bEndChecker = true;
		m_vCorePos.y = m_vOriPos.y;
		m_pTransformCom->Set_Pos(m_vCorePos);

		m_eAction = eHunterAction::Hunter_Angry;
		m_pMeshCom->Set_AnimationSet((_uint)m_eAction);
		
		return true;
	}
	return false;
}
//근접 공격
void CEnemyHunter::PattonA()
{
	if (m_fNowAttackTime >= m_fAttackInterval)
	{
		m_fNowAttackTime = 0.f;
		m_bAttackHitEnable = true;
	}

	if (m_eAction != eHunterAction::Melee_L && m_eAction != eHunterAction::Melee_R)
	{
		m_eAction = (eHunterAction)(rand() % 2 + (_uint)eHunterAction::Melee_L);
		m_fAnimationSpeed = 1.0f;
	}

	m_fNowAttackTime += m_fTime;

}

//플레쉐트 발사
void CEnemyHunter::PattonB()
{
	if (m_fNowAttackTime <= 0.f)
	{	
		Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
		if (pManagement == nullptr)
		{
			return;
		}


		//플레쉐트 출력파트
		Engine::CGameObject* pObject = pManagement->Clone_GameObject(L"Projectile_Flechette");
		NULL_CHECK(pObject);

		_vec3 vPos, vDir;
		_mat matWorld;
	
		vPos = m_vCorePos;
		vDir = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK);

		D3DXVec3Normalize(&vDir, &vDir);

		dynamic_cast<CProjFlechette*>(pObject)->Set_Position(vPos);
		dynamic_cast<CProjFlechette*>(pObject)->Set_Direction(vDir);
		dynamic_cast<CProjFlechette*>(pObject)->Set_TargetState(eTargetState::ToPlayer);

		TCHAR tObjName[128] = L"";
		TCHAR tObjData[] = L"HunterFlechette %d";
		swprintf_s(tObjName, tObjData, m_iPattonBShoot++);

		if (!FAILED(pManagement->Get_NowScene()->Get_Layer(L"WeaponLayer")->Add_GameObject(tObjName, pObject)))
		{
			pManagement->Stop_Sound(Engine::SOUND_CHANNELID::EFFECTD);
			pManagement->Play_Sound(L"ol01_hunter_impacts02.wav", Engine::SOUND_CHANNELID::EFFECTD);

			m_bAttackHitEnable = true;
		}
		else
		{
			Safe_Release(pObject);
		}

	}
	m_ePatton = eHunterPatton::PattonB;
	m_eAction = eHunterAction::RangeAttack;

	m_fNowAttackTime += m_fTime;

	if (End_Animation_State_Force() || m_fNowAttackTime >= m_fAttackInterval)
	{
		m_bAttackLock = false;
		m_bPattonLock = true;
	}
	
}
//기관총 발사 - 3발
void CEnemyHunter::PattonC()
{
	if (m_fNowAttackTime <= 0.f || (_uint)(m_fNowAttackTime*10) == 3 || (_uint)(m_fNowAttackTime * 10)  == 6)
	{
		if (m_bShootLock == false)
		{
			m_bShootLock = true;


			Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
			if (pManagement == nullptr)
			{
				return;
			}

			//기관총 출력파트
			Engine::CGameObject* pObject = pManagement->Clone_GameObject(L"Projectile_PulseAmmo");
			NULL_CHECK(pObject);

			_vec3 vPos, vDir;
			_mat matWorld;

			vPos = m_vCorePos;
			vDir = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK);

			D3DXVec3Normalize(&vDir, &vDir);

			dynamic_cast<CProjPulseAmmo*>(pObject)->Set_Position(vPos);
			dynamic_cast<CProjPulseAmmo*>(pObject)->Set_Direction(vDir);
			dynamic_cast<CProjPulseAmmo*>(pObject)->Set_TargetState(eTargetState::ToPlayer);

			TCHAR tObjName[128] = L"";
			TCHAR tObjData[] = L"HunterPulseAmmo %d";
			swprintf_s(tObjName, tObjData, m_iPattonBShoot++);

			if (!FAILED(pManagement->Get_NowScene()->Get_Layer(L"WeaponLayer")->Add_GameObject(tObjName, pObject)))
			{
				pManagement->Stop_Sound(m_eChannel);
				pManagement->Play_Sound(L"ar2_fire1.wav", m_eChannel);

				m_bAttackHitEnable = true;
			}
			else
			{
				Safe_Release(pObject);
			}
		}
	}
	else
	{
		m_bShootLock = false;
	}
	m_ePatton = eHunterPatton::PattonC;
	m_eAction = eHunterAction::Shoot_Minigun;

	m_fNowAttackTime += m_fTime;

	if (m_fNowAttackTime >= m_fAttackInterval)
	{
		m_bAttackLock = false;
		m_bPattonLock = true;
	}

}

HRESULT CEnemyHunter::Add_Component()
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	Engine::CComponent* pComponent = nullptr;

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"Hunter"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	//서포트 컴포넌트
	pComponent = m_pSupportCom = Engine::CControlSupportUnit::Create(m_pDevice);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Support", pComponent);

	//콜라이더
	pComponent = m_pColliderCom = Engine::CSphereCollider::Create(m_pDevice, &_vec3(0.f, 0.f, 0.f), m_fHitboxSize);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Collider", pComponent);

	switch (rand() % 5)
	{
	case 1:
		m_eChannel = Engine::SOUND_CHANNELID::ENEMYA;
		break;

	case 2:
		m_eChannel = Engine::SOUND_CHANNELID::ENEMYB;
		break;

	case 3:
		m_eChannel = Engine::SOUND_CHANNELID::ENEMYC;
		break;

	case 4:
		m_eChannel = Engine::SOUND_CHANNELID::ENEMYD;
		break;

	default:
		m_eChannel = Engine::SOUND_CHANNELID::ENEMY;
		break;
	}

	return S_OK;
}

void CEnemyHunter::Free()
{
	CDynamicObject::Free();
}
