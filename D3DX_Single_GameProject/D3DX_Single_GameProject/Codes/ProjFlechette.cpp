#include "framework.h"
#include "ProjFlechette.h"

#include "StaticMesh.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "ControlSupport.h"

#include "ExplosionEffect.h"

////쓸 수 있는 인자들
///from StaticObject:
//_uint m_fWeight;
//_float m_fLifeTime;
//_float m_fSpeed;
//eForceType m_eForceType;
// _float m_fHitboxSize;
//
//Engine::CStaticMesh* m_pMeshCom;
//Engine::CTransform* m_pTransformCom;
//Engine::CControlSupportUnit* m_pSupportCom;
//Engine::CSphereCollider* m_pColliderCom;

///from BaseProjectile:
//_uint m_iDamage;
//_float m_fSplashRadius;
//_vec3 m_vStartPos;
//_vec3 m_vDirection;
//_float m_fRotate;

CProjFlechette::CProjFlechette(_Device pDevice)
	: CBaseProjectile(pDevice)
	, m_bExplosion(false)
{
	m_fWeight = 0;
	m_fLifeTime = 30.f;
	m_fSpeed = 30.f;
	m_eForceType = eForceType::PULL;
	m_fHitboxSize = 2.5f;		//테스트용. 실제로는 좀 더 작게
	
	m_iDamage = 10;
	m_fSplashRadius = 15.f;
	m_vStartPos = _vec3(0.f, 0.f, 0.f);
	m_vDirection = _vec3(0.f, 0.f, 1.f);
	m_fRotate = 0.f;

}

CProjFlechette::CProjFlechette(const CProjFlechette& other)
	: CBaseProjectile(other)
	, m_bExplosion(false)
{
}

HRESULT CProjFlechette::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CProjFlechette::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);	

	return S_OK;
}

_int CProjFlechette::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	Engine::CGameObject::Update_GameObject(fDeltaTime);
	m_pTransformCom->Set_Scale(RESET_VECTOR);
	m_pTransformCom->Update_Component();



	m_fLifeTime -= fDeltaTime;

	if (m_bExplosion && m_pEffect != nullptr)
	{
		if (m_pEffect->LateUpdate_GameObject(fDeltaTime) == OBJ_DEAD)
		{
			pManagement->Release_Data_To_MemoryPool(L"ExploPool", m_pEffect);
			m_pEffect = nullptr;

			m_bClearDead = true;

			Set_Dead();
		}
	}
	if (m_fLifeTime < 0.f || m_bAmmoBreak == true)
	{
		m_iDamage = 0;

		if (m_pEffect == nullptr)
		{
			m_pEffect = dynamic_cast<CExplosionEffect*>(pManagement->Get_Data_From_MemoryPool(L"ExploPool"));
			m_pEffect->Set_Position(Get_Position());
			dynamic_cast<CExplosionEffect*>(m_pEffect)->Set_Ready();

			m_bExplosion = true;
		}

		return OBJ_DEAD;
	}

	return NO_EVENT;
}

_int CProjFlechette::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (m_bExplosion == true)
	{
		return OBJ_DEAD;
	}

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}	

	m_fRotate += (fDeltaTime*10.f);

	m_pTransformCom->Move_Pos(&m_vDirection, m_fSpeed, fDeltaTime);


	m_pTransformCom->Update_Component(fDeltaTime);

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);

	return NO_EVENT;
}

HRESULT CProjFlechette::Render_GameObject(void)
{
	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	_mat matWorld = m_pTransformCom->Get_TransformDescription().matWorld;

	m_pTransformCom->Set_Scale(ITEM_REDUCION_VECTOR);

	_vec2 vCalivY = _vec2(m_vDirection.x, m_vDirection.z);
	_vec3 vLookO = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK);
	_vec2 vLookY = _vec2(vLookO.x, vLookO.z);
	_float fCY = D3DXVec2Dot(&vCalivY, &vLookY) / (D3DXVec2Length(&vCalivY) * D3DXVec2Length(&vLookY));

	//좌측이면
	//if (m_vDirection.x < 0.f)
	//{
	//		fCY *= -1;
	//	}
	m_pTransformCom->Rotation(Engine::ROTATION::ROT_Y, acos(fCY));

	m_pTransformCom->Update_Component();
	m_pTransformCom->LateUpdate_Component(0.f);

	m_pMeshCom->Render_Meshes();


	m_pColliderCom->Render_Collider(eType, &matWorld, g_bViewCollider);

	return S_OK;
}

HRESULT CProjFlechette::Add_Component()
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

	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"Throwable_Flechette"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Mesh", pComponent);

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

CProjFlechette* CProjFlechette::Create(_Device pDevice)
{
	CProjFlechette* pInstance = new CProjFlechette(pDevice);

	if (FAILED(pInstance->Ready_GameObject(0)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CProjFlechette::Clone(void* pArg = nullptr)
{
	CProjFlechette* pClone = new CProjFlechette(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CProjFlechette");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CProjFlechette::Free()
{
	if (m_pEffect != nullptr)
	{
		Safe_Release(m_pEffect);
	}

	CBaseProjectile::Free();
}
