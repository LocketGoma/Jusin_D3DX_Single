#include "framework.h"
#include "EnemyManhack.h"

#include "DynamicMesh.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "ControlSupport.h"

CEnemyManhack::CEnemyManhack(_Device pDevice)
	: CDynamicObject(pDevice)
	, m_ePatton(eManhackPatton::Idle)
	, m_fVerticalRotate(0.f)
{
	m_fRecognizeRange = 65.f;
	m_fMoveRange = 65.f;
	m_fAttackRange = 10.f;		//의미있나?

	m_fHitboxSize = 3.5f;

	m_fMoveSpeed = 15.5f;
	m_fRotateSpeed = 4.5f;

	m_iMaxHP = 20;

	m_eForceType = eForceType::PULL;

	m_bState = false;

	m_iDamage = 5;
}

CEnemyManhack::CEnemyManhack(const CEnemyManhack& other)
	: CDynamicObject(other)
	, m_ePatton(other.m_ePatton)
	, m_fVerticalRotate(other.m_fVerticalRotate)
{
	m_eAction = eManhackAction::Netual;
	m_ePrevAction = m_eAction;
}

HRESULT CEnemyManhack::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CEnemyManhack::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(BASE_ENEMY_REDUCION_VECTOR);

	m_pMeshCom->Set_AnimationSet((_uint)m_eAction);

	return S_OK;
}

_int CEnemyManhack::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);


	m_fNowAttackTime += fDeltaTime;

	

	return NO_EVENT;
}

_int CEnemyManhack::LateUpdate_GameObject(const _float& fDeltaTime)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	m_pTransformCom->Move_Pos(&m_vImForceDirection, m_fImForcePower, fDeltaTime);
	m_fImForcePower -= (m_fImForcePower*fDeltaTime);
	if (m_fImForcePower < 1.5f)
	{
		m_vImForceDirection = ZERO_VECTOR;
		m_fImForcePower = 0.f;
	}


	m_pTransformCom->Rotation(Engine::ROTATION::ROT_Y, m_fRotate);
	m_pTransformCom->Single_Rotation(Engine::ROTATION::ROT_X, m_fVerticalRotate);
	m_fRotate = 0.f;

	m_pTransformCom->Update_Component(fDeltaTime);

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_ALPHA, this);

	m_fTime = fDeltaTime;

	return NO_EVENT;
}

HRESULT CEnemyManhack::Render_GameObject(void)
{
	m_pMeshCom->Set_AnimationSet((_uint)m_eAction);

	m_pTransformCom->LateUpdate_Component(0.f);

	m_pMeshCom->Play_AnimationSet(m_fTime * m_fAnimationSpeed);

	m_pMeshCom->Render_Meshes();

	m_vCorePos = Get_Position();

	m_pColliderCom->Render_Collider(eType, &m_vCorePos, g_bViewCollider);

	m_ePrevAction = m_eAction;

	return S_OK;
}

void CEnemyManhack::Go_Stright(_float fDeltaTime)
{
	_vec3 vLook = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);

	m_pTransformCom->Move_Pos(&vLook, m_fMoveSpeed, fDeltaTime);

	if (m_eAction != eManhackAction::Fly1 && m_eAction != eManhackAction::Fly2 && m_eAction != eManhackAction::Fly3)
	{
		m_eAction = (eManhackAction)(rand() % 3 + (_uint)eManhackAction::Fly1);
	}

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return;
	}

	_uint iSelect = rand() % 2;
	switch (iSelect)
	{
	case 0:
		pManagement->Play_Sound(L"mh_engine_loop1.wav", m_eChannel);
		break;
	case 1:
		pManagement->Play_Sound(L"mh_engine_loop2.wav", m_eChannel);
		break;
	}

}

//필요한가?
void CEnemyManhack::Go_Side(_float fDeltaTime, eAlign pAlign)
{
}

void CEnemyManhack::Do_Run(_float fDeltaTime)
{
	m_fMoveSpeed = 15.f;
	m_fAnimationSpeed = 1.5f;
}

void CEnemyManhack::Do_Walk(_float fDeltaTime)
{
	m_fMoveSpeed = 7.5f;
	m_fAnimationSpeed = 1.f;
}

void CEnemyManhack::Do_Rotate(_float fDeltaTime, eAlign pAlign)
{
	if (pAlign == eAlign::LEFT || pAlign == eAlign::RIGHT)
		m_fRotate = (((int)pAlign - 0.5f) * 2.f) * fDeltaTime * m_fRotateSpeed;

	if (pAlign == eAlign::UP || pAlign == eAlign::DOWN)
	{
		m_fVerticalRotate += ((((int)pAlign - (int)eAlign::DOWN) - 0.5f) * 2.f) * fDeltaTime * m_fRotateSpeed;

		if (cos(m_fVerticalRotate) <= cos(D3DXToRadian(60.f)))
		{
			m_fVerticalRotate -= ((((int)pAlign - (int)eAlign::DOWN) - 0.5f) * 2.f) * fDeltaTime * m_fRotateSpeed;
		}

		m_ePreHoriAlign = pAlign;

	}
	else if (pAlign == eAlign::IDLE)
	{
		_float fA = cos(m_fVerticalRotate);
		_float fB = cos(D3DXToRadian(85.f));

		if (cos(m_fVerticalRotate) <= cos(D3DXToRadian(5.f)))
		{
			m_fVerticalRotate -= ((((int)m_ePreHoriAlign - (int)eAlign::DOWN) - 0.5f) * 2.f) * fDeltaTime * m_fRotateSpeed;
		}
	}

}

void CEnemyManhack::Do_Attack(_float fDeltaTime, _uint iPatton)
{
	//얘 공격도 그냥 처박는거라서...
	m_ePatton = eManhackPatton::PattonA;
	Go_Stright(fDeltaTime);


	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return;
	}

	_uint iSelect = rand() % 5;
	switch (iSelect)
	{
	case 0:
		pManagement->Play_Sound(L"mh_grind1.wav", m_eChannel);
		break;
	case 1:
		pManagement->Play_Sound(L"mh_grind2.wav", m_eChannel);
		break;
	case 2:
		pManagement->Play_Sound(L"mh_grind3.wav", m_eChannel);
		break;
	case 3:
		pManagement->Play_Sound(L"mh_grind4.wav", m_eChannel);
		break;
	case 4:
		pManagement->Play_Sound(L"mh_grind5.wav", m_eChannel);
		break;
	default:
		break;
	}

}

void CEnemyManhack::Do_Idle(_float fDeltaTime)
{
	m_ePatton = eManhackPatton::Idle;
}

void CEnemyManhack::Do_Spawn(_float fDeltaTime)
{
	m_eAction = eManhackAction::Deploy;

	//if (m_pMeshCom->Get_NowAnimationNumber() == (_uint)eManhackAction::Deploy && m_pMeshCom->End_AnimationSet())
	//{
	//	m_eAction = eManhackAction::Fly1;
	//	m_pMeshCom->Force_Change_AnimationSet((_uint)m_eAction);
	//}

}

void CEnemyManhack::Do_Dead(_float fDeltaTime)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return;
	}
	_uint iSelect = rand() % 3;
	pManagement->Play_Sound(L"mh_gib.wav", Engine::SOUND_CHANNELID::ENEMYDEAD);
	switch (iSelect)
	{
	case 0:
		pManagement->Play_Sound(L"explode3.wav", Engine::SOUND_CHANNELID::EFFECTD);
		break;
	case 1:
		pManagement->Play_Sound(L"explode4.wav", Engine::SOUND_CHANNELID::EFFECTD);
		break;
	case 2:
		pManagement->Play_Sound(L"explode5.wav", Engine::SOUND_CHANNELID::EFFECTD);
		break;
	default:
		break;
	}
	


	CDynamicObject::Do_Dead(fDeltaTime);	
}

//상남자에게 회피란 없다
_bool CEnemyManhack::Do_Dodge(_float fDeltatime)
{
	return false;
}

void CEnemyManhack::Set_Animation(_uint iIndex)
{
	m_pMeshCom->Set_AnimationSet(iIndex);
}

_bool CEnemyManhack::End_Animation_State_Force()
{
	return (m_pMeshCom->Get_NowAnimationNumber() == (_uint)m_eAction && m_pMeshCom->End_AnimationSet());
}

_uint CEnemyManhack::Get_Patton()
{
	return (_uint)m_ePatton;
}

HRESULT CEnemyManhack::Add_Component(void)
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"Manhack"));
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

void CEnemyManhack::Free()
{
	CDynamicObject::Free();
}

CEnemyManhack* CEnemyManhack::Create(_Device pDevice)
{
	CEnemyManhack* pInstance = new CEnemyManhack(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CEnemyManhack::Clone(void* pArg)
{
	CEnemyManhack* pClone = new CEnemyManhack(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CEnemyManhack");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}
