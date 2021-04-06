#include "framework.h"
#include "BossStrider.h"

#include "DynamicMesh.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "ControlSupport.h"

CBossStrider::CBossStrider(_Device pDevice)
	: CDynamicObject(pDevice)
{
	m_fHitboxSize = 10.0;

}

CBossStrider::CBossStrider(const CBossStrider& other)
	: CDynamicObject(other)
{
	eAction = eStriderAction::idle_high;
	ePrevAction = eAction;

	ePatton = eStriderPatton::Idle;

	vCalibrationPos = _vec3(0.f, 28.f, 20.f);
}

HRESULT CBossStrider::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CBossStrider::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(BOSS_ENEMY_REDUCION_VECTOR);

	m_pMeshCom->Set_AnimationSet((_uint)eAction);

	return S_OK;
}

_int CBossStrider::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	return NO_EVENT;
}

_int CBossStrider::LateUpdate_GameObject(const _float& fDeltaTime)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	m_pTransformCom->Rotation(Engine::ROTATION::ROT_Y, m_fRotate);
	m_fRotate = 0.f;

	vOriPos = m_pTransformCom->Get_Info_RawData(Engine::TRANSFORM_INFO::INFO_POS);
	m_pTransformCom->Set_Pos(vOriPos + vCalibrationPos);
	m_pTransformCom->Update_Component();


	pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);
	
	m_fTime = fDeltaTime;

	return NO_EVENT;
}

HRESULT CBossStrider::Render_GameObject(void)
{
	m_pMeshCom->Set_AnimationSet((_uint)eAction);

	m_pTransformCom->LateUpdate_Component(0.f);


	m_pMeshCom->Play_AnimationSet(m_fTime * m_fAnimationSpeed);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	m_pMeshCom->Render_Meshes();

	////아래 눈깔 기준 정점
	m_vCorePos = _vec3(0.f, 0.f, 0.f);

	_mat matWorld = m_pMeshCom->Get_FrameByName("Combine_Strider_Neck_Bone")->CombinedTranformationMatrix;


	matWorld = matWorld * m_pTransformCom->Get_TransformDescription().matWorld;

	D3DXVec3TransformCoord(&m_vCorePos, &m_vCorePos, &matWorld);
	////정점계산 끝

	m_pColliderCom->Render_Collider(eType, &m_vCorePos);

	ePrevAction = eAction;

	m_pTransformCom->Set_Pos(vOriPos);		//애니메이션 업데이트에서도 썼던거땜에 여기서 갱신

	return S_OK;
}

void CBossStrider::Set_Position(_vec3 vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CBossStrider::Set_Size(_vec3 vSize)
{
	m_pTransformCom->Set_Scale(vSize);
}

_vec3 CBossStrider::Get_Position()
{
	return m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
}

_vec3 CBossStrider::Get_Size()
{
	return m_pTransformCom->Get_TransformDescription().vScale;
}

void CBossStrider::Go_Stright(_float fDeltaTime)
{
}

void CBossStrider::Go_Side(_float fDeltaTime, eAlign pAlign)
{
}

void CBossStrider::Do_Run(_float fDeltaTime)
{
}

void CBossStrider::Do_Walk(_float fDeltaTime)
{
}

void CBossStrider::Do_Rotate(_float fDeltaTime, eAlign pAlign)
{
}

void CBossStrider::Do_Attack(_float fDeltaTime, _uint iPatton)
{
}


void CBossStrider::Do_Idle(_float fDeltaTime)
{
}

void CBossStrider::Do_Spawn(_float fDeltaTime)
{
}

void CBossStrider::Do_Dead(_float fDeltaTime)
{
}


CBossStrider* CBossStrider::Create(_Device pDevice)
{
	CBossStrider* pInstance = new CBossStrider(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CBossStrider::Clone(void* pArg)
{
	CBossStrider* pClone = new CBossStrider(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CBossStrider");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

_bool CBossStrider::Do_Dodge(_float fDeltatime)
{
	return _bool();
}

_uint CBossStrider::Get_Patton()
{
	return _uint();
}


void CBossStrider::Set_Animation(_uint iIndex)
{
}

_bool CBossStrider::End_Animation_State_Force()
{
	return _bool();
}

void CBossStrider::PattonA()
{
}

void CBossStrider::PattonB()
{
}

void CBossStrider::PattonC()
{
}

HRESULT CBossStrider::Add_Component()
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"Strider"));
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

void CBossStrider::Free()
{
	CDynamicObject::Free();
}
