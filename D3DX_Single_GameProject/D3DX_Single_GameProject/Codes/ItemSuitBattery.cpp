#include "framework.h"
#include "ItemSuitBattery.h"

#include "StaticMesh.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "ControlSupport.h"
#include "Shader.h"

#include "Player.h"

CItemSuitBattery::CItemSuitBattery(_Device pDevice)
	: CBaseItem(pDevice)
{
	m_iAmount = 10;

	m_fWeight = 1;
	m_fLifeTime = 1.f;
	m_fSpeed = 0.f;
	m_eForceType = eForceType::PULL;
	m_fHitboxSize = 1.5f;

	m_vStartPos = _vec3(0.f, 0.f, 0.f);
	m_vDirection = _vec3(0.f, 0.f, 1.f);

}

CItemSuitBattery::CItemSuitBattery(const CItemSuitBattery& other)
	: CBaseItem(other)
{
}

HRESULT CItemSuitBattery::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CItemSuitBattery::Ready_GameObject_Clone(void* pArg)
{
	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}
	Set_Size(BASE_ENEMY_REDUCION_VECTOR);

	return S_OK;
}

_int CItemSuitBattery::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);	
	m_pTransformCom->Update_Component();


	m_vDirection = m_vDirection + _vec3(0.f, -0.1f, 0.f);
	D3DXVec3Normalize(&m_vDirection, &m_vDirection);
	m_fSpeed *= D3DXVec2Length(&_vec2(m_vDirection.x, m_vDirection.z));
	m_fSpeed /= m_fWeight;
	m_vDirection = _vec3(m_vDirection.x, 0.f, m_vDirection.z);

	m_fGravitionSpeed += m_fGravitionPower * fDeltaTime;

	return NO_EVENT;
}

_int CItemSuitBattery::LateUpdate_GameObject(const _float& fDeltaTime)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	m_pTransformCom->Move_Pos(&m_vDirection, m_fSpeed, fDeltaTime);
	m_pTransformCom->Move_Pos(&_vec3(0.0f, -1.f, 0.0f), m_fGravitionSpeed, fDeltaTime);

	m_pTransformCom->Update_Component();

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);

	return NO_EVENT;

}

HRESULT CItemSuitBattery::Render_GameObject(void)
{
	m_pTransformCom->LateUpdate_Component(0.f);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	//쉐이더 처리
	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK_RETURN(pEffect, E_FAIL);
	pEffect->AddRef();

	FAILED_CHECK_RETURN(Setup_ConstantTable(pEffect), E_FAIL);

	_uint	iPassMax = 0;

	pEffect->Begin(&iPassMax, 0);		// 1인자 : 현재 쉐이더 파일이 갖고 있는 pass의 최대 개수, 2인자 : 시작하는 방식(default)
	//if (m_bClearDead)
	pEffect->BeginPass(1);

	//m_pMeshCom->Render_Meshes();

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);

	//쉐이더 처리 끝

	m_pColliderCom->Render_Collider(eType, &Get_Position(), g_bViewCollider);
	

	return S_OK;
}

CItemSuitBattery* CItemSuitBattery::Create(_Device pDevice)
{
	CItemSuitBattery* pInstance = new CItemSuitBattery(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;


	return nullptr;
}

Engine::CGameObject* CItemSuitBattery::Clone(void* pArg)
{
	CItemSuitBattery* pClone = new CItemSuitBattery(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CItemSuitBattery");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

HRESULT CItemSuitBattery::Add_Component()
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


	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"Item_Battery"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	

	//서포트 컴포넌트
	pComponent = m_pSupportCom = Engine::CControlSupportUnit::Create(m_pDevice);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Support", pComponent);

	//콜라이더
	pComponent = m_pColliderCom = Engine::CSphereCollider::Create(m_pDevice, m_pMeshCom->Get_VtxPos(), ITEM_REDUCION_VECTOR, m_pMeshCom->Get_VtxCnt(), m_pMeshCom->Get_Stride(), m_fHitboxSize);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Collider", pComponent);

	//쉐이더
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(pManagement->Clone_Prototype(L"Shader_Dissolve"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

void CItemSuitBattery::Free()
{
	CBaseItem::Free();
}

HRESULT CItemSuitBattery::Interaction(Engine::CGameObject* pTarget)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pTarget);
	if (pPlayer == nullptr)
	{
		return E_FAIL;
	}
	pPlayer->Restore_Shield(m_iAmount);

	Set_Dead();

	return S_OK;
}
