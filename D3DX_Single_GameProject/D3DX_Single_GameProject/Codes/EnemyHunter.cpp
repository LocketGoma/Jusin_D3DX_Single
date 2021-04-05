#include "framework.h"
#include "EnemyHunter.h"

#include "DynamicMesh.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "ControlSupport.h"

CEnemyHunter::CEnemyHunter(_Device pDevice)
	: CDynamicObject(pDevice)
{
	m_fRecognizeRange = 35.f;
	m_fMoveRange = 30.f;
	m_fAttackRange = 15.f;

	m_fHitboxSize = 6.25;

	m_fMoveSpeed = 8.f;

	m_iDamage = 3;

}

CEnemyHunter::CEnemyHunter(const CEnemyHunter& other)
	: CDynamicObject(other)
{
	eAction = eHunterAction::Idle;
	ePrevAction = eAction;

	ePatton = eHunterPatton::Idle;	
}

HRESULT CEnemyHunter::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CEnemyHunter::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(BASE_ENEMY_REDUCION_VECTOR);

	m_pMeshCom->Set_AnimationSet((_uint)eAction);

	return S_OK;
}

_int CEnemyHunter::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	return NO_EVENT;
}

_int CEnemyHunter::LateUpdate_GameObject(const _float& fDeltaTime)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	m_pTransformCom->Rotation(Engine::ROTATION::ROT_Y, m_fRotate);
	m_fRotate = 0.f;

	m_pTransformCom->Update_Component();

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);

	m_fTime = fDeltaTime;

	return NO_EVENT;
}

HRESULT CEnemyHunter::Render_GameObject(void)
{
	m_pMeshCom->Set_AnimationSet((_uint)eAction);

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

	m_pColliderCom->Render_Collider(eType, &m_vCorePos);

	ePrevAction = eAction;

	return S_OK;
}

void CEnemyHunter::Set_Position(_vec3 vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CEnemyHunter::Set_Size(_vec3 vSize)
{
	m_pTransformCom->Set_Scale(vSize);
}

_vec3 CEnemyHunter::Get_Position()
{
	return m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
}

_vec3 CEnemyHunter::Get_Size()
{
	return m_pTransformCom->Get_TransformDescription().vScale;
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
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CEnemyHunter");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CEnemyHunter::Set_Animation(_uint iIndex)
{
	m_pMeshCom->Set_AnimationSet(iIndex);
}

_bool CEnemyHunter::End_Animation_State_Force()
{
	return (m_pMeshCom->Get_NowAnimationNumber() == (_uint)eAction && m_pMeshCom->End_AnimationSet());
}


void CEnemyHunter::Go_Stright(_float fDeltaTime)
{

	eAction = eHunterAction::Walk_N;
}

void CEnemyHunter::Go_Side(_float fDeltaTime, eAlign pAlign)
{
	if (pAlign == eAlign::LEFT)
		eAction = eHunterAction::Dodge_W;
	if (pAlign == eAlign::RIGHT)
		eAction = eHunterAction::Dodge_E;
}

void CEnemyHunter::Do_Run(_float fDeltaTime)
{
	m_fMoveSpeed = 12.f;
	m_fAnimationSpeed = 1.5f;
}

void CEnemyHunter::Do_Walk(_float fDeltaTime)
{
	m_fMoveSpeed = 8.f;
	m_fAnimationSpeed = 1.f;
}

void CEnemyHunter::Do_Rotate(_float fDeltaTime, eAlign pAlign)
{
	m_fRotate = (((int)pAlign - 0.5f) * 2.f) * fDeltaTime;
}

void CEnemyHunter::Do_Attack(_float fDeltaTime)
{
}

void CEnemyHunter::Do_Idle(_float fDeltaTime)
{
	eAction = eHunterAction::Idle;
	ePatton = eHunterPatton::Idle;
}

void CEnemyHunter::Do_Spawn(_float fDeltaTime)
{
}

void CEnemyHunter::Do_Dead(_float fDeltaTime)
{
	eAction = eHunterAction::Death;
}



void CEnemyHunter::PattonA()
{
}

void CEnemyHunter::PattonB()
{
}

void CEnemyHunter::PattonC()
{
}

HRESULT CEnemyHunter::Add_Component()
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

	return S_OK;
}

void CEnemyHunter::Free()
{
	CDynamicObject::Free();
}
