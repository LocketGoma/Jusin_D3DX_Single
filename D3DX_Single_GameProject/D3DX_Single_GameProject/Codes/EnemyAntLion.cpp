#include "framework.h"
#include "EnemyAntLion.h"

#include "DynamicMesh.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "ControlSupport.h"

CEnemyAntLion::CEnemyAntLion(_Device pDevice)
	: CDynamicObject(pDevice)
	, m_ePatton(eAntLionPatton::Idle)
{
	m_fRecognizeRange = 35.f;
	m_fMoveRange = 25.f;
	m_fAttackRange = 8.f;

	m_fAttackInterval = 1.6f;

	m_fHitboxSize = 5.25f;

	m_fMoveSpeed = 5.f;
	m_fRotateSpeed = 2.5f;

	m_iMaxHP = 20;


	m_iDamage = 3;
}

//분명 행렬처리 관련때문에 "무언가를" 깊은 복사 해줘야 한다고 했던거 같은데
CEnemyAntLion::CEnemyAntLion(const CEnemyAntLion& other)
	: CDynamicObject(other)		
	, m_ePatton(other.m_ePatton)
{
	m_eAction = eAntLionAction::DigDie;
	m_ePrevAction = m_ePrevAction;
}

HRESULT CEnemyAntLion::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CEnemyAntLion::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(BASE_ENEMY_REDUCION_VECTOR);

	m_pMeshCom->Set_AnimationSet((_uint)m_eAction);

	return S_OK;
}

_int CEnemyAntLion::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	m_fNowAttackTime += fDeltaTime;

	//if (m_pSupportCom->Collision_Picking(g_hWnd, m_pColliderCom, m_pTransformCom))

	return NO_EVENT;
}

_int CEnemyAntLion::LateUpdate_GameObject(const _float& fDeltaTime)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	m_pTransformCom->Rotation(Engine::ROTATION::ROT_Y,m_fRotate);
	m_fRotate = 0.f;

	//트랜스폼 업데이트 전 수행할 것들
	if ((m_eAction != m_ePrevAction && m_ePrevAction == eAntLionAction::Run) || (m_pMeshCom->Get_NowAnimationNumber() == (_uint)eAntLionAction::Run && m_pMeshCom->End_Animation_Sequence()&& m_bEndChecker == false))
	{
		vOriPos = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
		m_bEndChecker = true;
		m_vCorePos.y = vOriPos.y;
		m_pTransformCom->Set_Pos(m_vCorePos);

		m_pMeshCom->Force_Change_AnimationSet((_uint)m_eAction);
	}
	else
	{
		m_bEndChecker = false;
	}
	m_pTransformCom->Update_Component(fDeltaTime);
	
	//트랜스폼 업데이트 후 수행할 것들
	if (m_eAction == eAntLionAction::DigOut)
	{
		m_pTransformCom->Set_Pos(vOriPos);
	}

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);

	m_fTime = fDeltaTime;

	return NO_EVENT;
}

HRESULT CEnemyAntLion::Render_GameObject(void)
{
	m_pMeshCom->Set_AnimationSet((_uint)m_eAction);

	m_pTransformCom->LateUpdate_Component(0.f);

	if (m_eAction == eAntLionAction::Run)
	{
		m_fAnimationSpeed = 0.5f;
	}
	else
	{
		m_fAnimationSpeed = 1.0f;
	}

	m_pMeshCom->Play_AnimationSet(m_fTime * m_fAnimationSpeed);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	m_pMeshCom->Render_Meshes();
	
	//가슴 기준 정점
	m_vCorePos = _vec3(0.f, 0.f, 0.f);

	_mat matWorld = m_pMeshCom->Get_FrameByName("Antlion_Body_Bone")->CombinedTranformationMatrix;

	matWorld = matWorld * m_pTransformCom->Get_TransformDescription().matWorld;

	D3DXVec3TransformCoord(&m_vCorePos, &m_vCorePos, &matWorld);
	//정점계산 끝

	m_pColliderCom->Render_Collider(eType, &m_vCorePos);

	m_ePrevAction = m_eAction;

	return S_OK;
}

CEnemyAntLion* CEnemyAntLion::Create(_Device pDevice)
{
	CEnemyAntLion* pInstance = new CEnemyAntLion(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CEnemyAntLion::Clone(void* pArg)
{
	CEnemyAntLion* pClone = new CEnemyAntLion(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CEnemyAntLion");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

_uint CEnemyAntLion::Get_Patton()
{
	return (_uint)m_ePatton;
}

_bool CEnemyAntLion::Do_Dodge(_float fDeltatime)
{
	//회피 없음
	return false;
}

_bool CEnemyAntLion::End_Animation_State_Force()
{
	return (m_pMeshCom->Get_NowAnimationNumber() == (_uint)m_eAction && m_pMeshCom->End_AnimationSet());
}

void CEnemyAntLion::Set_Animation(_uint iIndex)
{
	m_pMeshCom->Set_AnimationSet(iIndex);
}

void CEnemyAntLion::Go_Stright(_float fDeltaTime)
{
	//Corepos 이용하기.
	_vec3 vLook = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK);
	D3DXVec3Normalize(&vLook, &vLook);

	m_eAction = eAntLionAction::Run;

}

void CEnemyAntLion::Go_Side(_float fDeltaTime, eAlign pAlign)
{
	if (pAlign == eAlign::LEFT)
		m_pTransformCom->Move_Pos(&m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_RIGHT), m_fMoveSpeed, fDeltaTime);
	if (pAlign == eAlign::RIGHT)
		m_pTransformCom->Move_Pos(&m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_RIGHT), -m_fMoveSpeed, fDeltaTime);
}

void CEnemyAntLion::Do_Run(_float fDeltaTime)
{
	m_fMoveSpeed = 10.f;
	m_fAnimationSpeed = 1.5f;
}

void CEnemyAntLion::Do_Walk(_float fDeltaTime)
{
	m_fMoveSpeed = 5.f;
	m_fAnimationSpeed = 1.f;
}

void CEnemyAntLion::Do_Rotate(_float fDeltaTime, eAlign pAlign)
{
	//+=? =?
	m_fRotate = (((int)pAlign - 0.5f) * 2.f)*fDeltaTime * m_fRotateSpeed;
}

void CEnemyAntLion::Do_Attack(_float fDeltaTime, _uint iPatton)
{
	if (m_fNowAttackTime >= m_fAttackInterval)
	{
		m_fNowAttackTime = 0.f;
		m_bAttackHitEnable = true;
	}

	if (m_eAction != eAntLionAction::AttackA && m_eAction != eAntLionAction::AttackB) 
	{
		m_ePatton = eAntLionPatton::PattonA;
		m_eAction = (eAntLionAction)(rand() % 2 + (_uint)eAntLionAction::AttackB);
		m_fAnimationSpeed = 1.0f;
	}
}

void CEnemyAntLion::Do_Idle(_float fDeltaTime)
{
	m_fNowAttackTime = 0.f;
	m_bAttackHitEnable = false;

	m_eAction = eAntLionAction::Idle;
	m_fAnimationSpeed = 1.0f;

	m_ePatton = eAntLionPatton::Idle;
}

void CEnemyAntLion::Do_Spawn(_float fDeltaTime)
{
	vOriPos = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);

	if (m_eAction == eAntLionAction::Idle)
	{
		return;
	}

	if (m_eAction == eAntLionAction::DigOut)
	{
		m_pTransformCom->Set_Pos(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS) - (m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK) * 8.5f / BASE_ENEMY_REDUCION_SIZE));
	}
	m_eAction = eAntLionAction::DigOut;
	//m_pMeshCom->Set_AnimationSet((_uint)eAntLionAction::DigOut);

	if (m_pMeshCom->Get_NowAnimationNumber() == (_uint)eAntLionAction::DigOut && m_pMeshCom->End_AnimationSet())
	{
		m_eAction = eAntLionAction::Idle;
		m_pMeshCom->Force_Change_AnimationSet((_uint)eAntLionAction::Idle);		

		m_pTransformCom->Set_Pos(vOriPos);
		
	}
}

void CEnemyAntLion::Do_Dead(_float fDeltaTime)
{
	m_eAction = eAntLionAction::RagDoll;
	//m_pMeshCom->Set_AnimationSet((_uint)eAntLionAction::RagDoll);
}

HRESULT CEnemyAntLion::Add_Component(void)
{
	auto pManagement = Engine::CManagement::Get_Instance();
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"Antlion"));	
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

	return S_OK;
}

void CEnemyAntLion::Free()
{
	CDynamicObject::Free();
}
