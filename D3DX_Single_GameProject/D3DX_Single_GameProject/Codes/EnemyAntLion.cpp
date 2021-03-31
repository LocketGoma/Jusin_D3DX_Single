#include "framework.h"
#include "EnemyAntLion.h"

#include "DynamicMesh.h"
#include "Transform.h"
#include "ControlSupport.h"

CEnemyAntLion::CEnemyAntLion(_Device pDevice)
	: CDynamicObject(pDevice)
{
}

//분명 행렬처리 관련때문에 "무언가를" 깊은 복사 해줘야 한다고 했던거 같은데
CEnemyAntLion::CEnemyAntLion(const CEnemyAntLion& other)
	: CDynamicObject(other)	
{
}

HRESULT CEnemyAntLion::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CEnemyAntLion::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMeshCom->Set_AnimationSet((_uint)eAntLionAction::Idle);

	return S_OK;
}

_int CEnemyAntLion::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	return NO_EVENT;
}

_int CEnemyAntLion::LateUpdate_GameObject(const _float& fDeltaTime)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	m_pTransformCom->Update_Component(fDeltaTime);

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_UI, this);

	return NO_EVENT;
}

HRESULT CEnemyAntLion::Render_GameObject(void)
{
	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	m_pMeshCom->Render_Meshes();

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
	return nullptr;
}

Engine::CGameObject* CEnemyAntLion::Clone(void* pArg)
{
	return nullptr;
}

void CEnemyAntLion::Set_Animation(_uint iIndex)
{
}

void CEnemyAntLion::Go_Stright(_float fDeltaTime)
{
	m_pTransformCom->Move_Pos(&m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK), 10.f, fDeltaTime);
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
	m_fMoveSpeed = 15.f;
	m_fAnimationSpeed = 1.5f;
}

void CEnemyAntLion::Do_Walk(_float fDeltaTime)
{
	m_fMoveSpeed = 10.f;
	m_fAnimationSpeed = 1.f;
}

void CEnemyAntLion::Do_Rotate(_float fDeltaTime)
{
}

void CEnemyAntLion::Do_Attack(_float fDeltaTime)
{
	m_pMeshCom->Set_AnimationSet(rand() % 2 + (_uint)eAntLionAction::AttackB);
}

void CEnemyAntLion::Do_Idle(_float fDeltaTime)
{
}

void CEnemyAntLion::Do_Spawn(_float fDeltaTime)
{
	m_pMeshCom->Set_AnimationSet((_uint)eAntLionAction::DigOut);
}

void CEnemyAntLion::Do_Dead(_float fDeltaTime)
{
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"AntLion"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	return S_OK;
}

void CEnemyAntLion::Free()
{
}
