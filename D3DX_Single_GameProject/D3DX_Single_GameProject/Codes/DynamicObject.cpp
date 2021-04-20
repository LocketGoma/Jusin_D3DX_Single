#include "framework.h"
#include "DynamicObject.h"

#include "ControlSupport.h"
#include "Transform.h"
#include "DynamicMesh.h"

#include "BaseEffect.h"

CDynamicObject::CDynamicObject(_Device pDevice)
	: CBaseObject(pDevice)
	, m_fMoveSpeed(5.f)
	, m_fAnimationSpeed(1.0f)
	, m_fAttackInterval(1.f)
	, m_fRotate(0.f)
	, m_iMaxHP(10)
	, m_iHP(10)
	, m_fRecognizeRange(30.f)
	, m_fMoveRange(20.f)
	, m_fAttackRange(10.f)
	, m_fRotateSpeed(1.0f)
	, m_bEndChecker(false)
	, m_bStartMove(false)
	, m_bHurt(false)
	, m_vCorePos(_vec3(0.f,0.f,0.f))
	, m_bState(true)
	, m_eChannel(Engine::SOUND_CHANNELID::ENEMY)
	, m_bUseBaseEffect(true)
	, m_bClearDead(false)
	, m_bDeadTrigger(false)
	, m_fDeadTime(0.f)
{
	m_iHP = m_iMaxHP;
	m_eForceType = eForceType::NONE;

	m_iDamage = 5;
}

CDynamicObject::CDynamicObject(const CDynamicObject& other)
	: CBaseObject(other)
	, m_fMoveSpeed(other.m_fMoveSpeed)
	, m_fAnimationSpeed(other.m_fAnimationSpeed)
	, m_fAttackInterval(other.m_fAttackInterval)
	, m_fRotate(other.m_fRotate)
	, m_iMaxHP(other.m_iMaxHP)
	, m_iHP(other.m_iMaxHP)
	, m_fRecognizeRange(other.m_fRecognizeRange)
	, m_fMoveRange(other.m_fMoveRange)
	, m_fAttackRange(other.m_fAttackRange)
	, m_fNowAttackTime(0.f)
	, m_fRotateSpeed(other.m_fRotateSpeed)
	, m_bEndChecker(false)
	, m_bStartMove(false)
	, m_bHurt(false)
	, m_vCorePos(other.m_vCorePos)
	, m_bState(other.m_bState)
	, m_eChannel(other.m_eChannel)
	, m_bUseBaseEffect(other.m_bUseBaseEffect)
	, m_bClearDead(false)
	, m_bDeadTrigger(false)
	, m_fDeadTime(0.f)
{
}
//얘 돌려주면 애니메이션과 충돌판정이 정확히 들어감.
void CDynamicObject::Force_Update_Animation()
{
	m_pMeshCom->Play_AnimationSet(0.f);
	m_pMeshCom->Update_Meshes();
	//m_pTransformCom->Update_Component(0.f);
}

_bool CDynamicObject::End_Animation_State()
{
	return m_pMeshCom->End_AnimationSet();
}




_uint CDynamicObject::Get_NowHP()
{
	return m_iHP;
}

bool CDynamicObject::Hit_Attack(_uint iDamage)
{
	m_iHP -= iDamage;

	if (m_iHP <= 0)
	{
		return true;
	}
	return false;
}

bool CDynamicObject::Get_Clear_Dead_State()
{
	return m_bClearDead;
}

HRESULT CDynamicObject::Interaction(Engine::CGameObject* pTarget)
{
	return S_OK;
}

void CDynamicObject::Set_StartMove()
{
	m_bStartMove = true;
}

const _float CDynamicObject::Get_RecogRange()
{
	return m_fRecognizeRange;
}

const _float CDynamicObject::Get_MoveRange()
{
	return m_fMoveRange;
}

const _float CDynamicObject::Get_AttackRange()
{
	return m_fAttackRange;
}

const _float CDynamicObject::Get_CollideRange()
{
	return m_fHitboxSize;
}

const _vec3 CDynamicObject::Get_CorePos()
{
	return m_vCorePos;
}

_bool CDynamicObject::Check_RayCollision()
{
	return m_pSupportCom->Picking_Object_Dynamic(g_hWnd, m_pMeshCom, m_pTransformCom);
}

_bool CDynamicObject::Check_Attack_Collide(const _vec3* pSourcePos, const _float fSourceRadius)
{
	_bool bReturn = m_pSupportCom->Collision_Sphere(&(this->Get_Position()), this->m_fHitboxSize, pSourcePos, fSourceRadius);

	bReturn == true ? eType = Engine::COLIDETYPE::COL_TRUE : eType = Engine::COLIDETYPE::COL_FALSE;

	return bReturn;
}

_bool CDynamicObject::Check_RayCollision_By_CollisionSphere()
{
	if (m_pSupportCom->Picking_Object_Collider(g_hWnd, m_pColliderCom, m_vCorePos))
	{
		eType = Engine::COLIDETYPE::COL_INRANGE;
		return true;
	}
	else
	{
		eType = Engine::COLIDETYPE::COL_FALSE;
		return false;
	}
}

_bool CDynamicObject::Get_State()
{
	return m_bState;
}

void CDynamicObject::Set_Speed(_float fSpeed)
{
	m_fImForcePower = fSpeed;
}

void CDynamicObject::Set_Direction(_vec3 vDir)
{
	m_vImForceDirection = vDir;
}

_vec3 CDynamicObject::Get_Reflection(_vec3 vDir, _vec3 vPos)
{
	_vec3 vResult = m_pSupportCom->Reflection(vDir, m_pSupportCom->Get_Position());

	return vResult;
}

_bool CDynamicObject::Get_RayPick(_vec3 vDir, _vec3 vPos)
{
	return (m_pSupportCom->Collision_RayPick_Dynamic(vDir, vPos, m_pMeshCom, m_pTransformCom)&& (m_pSupportCom->Get_Distance() < 0.5f));
}





void CDynamicObject::Free(void)
{
	if (m_pEffect != nullptr)
	{
		Safe_Release(m_pEffect);
	}

	CBaseObject::Free();
}

HRESULT CDynamicObject::Setup_ConstantTable(LPD3DXEFFECT& pEffect, _bool bDissolve)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement || pEffect == nullptr)
	{
		return E_FAIL;
	}

	_matrix			matWorld, matView, matProj;

	matWorld = m_pTransformCom->Get_TransformDescription().matWorld;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	pEffect->SetFloat("g_DissolveAmount", m_fDeadTime);

	if (bDissolve)
		pEffect->SetTexture("g_DissolveTexture", m_pDesolveTextureCom->Get_Texture());

	D3DLIGHT9* pLight = new D3DLIGHT9;
	m_pDevice->GetLight(0,pLight);

	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLight->Diffuse);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLight->Ambient);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLight->Specular);
	pEffect->SetVector("g_vLightDir", &_vec4(pLight->Direction, 0.f));

	delete pLight;
	pLight = nullptr;

	return S_OK;
}


void CDynamicObject::Do_Anichange(_uint iAnimation)
{
	m_pMeshCom->Set_AnimationSet(iAnimation);
}

void CDynamicObject::Check_Hit(_bool bForce, _uint iDamage)
{
	if (m_pSupportCom->Picking_Object_Dynamic(g_hWnd, m_pMeshCom, m_pTransformCom))
	{
		eType = Engine::COLIDETYPE::COL_TRUE;

		//이펙트 펑
		if (m_bUseBaseEffect == true && iDamage != 0 && m_pEffect == nullptr)
		{
			Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();

			m_pEffect = dynamic_cast<CBaseEffect*>(pManagement->Get_Data_From_MemoryPool(L"HurtPool"));
			m_pEffect->Set_Position(m_pSupportCom->Get_Position());

			m_bHurt = true;
		}
		Hit_Attack(iDamage);
	}
	else if (eType != Engine::COLIDETYPE::COL_INRANGE)
	{
		eType = Engine::COLIDETYPE::COL_FALSE;
	}
}

_vec3 CDynamicObject::Calc_Position_At_Mesh()
{
	_vec3 vNowPos = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);

	_vec3 vGab = m_vCorePos - m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
	
	vGab.y = 0.f;

	_vec3 vCalivPos = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS) - vGab;

	return vCalivPos;
}

_vec3 CDynamicObject::Get_Position_At_Mesh()
{
	return m_vCorePos;
}

_int CDynamicObject::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();

	if (m_bHurt == true)
	{	
		if (m_pEffect->LateUpdate_GameObject(fDeltaTime) == OBJ_DEAD)
		{
			pManagement->Release_Data_To_MemoryPool(L"HurtPool", m_pEffect);
			m_pEffect = nullptr;

			m_bHurt = false;
		}
	}


	return NO_EVENT;
}

_int CDynamicObject::LateUpdate_GameObject(const _float& fDeltaTime)
{
	return NO_EVENT;
}
