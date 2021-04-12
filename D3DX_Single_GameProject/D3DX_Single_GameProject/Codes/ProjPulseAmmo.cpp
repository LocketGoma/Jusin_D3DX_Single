#include "framework.h"
#include "ProjPulseAmmo.h"

#include "StaticMesh.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "ControlSupport.h"

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

CProjPulseAmmo::CProjPulseAmmo(_Device pDevice)
	: CBaseProjectile(pDevice)
{
	m_fWeight = 0;
	m_fLifeTime = 0.5f;
	m_fSpeed = 50.f;
	m_eForceType = eForceType::NONE;
	m_fHitboxSize = 2.5f;		//테스트용. 실제로는 좀 더 작게

	m_iDamage = 2;
	m_fSplashRadius = m_fHitboxSize;
	m_vStartPos = _vec3(0.f, 0.f, 0.f);
	m_vDirection = _vec3(0.f, 0.f, 1.f);
	m_fRotate = 0.f;


}

CProjPulseAmmo::CProjPulseAmmo(const CProjPulseAmmo& other)
	: CBaseProjectile(other)
{
}

HRESULT CProjPulseAmmo::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CProjPulseAmmo::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Info(Engine::TRANSFORM_INFO::INFO_LOOK, &m_vDirection);

	m_vAmmoSize = _vec3(0.2f, 0.4f, 1.f);

	return S_OK;
}

_int CProjPulseAmmo::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	m_fLifeTime -= fDeltaTime;

	if (m_fLifeTime < 0.f)
	{
		m_bDead = true;

		return OBJ_DEAD;
	}

	return NO_EVENT;
}

_int CProjPulseAmmo::LateUpdate_GameObject(const _float& fDeltaTime)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	m_fRotate += (fDeltaTime * 10.f);

	m_pTransformCom->Move_Pos(&m_vDirection, m_fSpeed, fDeltaTime);

	m_pTransformCom->Rotation(Engine::ROTATION::ROT_Z, m_fRotate);

	m_pTransformCom->Update_Component();

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_EFFECT, this);

	return _int();
}

HRESULT CProjPulseAmmo::Render_GameObject(void)
{
	m_pTransformCom->LateUpdate_Component(0.f);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	_mat matWorld = m_pTransformCom->Get_TransformDescription().matWorld;

	m_pColliderCom->Render_Collider(eType, &matWorld);


	if (FAILED(m_pTextureCom->Set_Texture()))
	{
		return E_FAIL;
	}

	m_pTransformCom->Set_Scale(m_vAmmoSize);

	m_pTransformCom->Rotation(Engine::ROTATION::ROT_X, D3DXToRadian(-90.f));
	m_pTransformCom->Update_Component();
	m_pTransformCom->LateUpdate_Component(0.f);
	if (FAILED(m_pBufferCom[(_uint)eEffectAxis::AXIS_X]->Render_Buffer()))
	{
		return E_FAIL;
	}

	m_pTransformCom->Rotation(Engine::ROTATION::ROT_Z, D3DXToRadian(90.f));
	m_pTransformCom->Update_Component();
	m_pTransformCom->LateUpdate_Component(0.f);

	if (FAILED(m_pBufferCom[(_uint)eEffectAxis::AXIS_Z]->Render_Buffer()))
	{
		return E_FAIL;
	}


	m_pTransformCom->Reset_Rotation();
	m_pTransformCom->Set_Scale(RESET_VECTOR);
	m_pTransformCom->Update_Component();


	return S_OK;
}

HRESULT CProjPulseAmmo::Add_Component()
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

	//서포트 컴포넌트
	pComponent = m_pSupportCom = Engine::CControlSupportUnit::Create(m_pDevice);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Support", pComponent);

	//콜라이더
	pComponent = m_pColliderCom = Engine::CSphereCollider::Create(m_pDevice, &_vec3(0.f, 0.f, 0.f), m_fHitboxSize);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Collider", pComponent);

	pComponent = m_pBufferCom[(_uint)eEffectAxis::AXIS_X] = dynamic_cast<Engine::CVTXRectTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Buffer_X", pComponent);
	pComponent = m_pBufferCom[(_uint)eEffectAxis::AXIS_Y] = nullptr;
	pComponent = m_pBufferCom[(_uint)eEffectAxis::AXIS_Z] = dynamic_cast<Engine::CVTXRectTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Buffer_Z", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_Tracer_Pulse"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Texture", pComponent);


	return S_OK;
}

CProjPulseAmmo* CProjPulseAmmo::Create(_Device pDevice)
{
	CProjPulseAmmo* pInstance = new CProjPulseAmmo(pDevice);

	if (FAILED(pInstance->Ready_GameObject(0)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CProjPulseAmmo::Clone(void* pArg)
{
	CProjPulseAmmo* pClone = new CProjPulseAmmo(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CProjPulseAmmo");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CProjPulseAmmo::Free()
{
	Safe_Release(m_pBufferCom[0]);
	Safe_Release(m_pBufferCom[1]);
	Safe_Release(m_pBufferCom[2]);
	Safe_Release(m_pTextureCom);

	CBaseProjectile::Free();
}
