#include "framework.h"
#include "ProjShotgunPellet.h"

#include "StaticMesh.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "ControlSupport.h"

#include "AmmoParticle.h"

CProjShotgunPellet::CProjShotgunPellet(_Device pDevice)
	: CBaseProjectile(pDevice)
{
	m_fWeight = 0;
	m_fLifeTime = 1.5f;
	m_fSpeed = 100.f;
	m_eForceType = eForceType::NONE;
	m_fHitboxSize = 1.5f;		//테스트용. 실제로는 좀 더 작게

	m_iDamage = 1;
	m_fSplashRadius = m_fHitboxSize;
	m_vStartPos = _vec3(0.f, 0.f, 0.f);
	m_vDirection = _vec3(0.f, 0.f, 1.f);
	m_fRotate = 0.f;
}

CProjShotgunPellet::CProjShotgunPellet(const CProjShotgunPellet& other)
	: CBaseProjectile(other)
{
}

HRESULT CProjShotgunPellet::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CProjShotgunPellet::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Info(Engine::TRANSFORM_INFO::INFO_LOOK, &m_vDirection);

	m_vAmmoSize = _vec3(1.6f, 0.8f, 1.f);

	return S_OK;
}

_int CProjShotgunPellet::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	if (m_fLifeTime < 0.f)
	{
		m_bDead = true;

		return OBJ_DEAD;
	}
	if (m_bAmmoBreak == true)
	{
		m_iDamage = 0;
		if (m_pParticle == nullptr)
		{
			m_pParticle = dynamic_cast<CAmmoParticle*>(pManagement->Get_Data_From_MemoryPool(L"AmmoBreakPool"));
		}

		m_pParticle->Set_Position(Get_Position());
		if (m_pParticle->LateUpdate_GameObject(fDeltaTime) == OBJ_DEAD)
		{
			pManagement->Release_Data_To_MemoryPool(L"AmmoBreakPool", m_pParticle);
			m_pParticle = nullptr;

			Set_Dead();
		}

		return OBJ_DEAD;
	}

	m_fLifeTime -= fDeltaTime;

	m_fRotate += fDeltaTime * 1440.f;



	return NO_EVENT;
}

_int CProjShotgunPellet::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (m_bAmmoBreak == true)
	{
		return OBJ_DEAD;
	}

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	m_pTransformCom->Move_Pos(&m_vDirection, m_fSpeed, fDeltaTime);

	m_pTransformCom->Update_Component();

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_EFFECT, this);

	return _int();
}

HRESULT CProjShotgunPellet::Render_GameObject(void)
{
	m_pTransformCom->LateUpdate_Component(0.f);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	_mat matWorld = m_pTransformCom->Get_TransformDescription().matWorld;

	m_pColliderCom->Render_Collider(eType, &matWorld, g_bViewCollider);

	if (FAILED(m_pTextureCom->Set_Texture(1)))
	{
		return E_FAIL;
	}
	m_pTransformCom->Set_Scale(m_vAmmoSize);
	_vec2 vCalivY = _vec2(m_vDirection.x, m_vDirection.z);
	_vec3 vLookO = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK);
	_vec2 vLookY = _vec2(vLookO.x, vLookO.z);
	_float fCY = D3DXVec2Dot(&vCalivY, &vLookY) / (D3DXVec2Length(&vCalivY) * D3DXVec2Length(&vLookY));

	_float fXRad = D3DXToRadian(90.f);
	//좌측이면
	if (m_vDirection.x < 0.f)
	{
		fCY *= -1;
		fXRad *= -1;
	}
	m_pTransformCom->Rotation(Engine::ROTATION::ROT_Y, acos(fCY));

	m_pTransformCom->Rotation(Engine::ROTATION::ROT_X, fXRad);
	m_pTransformCom->Update_Component();
	m_pTransformCom->LateUpdate_Component(0.f);
	if (FAILED(m_pBufferCom[(_uint)eEffectAxis::AXIS_X]->Render_Buffer()))
	{
		return E_FAIL;
	}

	_mat matAWorld, matScale, matRotate, matRotateX, matRotateY, matTrans;
	_vec3 vPivot = _vec3(m_vDirection.x, 0.f, m_vDirection.z);

	D3DXMatrixScaling(&matScale, m_vAmmoSize.x, m_vAmmoSize.y, m_vAmmoSize.z);
	D3DXMatrixRotationX(&matRotateX, fXRad);
	D3DXMatrixRotationY(&matRotateY, acos(fCY));
	D3DXMatrixRotationAxis(&matRotate, &vPivot, D3DXToRadian(90.f + m_fRotate));
	D3DXMatrixTranslation(&matTrans, Get_Position().x, Get_Position().y, Get_Position().z);

	matAWorld = matScale * matRotateX * matRotateY * matRotate * matTrans;

	m_pDevice->SetTransform(D3DTS_WORLD, &matAWorld);

	if (FAILED(m_pBufferCom[(_uint)eEffectAxis::AXIS_Z]->Render_Buffer()))
	{
		return E_FAIL;
	}

	m_pTransformCom->Reset_Rotation();
	m_pTransformCom->Set_Scale(RESET_VECTOR);
	m_pTransformCom->Update_Component();


	return S_OK;
}

HRESULT CProjShotgunPellet::Add_Component()
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

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_Tracer_PHYS"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Texture", pComponent);


	return S_OK;
}

CProjShotgunPellet* CProjShotgunPellet::Create(_Device pDevice)
{
	CProjShotgunPellet* pInstance = new CProjShotgunPellet(pDevice);

	if (FAILED(pInstance->Ready_GameObject(0)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CProjShotgunPellet::Clone(void* pArg)
{
	CProjShotgunPellet* pClone = new CProjShotgunPellet(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CProjShotgunPellet");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CProjShotgunPellet::Free()
{
	Safe_Release(m_pBufferCom[0]);
	Safe_Release(m_pBufferCom[1]);
	Safe_Release(m_pBufferCom[2]);
	Safe_Release(m_pTextureCom);

	if (m_pParticle != nullptr)
	{
		Safe_Release(m_pParticle);
	}

	CBaseProjectile::Free();
}
