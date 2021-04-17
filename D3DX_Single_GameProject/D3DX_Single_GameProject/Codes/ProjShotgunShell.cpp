#include "framework.h"
#include "ProjShotgunShell.h"

#include "StaticMesh.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "ControlSupport.h"

#include "ProjShotgunPellet.h"

//걍 탄껍데기임
CProjShotgunShell::CProjShotgunShell(_Device pDevice)
	: CBaseProjectile(pDevice)
	, m_fAxisDifferenceRange_X(2.5f)
	, m_fAxisDifferenceRange_Y(0.8f)
	, m_iShellNumber(0)
	, m_iPelletCount(24)

{
	m_fWeight = 0;
	m_fLifeTime = 0.005f;
	m_fSpeed = 300.f;
	m_eForceType = eForceType::NONE;
	m_fHitboxSize = 0.5f;

	m_iDamage = 0;
	m_fSplashRadius = m_fHitboxSize;
	m_vStartPos = _vec3(0.f, 0.f, 0.f);
	m_vDirection = _vec3(0.f, 0.f, 1.f);
	m_fRotate = 0.f;
}

CProjShotgunShell::CProjShotgunShell(const CProjShotgunShell& other)
	: CBaseProjectile(other)
	, m_fAxisDifferenceRange_X(other.m_fAxisDifferenceRange_X)
	, m_fAxisDifferenceRange_Y(other.m_fAxisDifferenceRange_Y)
	, m_iShellNumber(other.m_iShellNumber)
	, m_iPelletCount(other.m_iPelletCount)
{
}

HRESULT CProjShotgunShell::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CProjShotgunShell::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Info(Engine::TRANSFORM_INFO::INFO_LOOK, &m_vDirection);

	m_vAmmoSize = _vec3(0.4f, 0.4f, 0.4f);

	return S_OK;
}

_int CProjShotgunShell::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	m_fLifeTime -= fDeltaTime;

	if (m_fLifeTime < 0.f)
	{
		Shoot_Pellet(m_iPelletCount);

		return NO_EVENT;
	}

	return NO_EVENT;
}

_int CProjShotgunShell::LateUpdate_GameObject(const _float& fDeltaTime)
{
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

HRESULT CProjShotgunShell::Render_GameObject(void)
{
	m_pTransformCom->LateUpdate_Component(0.f);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;





	_mat matWorld = m_pTransformCom->Get_TransformDescription().matWorld;

	m_pColliderCom->Render_Collider(eType, &matWorld, g_bViewCollider);
}

void CProjShotgunShell::Set_ShellNumber(_uint iShellNumber)
{
	m_iShellNumber = iShellNumber;
}

HRESULT CProjShotgunShell::Shoot_Pellet(_uint iPelletCount)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return false;
	}
	for (int i = 0; i < iPelletCount; i++)
	{
		Engine::CGameObject* pObject = pManagement->Clone_GameObject(L"Projectile_Pellet");
		NULL_CHECK_RETURN(pObject, E_FAIL);

		//1. 현재 방향, 현재 위치 구함
		//2. 총구 위치 구함
		//3. 총구 위치 -> (현재 방향 x 현재 위치*속도) 값으로 최종 방향 구함
		//4. 3번의 방향, 2번의 위치를 적용시킴.

		_vec3 vDir = m_vDirection;

		int iRandX = rand() % 100 - 50;
		int iRandY = rand() % 100 - 50;

		_float fRangeX = ((float)iRandX / 1000 * m_fAxisDifferenceRange_X);
		_float fRangeY = ((float)iRandY / 1000 * m_fAxisDifferenceRange_Y);


		vDir.x += fRangeX;
		vDir.y += fRangeY;

		dynamic_cast<CProjShotgunPellet*>(pObject)->Set_Position(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS));
		dynamic_cast<CProjShotgunPellet*>(pObject)->Set_Direction(vDir);
		dynamic_cast<CProjShotgunPellet*>(pObject)->Set_TargetState(eTargetState::ToEnemy);

		TCHAR tObjName[128] = L"";
		TCHAR tObjData[] = L"%d ShotgunPellet %d";
		swprintf_s(tObjName, tObjData, m_iShellNumber, i);


		if (FAILED(pManagement->Get_NowScene()->Get_Layer(L"WeaponLayer")->Add_GameObject(tObjName, pObject)))
		{
			Safe_Release(pObject);
		}

	}
	
	Set_Dead();

	return S_OK;
}

HRESULT CProjShotgunShell::Add_Component()
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

	return S_OK;
}

CProjShotgunShell* CProjShotgunShell::Create(_Device pDevice)
{
	CProjShotgunShell* pInstance = new CProjShotgunShell(pDevice);

	if (FAILED(pInstance->Ready_GameObject(0)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CProjShotgunShell::Clone(void* pArg)
{
	CProjShotgunShell* pClone = new CProjShotgunShell(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CProjShotgunShell");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CProjShotgunShell::Free()
{
	CBaseProjectile::Free();
}
