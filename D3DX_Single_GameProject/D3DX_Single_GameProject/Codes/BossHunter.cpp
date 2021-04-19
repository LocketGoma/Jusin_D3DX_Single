#include "framework.h"
#include "BossHunter.h"

#include "DynamicMesh.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "ControlSupport.h"

#include "ProjFlechette.h"
#include "ProjPulseAmmo.h"

#include "Shader.h"
#include "Texture.h"

_uint CBossHunter::m_iPattonBShoot = 0;

CBossHunter::CBossHunter(_Device pDevice)
	: CDynamicObject(pDevice)
	, m_fPattonCooltime(0.f)
	, m_fPattonInterval(1.8f)
	, m_bAttackLock( false)
	, m_bPattonLock( false)
	, m_bShootLock (false)
	
{
	m_fRecognizeRange = 95.f;
	m_fMoveRange = 45.f;
	m_fAttackRange = 20.f;

	m_fHitboxSize = 4.25;

	m_fMoveSpeed = 6.f;
	m_fRotateSpeed = 4.5f;	//multiple

	m_iMaxHP = 200;

	m_bState = false;

	m_iDamage = 5;

	m_fAnimationSpeed = 1.25f;

	m_eAction = eHunterAction::Idle;
	m_ePrevAction = m_eAction;

	m_fDeadTime = 0.0f;

	m_ePatton = eHunterPatton::Idle;

}


CBossHunter::CBossHunter(const CBossHunter& other)
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

HRESULT CBossHunter::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CBossHunter::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(BOSS_ENEMY_REDUCION_VECTOR);

	m_pMeshCom->Set_AnimationSet((_uint)m_eAction);

	//m_vCorePos = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);

	return S_OK;
}

_int CBossHunter::Update_GameObject(const _float& fDeltaTime)
{
	if (m_bDeadTrigger == true)
	{
		if (m_fDeadTime >= 1.0f)
		{
			m_bClearDead = true;
		}

		m_fDeadTime += fDeltaTime * 0.55f;

		return OBJ_DEAD;
	}


	Engine::CGameObject::Update_GameObject(fDeltaTime);
	CDynamicObject::Update_GameObject(fDeltaTime);


	return NO_EVENT;
}

_int CBossHunter::LateUpdate_GameObject(const _float& fDeltaTime)
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
		&& m_pMeshCom->End_Animation_Sequence() && m_bEndChecker == false)
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
	else
	{
		m_bEndChecker = false;
	}

	m_pTransformCom->Update_Component();

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_ALPHA, this);

	m_fTime = fDeltaTime;

	return NO_EVENT;
}

HRESULT CBossHunter::Render_GameObject(void)
{
	eHunterAction eAction = (eHunterAction)m_pMeshCom->Get_NowAnimationNumber();

	if ((_uint)m_eAction != m_pMeshCom->Get_NowAnimationNumber())
		m_pMeshCom->Set_AnimationSet((_uint)m_eAction);

	m_pTransformCom->LateUpdate_Component(0.f);

	m_pMeshCom->Play_AnimationSet(m_fTime * m_fAnimationSpeed);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	//쉐이더 처리
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK_RETURN(pEffect, E_FAIL);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(Setup_ConstantTable(pEffect), E_FAIL);

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	if (m_bDeadTrigger)
		pEffect->BeginPass(0);
	else
		pEffect->BeginPass(1);

	//m_pMeshCom->Render_Meshes();

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	//쉐이더 처리 끝

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

CBossHunter* CBossHunter::Create(_Device pDevice)
{
	CBossHunter* pInstance = new CBossHunter(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CBossHunter::Clone(void* pArg)
{
	CBossHunter* pClone = new CBossHunter(*this);
	if (pClone == nullptr || (FAILED(pClone->Ready_GameObject_Clone(pArg))))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CBossHunter");

		Safe_Release(pClone);

		return nullptr;
	}

	return pClone;
}

_uint CBossHunter::Get_Patton()
{
	return _uint(m_ePatton);
}


void CBossHunter::Set_Animation(_uint iIndex)
{
	m_pMeshCom->Set_AnimationSet(iIndex);
}

_bool CBossHunter::End_Animation_State_Force()
{
	return (m_pMeshCom->Get_NowAnimationNumber() == (_uint)m_eAction && m_pMeshCom->End_AnimationSet());
}


void CBossHunter::Go_Stright(_float fDeltaTime)
{
	m_bState = true;

	m_pManagement->Play_Sound(L"ol01_hunterdoublestep.wav", m_eChannel);
	m_eAction = eHunterAction::Walk_N;
}

void CBossHunter::Go_Side(_float fDeltaTime, eAlign pAlign)
{
	if (pAlign == eAlign::LEFT)
		m_eAction = eHunterAction::Dodge_W;
	if (pAlign == eAlign::RIGHT)
		m_eAction = eHunterAction::Dodge_E;
}

void CBossHunter::Do_Run(_float fDeltaTime)
{
	m_fMoveSpeed = 9.f;
	m_fAnimationSpeed = 1.5f;
}

void CBossHunter::Do_Walk(_float fDeltaTime)
{
	m_fMoveSpeed = 6.f;
	m_fAnimationSpeed = 1.f;
}

void CBossHunter::Do_Rotate(_float fDeltaTime, eAlign pAlign)
{
	m_fRotate = (((int)pAlign - 0.5f) * 2.f) * fDeltaTime;
}

void CBossHunter::Do_Attack(_float fDeltaTime, _uint iPatton)
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

void CBossHunter::Do_Idle(_float fDeltaTime)
{
	m_bState = true;

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return;
	}
	pManagement->Play_Sound(L"ol01_hunter_preattackvx02.wav", m_eChannel);


	m_eAction = eHunterAction::Idle;
	m_ePatton = eHunterPatton::Idle;
}

void CBossHunter::Do_Spawn(_float fDeltaTime)
{
	m_bState = true;

	m_eAction = eHunterAction::Drop_Down;
}

void CBossHunter::Do_Dead(_float fDeltaTime)
{
	m_eAction = eHunterAction::Death;

	m_bDeadTrigger = true;

	//CDynamicObject::Do_Dead(fDeltaTime);
}

_bool CBossHunter::Do_Dodge(_float fDeltatime)
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
void CBossHunter::PattonA()
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
void CBossHunter::PattonB()
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
void CBossHunter::PattonC()
{
	if (m_fNowAttackTime <= 0.f || (_uint)(m_fNowAttackTime * 10) == 3 || (_uint)(m_fNowAttackTime * 10) == 6)
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

HRESULT CBossHunter::Add_Component()
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"BossHunter"));
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

	//쉐이더
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(pManagement->Clone_Prototype(L"Shader_Dissolve"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pDesolveTextureCom = dynamic_cast<Engine::CTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_Dissolve"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Dissolve", pComponent);


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

void CBossHunter::Free()
{
	CDynamicObject::Free();
}
