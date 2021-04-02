#include "framework.h"
#include "EnemyAntLion.h"

#include "DynamicMesh.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "ControlSupport.h"

CEnemyAntLion::CEnemyAntLion(_Device pDevice)
	: CDynamicObject(pDevice)
{
	m_fRecognizeRange = 25.f;
	m_fMoveRange = 10.f;
	m_fAttackRange = 8.f;
}

//분명 행렬처리 관련때문에 "무언가를" 깊은 복사 해줘야 한다고 했던거 같은데
CEnemyAntLion::CEnemyAntLion(const CEnemyAntLion& other)
	: CDynamicObject(other)		
{
	eAction = eAntLionAction::DigDie;
}

HRESULT CEnemyAntLion::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CEnemyAntLion::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(BASE_ENEMY_REDUCION_VECTOR);

	m_pMeshCom->Set_AnimationSet((_uint)eAction);

	return S_OK;
}

_int CEnemyAntLion::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);

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

	m_pTransformCom->Update_Component(fDeltaTime);

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);

	m_fTime = fDeltaTime;

	return NO_EVENT;
}

HRESULT CEnemyAntLion::Render_GameObject(void)
{
	eAntLionAction Anumber = (eAntLionAction)m_pMeshCom->Get_NowAnimationNumber();

	m_pMeshCom->Set_AnimationSet((_uint)eAction);

	m_pTransformCom->LateUpdate_Component(0.f);

	m_pMeshCom->Play_AnimationSet(m_fTime);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	m_pMeshCom->Render_Meshes();
	m_pColliderCom->Render_Collider(eType, &(m_pTransformCom->Get_TransformDescription().matWorld));
	return S_OK;
}

void CEnemyAntLion::Set_Position(_vec3 vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CEnemyAntLion::Set_Size(_vec3 vSize)
{
	m_pTransformCom->Set_Scale(vSize);
}

_vec3 CEnemyAntLion::Get_Position()
{
	return m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
}

_vec3 CEnemyAntLion::Get_Size()
{
	return m_pTransformCom->Get_TransformDescription().vScale;	
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
//얘 돌려주면 애니메이션과 충돌판정이 정확히 들어감.
void CEnemyAntLion::Force_Update_Animation()
{
	m_pMeshCom->Set_AnimationSet((_uint)eAction);
	m_pMeshCom->Play_AnimationSet(0.f);	
	m_pMeshCom->Update_Meshes();
	m_pTransformCom->Update_Component(0.f);
}

void CEnemyAntLion::Set_Animation(_uint iIndex)
{
	m_pMeshCom->Set_AnimationSet(iIndex);
}

void CEnemyAntLion::Go_Stright(_float fDeltaTime)
{
	m_pTransformCom->Move_Pos(&m_pTransformCom->Get_Info_RawData(Engine::TRANSFORM_INFO::INFO_LOOK), m_fMoveSpeed/BASE_ENEMY_REDUCION_SIZE, fDeltaTime);
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
	m_fRotate = (((int)pAlign - 0.5f) * 2.f)*fDeltaTime;
}

void CEnemyAntLion::Do_Attack(_float fDeltaTime)
{
	if (eAction != eAntLionAction::AttackA && eAction != eAntLionAction::AttackB) 
	{
		eAction = (eAntLionAction)(rand() % 2 + (_uint)eAntLionAction::AttackB);
		m_pMeshCom->Set_AnimationSet((_uint)eAction);
	}
}

void CEnemyAntLion::Do_Idle(_float fDeltaTime)
{
	eAction = eAntLionAction::Idle;
	m_pMeshCom->Set_AnimationSet((_uint)eAntLionAction::Idle);
}

void CEnemyAntLion::Do_Spawn(_float fDeltaTime)
{
	eAction = eAntLionAction::DigOut;
	m_pMeshCom->Set_AnimationSet((_uint)eAntLionAction::DigOut);
}

void CEnemyAntLion::Do_Dead(_float fDeltaTime)
{
	eAction = eAntLionAction::RagDoll;
	m_pMeshCom->Set_AnimationSet((_uint)eAntLionAction::RagDoll);
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
	//pComponent = m_pColliderCom = Engine::CCollider::Create(m_pDevice,
	//	m_pMeshCom->Get_VtxPos(),
	//	m_pMeshCom->Get_VtxCnt(),
	//	m_pMeshCom->Get_Stride());
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Collider", pComponent);

	pComponent = m_pColliderCom = Engine::CSphereCollider::Create(m_pDevice, &_vec3(0.f, 0.f, 0.f), 4.25f);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;
}

void CEnemyAntLion::Free()
{
	CDynamicObject::Free();
}
