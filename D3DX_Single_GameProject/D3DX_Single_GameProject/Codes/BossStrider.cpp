#include "framework.h"
#include "BossStrider.h"

#include "DynamicMesh.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "ControlSupport.h"

#include "ProjFlechette.h"
#include "ProjPulseAmmo.h"

CBossStrider::CBossStrider(_Device pDevice)
	: CDynamicObject(pDevice)	
{
	m_fRecognizeRange = 75.f;
	m_fMoveRange = 55.f;
	m_fAttackRange = 35.f;
	
	m_fHitboxSize = 10.0;

	m_fMoveSpeed = 2.f;
	m_fRotateSpeed = 1.5f;	//multiple

	m_iMaxHP = 200;

	m_iDamage = 2;

	m_eAction = eStriderAction::idle_high;
	m_ePrevAction = m_eAction;

	m_ePatton = eStriderPatton::Idle;

	m_bStand = true;


	m_fPattonInterval = 5.f;
	m_fPattonCooltime = 0.f;

	m_bAttackLock = false;
	m_bPattonLock = false;

	m_fPattonAInterval = 10.f;
	m_fPattonACooltime = 0.f;

	m_fTripleShootInterval = 0.15f;
	m_fTripleShootCooldownInterval = 0.45f;
	m_fTripleShootTime = 0.f;

	m_bShootLock = false;
	m_bTripleShootLock = false;
}

CBossStrider::CBossStrider(const CBossStrider& other)
	: CDynamicObject(other)
	, m_bStand(other.m_bStand)
	, m_eAction(other.m_eAction)
	, m_ePrevAction(other.m_ePrevAction)
	, m_ePatton(other.m_ePatton)
	, m_vOriPos(other.m_vOriPos)
	, m_vCalibrationPos(other.m_vCalibrationPos)
	, m_fPattonInterval(other.m_fPattonInterval)
	, m_fPattonCooltime(other.m_fPattonCooltime)
	, m_bAttackLock(other.m_bAttackLock)
	, m_bPattonLock(other.m_bPattonLock)
	, m_fPattonAInterval(other.m_fPattonAInterval)
	, m_fPattonACooltime(other.m_fPattonACooltime)
	, m_fTripleShootInterval(other.m_fTripleShootInterval)
	, m_fTripleShootCooldownInterval(other.m_fTripleShootCooldownInterval)
	, m_fTripleShootTime(other.m_fTripleShootTime)
	, m_bShootLock(other.m_bShootLock)
	, m_bTripleShootLock(other.m_bTripleShootLock)
{
	//m_vCalibrationPos = _vec3(0.f, 28.f, 20.f);
	m_vCalibrationPos = _vec3(0.f,28.f,0.f);
}
_uint CBossStrider::m_iPattonBShoot = 0;
_uint CBossStrider::m_iPattonCShoot = 0;
_uint CBossStrider::m_iPattonDShoot = 0;

HRESULT CBossStrider::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CBossStrider::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(BOSS_ENEMY_REDUCION_VECTOR);

	m_pMeshCom->Set_AnimationSet((_uint)m_eAction);

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

	//아래는 위치 보정 함수... 가 의미 없음 피봇 위치 자체를 바꿔야됨!
	m_vOriPos = m_pTransformCom->Get_Info_RawData(Engine::TRANSFORM_INFO::INFO_POS);
	m_pTransformCom->Set_Pos(m_vOriPos + m_vCalibrationPos);
	m_pTransformCom->Update_Component();


	pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);
	
	m_fTime = fDeltaTime;

	return NO_EVENT;
}

HRESULT CBossStrider::Render_GameObject(void)
{
	m_pMeshCom->Set_AnimationSet((_uint)m_eAction);

	m_pTransformCom->LateUpdate_Component(0.f);

	m_pMeshCom->Play_AnimationSet(m_fTime * m_fAnimationSpeed);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	m_pMeshCom->Render_Meshes();

	////아래 눈깔 기준 정점
	m_vCorePos = _vec3(0.f, 0.f, 0.f);
	m_vShootPos = _vec3(0.f, 0.f, 0.f);

	_mat matWorld = m_pMeshCom->Get_FrameByName("Combine_Strider_Neck_Bone")->CombinedTranformationMatrix;
	matWorld = matWorld * m_pTransformCom->Get_TransformDescription().matWorld;
	D3DXVec3TransformCoord(&m_vCorePos, &m_vCorePos, &matWorld);
	////정점계산 끝

	matWorld = m_pMeshCom->Get_FrameByName("Combine_Strider_Gun_Bone2")->CombinedTranformationMatrix;
	matWorld = matWorld * m_pTransformCom->Get_TransformDescription().matWorld;
	D3DXVec3TransformCoord(&m_vShootPos, &m_vShootPos, &matWorld);
	//총구 위치
	

	m_pColliderCom->Render_Collider(eType, &m_vCorePos);

	m_ePrevAction = m_eAction;

	//위치 보정 복원
	m_pTransformCom->Set_Pos(m_vOriPos);		//애니메이션 업데이트에서도 썼던거땜에 여기서 갱신

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

//일단 킵
void CBossStrider::Go_Stright(_float fDeltaTime)
{
}

//일단 킵 (2)
void CBossStrider::Go_Side(_float fDeltaTime, eAlign pAlign)
{
}

void CBossStrider::Do_Run(_float fDeltaTime)
{
	m_fMoveSpeed = 3.f;
	m_fAnimationSpeed = 1.5f;
}

void CBossStrider::Do_Walk(_float fDeltaTime)
{
	m_fMoveSpeed = 2.f;
	m_fAnimationSpeed = 1.f;
}

//일단 킵 (3)
void CBossStrider::Do_Rotate(_float fDeltaTime, eAlign pAlign)
{
	m_fRotate = (((int)pAlign - 0.5f) * 2.f) * fDeltaTime;
}

void CBossStrider::Do_Attack(_float fDeltaTime, _uint iPatton)
{
	if (m_bAttackLock == false && m_bPattonLock == false)
	{
		m_bAttackLock = true;
	}

	if ((eStriderPatton)iPatton == eStriderPatton::PattonA)
	{
		m_ePatton = eStriderPatton::PattonA;
		PattonA();
	}
	else if (m_bPattonLock)
	{
		m_bStand == true ? m_eAction = eStriderAction::idle_high : m_eAction = eStriderAction::idle_low;

		m_ePatton = eStriderPatton::Idle;
	}
	else if (m_fPattonCooltime <= 0.f)
	{
		//if ((eStriderPatton)iPatton == eStriderPatton::PattonB)
		//{
		//	m_ePatton = eStriderPatton::PattonB;
		//	PattonB();
		//}
		//else if ((eStriderPatton)iPatton == eStriderPatton::PattonC)
		//{
			m_ePatton = eStriderPatton::PattonC;
			PattonC();
		//}
		//else if ((eStriderPatton)iPatton == eStriderPatton::PattonD)
		//{
		//	m_ePatton = eStriderPatton::PattonD;
		//	PattonD();
		//}
	}

	if (m_bPattonLock)
	{
		m_fPattonCooltime += fDeltaTime;
	}

	if (m_fPattonCooltime >= m_fPattonInterval)
	{
		m_fPattonCooltime = 0.f;
		m_fNowAttackTime = 0.f;
		m_bPattonLock = false;
	}
}


void CBossStrider::Do_Idle(_float fDeltaTime)
{
	m_bStand == true ? m_eAction = eStriderAction::idle_high : m_eAction = eStriderAction::idle_low;
	m_ePatton = eStriderPatton::Idle;
}

void CBossStrider::Do_Spawn(_float fDeltaTime)
{
	m_eAction = eStriderAction::Surprise;


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
	return (_uint)m_ePatton;
}

void CBossStrider::Set_TargetPosition(_vec3 vPos)
{
	m_vTargetPos = vPos;
}


void CBossStrider::Set_Animation(_uint iIndex)
{
	m_pMeshCom->Set_AnimationSet(iIndex);
}

_bool CBossStrider::End_Animation_State_Force()
{
	return (m_pMeshCom->Get_NowAnimationNumber() == (_uint)m_eAction && m_pMeshCom->End_AnimationSet());
}

void CBossStrider::PattonA()
{
	if (m_bStand == true)
	{

		if (m_fPattonACooltime <= 0.f)
		{
			m_eAction = eStriderAction::Stomp;
			m_ePatton = eStriderPatton::PattonA;
		}

		if (End_Animation_State_Force())
		{
			m_eAction = eStriderAction::idle_high;
		}

		m_fPattonACooltime += m_fTime;

		if (m_fPattonACooltime >= m_fPattonAInterval)
		{
			m_fPattonACooltime = 0.f;
			m_bAttackHitEnable = true;
		}
	}
	else
	{
		if (m_bStand == false)
		{
			m_eAction = eStriderAction::Stand;
		}
		if (m_eAction == eStriderAction::Stand && End_Animation_State_Force())
		{
			m_bStand = true;

			m_eAction = eStriderAction::idle_high;
		}
	}

}

void CBossStrider::PattonB()
{
	if (m_fNowAttackTime <= 0.f || (_uint)(m_fNowAttackTime * 10) == 4 || (_uint)(m_fNowAttackTime * 10) == 8)
	{
		if (m_bStand == false)
		{
			m_eAction = eStriderAction::Stand;
		}
		if (m_eAction == eStriderAction::Stand && End_Animation_State_Force())
		{
			m_bStand = true;

			m_eAction = eStriderAction::idle_high;
		}
		if (m_bStand == true)
		{
			if (m_bShootLock == false)
			{
				m_bShootLock = true;


				auto pManagement = Engine::CManagement::Get_Instance();
				if (pManagement == nullptr)
				{
					return;
				}

				//기관총 출력파트
				Engine::CGameObject* pObject = pManagement->Clone_GameObject(L"Projectile_Flechette");
				NULL_CHECK(pObject);

				_vec3 vPos, vDir;
				_mat matWorld;

				vPos = m_vShootPos;
				vDir = m_vTargetPos - vPos;

				D3DXVec3Normalize(&vDir, &vDir);

				dynamic_cast<CProjFlechette*>(pObject)->Set_Position(vPos);
				dynamic_cast<CProjFlechette*>(pObject)->Set_Direction(vDir);
				dynamic_cast<CProjFlechette*>(pObject)->Set_TargetState(eTargetState::ToPlayer);

				TCHAR tObjName[128] = L"";
				TCHAR tObjData[] = L"StriderFlechette %d";
				swprintf_s(tObjName, tObjData, m_iPattonBShoot++);

				if (!FAILED(pManagement->Get_NowScene()->Get_Layer(L"WeaponLayer")->Add_GameObject(tObjName, pObject)))
				{
					m_bAttackHitEnable = true;
				}
			}
		}

	}
	else
	{
		m_bShootLock = false;
	}

	if (m_bStand == true)
	{
		m_ePatton = eStriderPatton::PattonB;
		m_eAction = eStriderAction::idle_high;

		m_fNowAttackTime += m_fTime;
	}

	if (m_fNowAttackTime >= m_fAttackInterval)
	{
		m_bAttackLock = false;
		m_bPattonLock = true;
	}

}

void CBossStrider::PattonC()
{
	if (m_fNowAttackTime <= 0.f)
	{
		m_bTripleShootLock = true;
		m_fTripleShootTime = 0.f;

		if (m_bStand == true)
		{
			m_eAction = eStriderAction::Crouch;
		}
		if (m_eAction == eStriderAction::Crouch && End_Animation_State_Force())
		{
			m_bStand = false;

			m_eAction = eStriderAction::idle_low;
		}
		if (m_bStand == false)
		{
			m_eAction = eStriderAction::idle_low;
		}
	}
	else 
	{
		m_bShootLock = false;		
	}

	if (m_bStand == false)
	{
		m_ePatton = eStriderPatton::PattonC;
		m_eAction = eStriderAction::idle_low;

		//3발 연속 사격이 가능하면
		if (m_bTripleShootLock == true)
		{
			//3발 연속 쿨타임이 초기화상태면
			if (m_fTripleShootTime <= 0.f)
			{
				//shoot
				auto pManagement = Engine::CManagement::Get_Instance();
				if (pManagement == nullptr)
				{
					return;
				}

				//기관총 출력파트
				Engine::CGameObject* pObject = pManagement->Clone_GameObject(L"Projectile_PulseAmmo");
				NULL_CHECK(pObject);

				_vec3 vPos, vDir;
				_mat matWorld;

				vPos = m_vShootPos;
				vDir = m_vTargetPos - vPos;

				D3DXVec3Normalize(&vDir, &vDir);

				dynamic_cast<CProjPulseAmmo*>(pObject)->Set_Position(vPos);
				dynamic_cast<CProjPulseAmmo*>(pObject)->Set_Direction(vDir);
				dynamic_cast<CProjPulseAmmo*>(pObject)->Set_TargetState(eTargetState::ToPlayer);

				TCHAR tObjName[128] = L"";
				TCHAR tObjData[] = L"HunterPulsetAmmo %d";
				swprintf_s(tObjName, tObjData, m_iPattonBShoot++);

				if (!FAILED(pManagement->Get_NowScene()->Get_Layer(L"WeaponLayer")->Add_GameObject(tObjName, pObject)))
				{
					m_bAttackHitEnable = true;
					m_iTripleShootCount++;
				}
				//Shoot 끝
			}

			m_fTripleShootTime += m_fTime;

			if (m_fTripleShootTime >= m_fTripleShootInterval)
			{
				m_fTripleShootTime = 0.f;
			}
			if (m_iTripleShootCount >= 3)
			{
				m_iTripleShootGroupCount++;
				m_fTripleShootTime = 0.f;
				m_bTripleShootLock = false;
			}
		}
	}

	//3발 연속 다 사격했으면 (=쿨타임)
	//이때 패턴 C 글로벌 쿨타임이 돔.
	if (m_bTripleShootLock == false)
	{
		m_fTripleShootTime += m_fTime;

		m_fNowAttackTime += m_fTime;
		m_iTripleShootCount = 0;
	}
	if (m_fTripleShootTime >= m_fTripleShootCooldownInterval)
	{
		m_bTripleShootLock = true;
		m_fTripleShootTime = 0.f;
	}

	//패턴 C 글로벌 쿨타임이 끝나면
	//패턴 락 쿨타임 시작 = 패턴 빠져나감
	if (m_fNowAttackTime >= m_fAttackInterval || m_iTripleShootGroupCount >= 3)
	{
		m_iTripleShootGroupCount = 0;
		m_iTripleShootCount = 0;
		m_fTripleShootTime = 0.f;
		
		m_bTripleShootLock = false;
		m_bAttackLock = false;
		m_bPattonLock = true;
	}

}

void CBossStrider::PattonD()
{
	m_bAttackLock = false;
	m_bPattonLock = true;
}

void CBossStrider::PattonE()
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
