#include "framework.h"
#include "TriggerBox.h"

#include "Collider.h"
#include "Transform.h"
#include "ControlSupport.h"

CTriggerBox::CTriggerBox(_Device pDevice)
	: CGameObject(pDevice)
	, m_pColliderCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pSupportCom(nullptr)
	, m_eType(Engine::COLIDETYPE::COL_FALSE)
	, m_fLength(0.f)
{
}

CTriggerBox::CTriggerBox(const CTriggerBox& other)
	: CGameObject(other)
	, m_pColliderCom(other.m_pColliderCom)
	, m_pTransformCom(other.m_pTransformCom)
	, m_pSupportCom(other.m_pSupportCom)
	, m_eType(other.m_eType)
	, m_fLength(other.m_fLength)
{
}

HRESULT CTriggerBox::Ready_GameObject(void)
{
	m_vMin = _vec3(-0.5f, -0.5f, -0.5f);
	m_vMax = _vec3(0.5f, 0.5f, 0.5f);


	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CTriggerBox::Ready_GameObject(_vec3 vMin, _vec3 vMax)
{
	m_vMin = vMin;
	m_vMax = vMax;


	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CTriggerBox::Ready_GameObject_Clone(void* pArg)
{
	m_vMin = _vec3(-0.5f, -0.5f, -0.5f);
	m_vMax = _vec3(0.5f, 0.5f, 0.5f);


	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CTriggerBox::Ready_GameObject_Clone(_vec3 vMin, _vec3 vMax)
{
	m_vMin = vMin;
	m_vMax = vMax;


	if (FAILED(Add_Component()))
	{
		return E_FAIL;
	}

	return S_OK;
}

_int CTriggerBox::Update_GameObject(const _float& fDeltaTime)
{
	m_fLength = D3DXVec2Length(&(_vec2(m_vMax.x, m_vMax.z) - _vec2(m_vMin.x, m_vMin.z)));

	return NO_EVENT;
}

_int CTriggerBox::LateUpdate_GameObject(const _float& fDeltaTime)
{

	if (g_bViewCollider == true)
	{
		Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
		if (nullptr == pManagement)
		{
			return MANAGER_OUT;
		}

		pManagement->Add_RenderList(Engine::RENDERID::RENDER_WIREFRAME, this);
	}

	m_pTransformCom->Update_Component();

	return NO_EVENT;
}

HRESULT CTriggerBox::Render_GameObject(void)
{
	m_pTransformCom->LateUpdate_Component(0.f);

	m_pColliderCom->Render_Collider(m_eType, &m_vPos, g_bViewCollider);

	return S_OK;
}

CTriggerBox* CTriggerBox::Create(_Device pDevice)
{
	CTriggerBox* pInstance = new CTriggerBox(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTriggerBox* CTriggerBox::Create(_Device pDevice, _vec3 vMin, _vec3 vMax)
{
	CTriggerBox* pInstance = new CTriggerBox(pDevice);

	if (FAILED(pInstance->Ready_GameObject(vMin,vMax)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CTriggerBox::Clone(_vec3 vMin, _vec3 vMax)
{
	CTriggerBox* pClone = new CTriggerBox(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CTriggerBox");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(vMin,vMax)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

Engine::CGameObject* CTriggerBox::Clone(void* pArg)
{
	CTriggerBox* pClone = new CTriggerBox(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CTriggerBox (Base Setting)");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}


HRESULT CTriggerBox::Add_Component()
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	Engine::CComponent* pComponent = nullptr;

	//콜라이더
	pComponent = m_pColliderCom = Engine::CCollider::Create(m_pDevice,&m_vMin,&m_vMax);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Collider", pComponent);

	//서포트 컴포넌트
	pComponent = m_pSupportCom = Engine::CControlSupportUnit::Create(m_pDevice);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Support", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	return S_OK;
}

void CTriggerBox::Free()
{
	Engine::CGameObject::Free();
}

void CTriggerBox::Set_Position(_vec3 vPos)
{
	m_pTransformCom->Set_Pos(vPos);
	m_vPos = vPos;
}

void CTriggerBox::Set_Size(_vec3 vSize)
{
	;
}

_vec3 CTriggerBox::Get_Position()
{
	return m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
}

_vec3 CTriggerBox::Get_Size()
{
	return _vec3();
}

Engine::CControlSupportUnit* CTriggerBox::Get_SupportUnit()
{
	return m_pSupportCom;
}

_bool CTriggerBox::Check_Collision(const _vec3* pSourcePos, const _float fSourceRadius)
{

	_bool bReturn = m_pSupportCom->Collision_Sphere(&(this->Get_Position()), m_fLength / 2, pSourcePos, fSourceRadius);

	bReturn == true ? m_eType = Engine::COLIDETYPE::COL_TRUE : m_eType = Engine::COLIDETYPE::COL_FALSE;

	return bReturn;
}
//고치셈
_vec3* CTriggerBox::Check_Passed(_vec3* pSourcePos)
{
	if (m_ePassType == ePassType::COL_PASSED)
	{
		return pSourcePos;
	}
	_vec3 vReturnPos;

	_vec3 vReturn = ((*pSourcePos)-m_vPos);
	D3DXVec3Normalize(&vReturn, &vReturn);
	
	vReturnPos = m_vPos + (vReturn) * ((m_fLength/2)+PLAYER_BASE_HITBOX);
	vReturnPos.y = pSourcePos->y;


	return &vReturnPos;
}

const Engine::CTransform* CTriggerBox::Get_Transform()
{
	return m_pTransformCom;
}

ePassType CTriggerBox::Get_PassType()
{
	return m_ePassType;
}

void CTriggerBox::Set_PassType(ePassType eType)
{
	m_ePassType = eType;
}

