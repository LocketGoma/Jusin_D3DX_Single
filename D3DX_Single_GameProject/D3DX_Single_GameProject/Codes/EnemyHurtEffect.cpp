#include "framework.h"
#include "EnemyHurtEffect.h"

#include "VTXRectTexture.h"
#include "Transform.h"
#include "Texture.h"


CEnemyHurtEffect::CEnemyHurtEffect(_Device pDevice)
	:CBaseEffect(pDevice)
	, m_vStartSize(ZERO_VECTOR)
	,m_vWeightSize(ZERO_VECTOR)
	,m_fWeight(0.f)
	, m_iEffectTexture(0)
{
	m_fLifetime = 0.15f;
}

CEnemyHurtEffect::CEnemyHurtEffect(const CEnemyHurtEffect& other)
	: CBaseEffect(other)
	, m_vStartSize(other.m_vStartSize)
	, m_vWeightSize(m_vWeightSize)
	, m_fWeight(other.m_fWeight)
	, m_iEffectTexture(0)
{
}

HRESULT CEnemyHurtEffect::Ready_GameObject(void)
{
	return S_OK;
}

HRESULT CEnemyHurtEffect::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_vStartSize = _vec3(2.5f, 2.5f, 2.5f);
	m_vWeightSize = _vec3(3.5f, 3.5f, 3.5f);

	Set_Size(m_vStartSize);

	return S_OK;
}

_int CEnemyHurtEffect::Update_GameObject(const _float& fDeltaTime)
{
	return NO_EVENT;
}

_int CEnemyHurtEffect::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (m_fNowTime >= m_fLifetime)
	{
		Set_Size(m_vStartSize);

		m_iEffectTexture = rand() % 4;

		m_fNowTime = 0.f;
		return OBJ_DEAD;
	}
	m_fNowTime += fDeltaTime;
	m_fWeight += fDeltaTime * 180.f * (1 / m_fLifetime);
	Set_Size(m_vStartSize + (m_vWeightSize * sin(m_fWeight)));


	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	m_pTransformCom->Update_Component();

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_EFFECT, this);

	return NO_EVENT;
}

HRESULT CEnemyHurtEffect::Render_GameObject(void)
{
	_mat		matBill, matView, matWorld;

	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	matWorld = m_pTransformCom->Get_TransformDescription().matWorld;

	m_pTransformCom->Set_WorldMatrix((matBill * matWorld));

	m_pTransformCom->LateUpdate_Component();

	if (FAILED(CGameObject::Render_GameObject()))
	{
		return E_FAIL;
	}


	if (FAILED(m_pTextureCom->Set_Texture(m_iEffectTexture)))
	{
		return E_FAIL;
	}

	if (FAILED(m_pBufferCom->Render_Buffer()))
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CEnemyHurtEffect::Add_Component()
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	Engine::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CVTXRectTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_EnemyHurt"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Texture", pComponent);

	m_iEffectTexture = rand() % 4;

	return S_OK;
}

void CEnemyHurtEffect::Free()
{
	CBaseEffect::Free();
}

CEnemyHurtEffect* CEnemyHurtEffect::Create(_Device pDevice)
{
	CEnemyHurtEffect* pInstance = new CEnemyHurtEffect(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CEnemyHurtEffect::Clone(void* pArg)
{
	CEnemyHurtEffect* pClone = new CEnemyHurtEffect(*this);

	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CEnemyHurtEffect");

		return nullptr;
	}
	if (FAILED(pClone->Ready_GameObject_Clone()))
	{
		Safe_Release(pClone);
	}


	return pClone;
}
