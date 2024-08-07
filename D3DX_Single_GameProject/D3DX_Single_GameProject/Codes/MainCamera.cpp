#include "framework.h"
#include "MainCamera.h"

#include "Transform.h"
#include "CameraComponent.h"

#include "Player.h"

CMainCamera::CMainCamera(_Device pDevice)
	: Engine::CGameObject(pDevice)
	, m_fRotate(0.f)
	, m_fAxisXSpeed(100.f)
	, m_fAxisYSpeed(100.f)
	, m_bMouseLock(true)
	, m_fLoopAngle(0.f)
	, m_bShaking(false)
	, m_bRecoilShake(false)
	, m_vShake(ZERO_VECTOR)
	, m_vRecoilShake(ZERO_VECTOR)
	, m_vRecoilShakeAdd(ZERO_VECTOR)
	, m_vRecoilShakeLoop(ZERO_VECTOR)
	, m_iShakePatton(0)
	, m_fRecoilPower(0.f)
	, m_fLoopRecoilAngle(90.f)
	, m_fLoopRecoilTimer(0.f)
	, m_fCameraRange(5.0f)
	, m_fQuakeMovement(0.f)
	, m_fQuakeTimer(0.f)
	, m_bQuake(false)
{
}

CMainCamera::CMainCamera(const CMainCamera& other)
	: Engine::CGameObject(other)
	, m_fRotate(other.m_fRotate)
	, m_fAxisXSpeed(other.m_fAxisXSpeed)
	, m_fAxisYSpeed(other.m_fAxisYSpeed)
	, m_bMouseLock(true)
	, m_fLoopAngle(0.f)
	, m_bShaking(false)
	, m_bRecoilShake(false)
	, m_vShake(ZERO_VECTOR)
	, m_vRecoilShake(ZERO_VECTOR)
	, m_vRecoilShakeAdd(ZERO_VECTOR)
	, m_vRecoilShakeLoop(ZERO_VECTOR)
	, m_iShakePatton(0)
	, m_fRecoilPower(other.m_fRecoilPower)
	, m_fLoopRecoilAngle(90.f)
	, m_fLoopRecoilTimer(0.f)
	, m_fCameraRange(other.m_fCameraRange)
	, m_fQuakeMovement(0.f)
	, m_fQuakeTimer(0.f)
	, m_bQuake(false)
{
}

HRESULT CMainCamera::Ready_GameObject(void)
{
	return S_OK;
}

HRESULT CMainCamera::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	POINT ptMouse;
	ptMouse.x = WINCX / 2;
	ptMouse.y = WINCY / 2;
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);


	m_fAxisXSpeed = 150.f;
	m_fAxisYSpeed = 100.f;

	return S_OK;
}

_int CMainCamera::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	Key_Input(fDeltaTime);

	if (m_bRecoilShaking == true)
	{		
		m_pTransformCom->Single_Rotation(Engine::ROTATION::ROT_X,m_fStartRecoil);
		m_pTransformCom->Rotation(Engine::ROTATION::ROT_X, -m_vRecoilShake.y);
	}

	if (m_bMouseLock == true)
	{
		Mouse_Movement();
	}
	
	return NO_EVENT;
}

_int CMainCamera::LateUpdate_GameObject(const _float& fDeltaTime)
{
	Base_Movement(fDeltaTime);


	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return 0;
	}

	if (m_pPlayer->HurtState() || m_bShaking == true)
	{
		Shake_Camera(fDeltaTime);
	}
	if (m_bQuake == true)
	{
		EarthQuake_Movement(fDeltaTime);
	}

	if (m_bRecoilShake == false)
	{
		m_fStartRecoil = m_pTransformCom->Get_Rotate(Engine::ROTATION::ROT_X);
	}
	if (m_pPlayer->Get_Recoil())
	{
		m_bRecoilShake = true;
		Set_Recoil_Weapon_Power(m_pPlayer->Get_RecoilPower());
		m_bRecoilShaking = false;
		Recoil_Weapon_Camera(fDeltaTime);
	}
	else if (m_bRecoilShaking == true)
	{
		Recoil_Weapon_Camera(fDeltaTime);
	}


	m_pTransformCom->Update_Component(fDeltaTime);

	//수정해줘야됨
	m_pCameraCom->Set_ViewVector(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS)+ m_vShake, m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS) + m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK) * (m_fCameraRange + m_fQuakeMovement), m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_UP));
	
	return m_pCameraCom->LateUpdate_Component(fDeltaTime);
}

HRESULT CMainCamera::Render_GameObject(void)
{
	m_pTransformCom->LateUpdate_Component(0.f);

	return S_OK;
}

void CMainCamera::Set_Position(_vec3 vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CMainCamera::Set_Size(_vec3 vSize)
{
}

_vec3 CMainCamera::Get_Position()
{
	return m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
}

_vec3 CMainCamera::Get_Size()
{
	return _vec3();
}

HRESULT CMainCamera::Set_Player(CPlayer* pPlayer)
{
	if (pPlayer == nullptr)
	{
		return E_FAIL;
	}

	m_pPlayer = pPlayer;

	return S_OK;
}

HRESULT CMainCamera::Add_Component(void)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return 0;
	}

	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pCameraCom = dynamic_cast<Engine::CCameraComponent*>(pManagement->Clone_Prototype(L"Camera_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Camera", pComponent);

	//카메라 기본 세팅
	Engine::CCameraComponent::CAMERA_DESC m_pCameradesc;

	ZeroMemory(&m_pCameradesc, sizeof(Engine::CCameraComponent::CAMERA_DESC));
	m_pCameradesc.vEye = _vec3(0.f, 0.f, -5.f);
	m_pCameradesc.vAt = _vec3(0.f, 0.f, 1.f);
	m_pCameradesc.vUp = _vec3(0.f, 1.f, 0.f);

	m_pCameradesc.fAspect = _float(WINCX) / WINCY;
	m_pCameradesc.fFovY = D3DXToRadian(60.f);
	m_pCameradesc.fZNear = 0.1f;
	m_pCameradesc.fZFar = 500.f;

	D3DXMatrixLookAtLH(&m_pCameradesc.matView, &m_pCameradesc.vEye, &m_pCameradesc.vAt, &m_pCameradesc.vUp);
	D3DXMatrixPerspectiveFovLH(&m_pCameradesc.matProj, m_pCameradesc.fFovY, m_pCameradesc.fAspect, m_pCameradesc.fZNear, m_pCameradesc.fZFar);

	m_pCameraCom->Set_CameraDesc(m_pCameradesc);

	return S_OK;
}

HRESULT CMainCamera::Base_Movement(const _float& fDeltatime)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	Engine::CTransform* pPlayerTransform = dynamic_cast<Engine::CTransform *>(m_pPlayer->Get_Component(L"Com_Transform",Engine::COMPONENT_ID::ID_DYNAMIC));
	if (pPlayerTransform == nullptr)
	{
		return S_OK;
	}

	_mat matPlayerWorld = pPlayerTransform->Get_TransformDescription().matWorld;
	_vec3 vPlayerAt = pPlayerTransform->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
	//_vec3 vPlayerLook = pPlayerTransform->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK);
	//_vec3 vPlayerRight = pPlayerTransform->Get_Info(Engine::TRANSFORM_INFO::INFO_RIGHT);

	m_pTransformCom->Set_Info(Engine::TRANSFORM_INFO::INFO_POS, &vPlayerAt);
	//m_pTransformCom->Set_Info(Engine::TRANSFORM_INFO::INFO_LOOK, &vPlayerLook);
	//m_pTransformCom->Set_Info(Engine::TRANSFORM_INFO::INFO_RIGHT, &vPlayerRight);

	return S_OK;
}

void CMainCamera::Key_Input(const _float& fDeltaTime)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return;
	}

	if (pManagement->Key_Down(VK_LCONTROL))        // = 'CTRL'
	{
		m_bMouseLock = m_bMouseLock == false;
	}


}

void CMainCamera::Mouse_Movement()
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	//뷰포트 -> 투영
	_float3 vMouse = _float3(0.f, 0.f, 0.f);
	vMouse.x = ptMouse.x / (WINCX * 0.5f) - 1.f;
	vMouse.y = 1.f - ptMouse.y / (WINCY * 0.5f);

	//투영 -> 뷰
	_float4x4 matProj = m_pCameraCom->Get_CameraDesc().matProj;
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMouse, &vMouse, &matProj);

	m_pTransformCom->Rotation(Engine::ROTATION::ROT_Y, vMouse.x * (m_fAxisXSpeed / 100.f));
	m_pTransformCom->Rotation(Engine::ROTATION::ROT_X, -vMouse.y * (m_fAxisYSpeed / 100.f));

	ptMouse.x = WINCX >> 1;
	ptMouse.y = WINCY >> 1;
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);

}

void CMainCamera::Picking_Mouse()
{
}

HRESULT CMainCamera::Shake_Camera(const _float& fDeltaTime)
{
	if (m_fLoopAngle >= 360.f)
	{
		m_bShaking = false;
		m_fLoopAngle = 0.f;
		m_vShake = ZERO_VECTOR;
		return S_OK;
	}
	if (m_bShaking == false) 
	{		
		m_bShaking = true;
		m_iShakePatton = rand() % 3;
	}
	switch (m_iShakePatton)
	{
		case 0 :
		{
			m_vShake.x = -sin(D3DXToRadian(m_fLoopAngle)) * 0.75f;
			m_vShake.y = sin(D3DXToRadian(m_fLoopAngle)) * 0.75f;
			break;
		}
		case 1:
		{
			m_vShake.y = sin(D3DXToRadian(m_fLoopAngle)) * 0.75f;
			break;
		}
		case 2:
		{
			m_vShake.x = sin(D3DXToRadian(m_fLoopAngle)) * 0.75f;
			m_vShake.y = sin(D3DXToRadian(m_fLoopAngle)) * 0.75f;
			break;
		}
		default:
		{	
			m_vShake.y = sin(D3DXToRadian(m_fLoopAngle)) * 0.75f;
		break;
		}
	}

	m_fLoopAngle += fDeltaTime * 720.f;
	return S_OK;
}

HRESULT CMainCamera::EarthQuake_Movement(const _float& fDeltaTime)
{
	if (m_fQuakeTimer >= 720.f)
	{
		m_bQuake = false;
		m_fQuakeTimer = 0.f;

		m_fQuakeMovement = 0.f;

		return S_OK;
	}
	if (m_bQuake == false)
	{
		m_bQuake = true;
	}

	m_fQuakeMovement = sin(D3DXToRadian(m_fQuakeTimer * 1.75f));

	m_fQuakeTimer += fDeltaTime * 720.f;
	return S_OK;
}

HRESULT CMainCamera::Recoil_Weapon_Camera(const _float& fDeltaTime)
{
	if (m_fLoopRecoilTimer >= 180.f)
	{
		m_fLoopRecoilTimer = 0.f;
		m_bRecoilShaking = false;
		m_bRecoilShake = false;
		m_vRecoilShake = ZERO_VECTOR;
		m_vRecoilShakeAdd = ZERO_VECTOR;
		m_vRecoilShakeLoop = ZERO_VECTOR;
		return S_OK;
	}

	m_vRecoilShakeLoop.y = sin(D3DXToRadian(m_fLoopRecoilTimer)) * fabs(m_fRecoilPower);

	if (m_bRecoilShaking == false)
	{
		m_fLoopRecoilTimer = 0.f;
		m_vRecoilShakeAdd += m_vRecoilShakeLoop;
		m_bRecoilShaking = true;
	}

	if (m_vRecoilShake.y > 1)
	{
		m_vRecoilShake.y = 1;
	}
	else
	{
		m_vRecoilShake = m_vRecoilShakeAdd + m_vRecoilShakeLoop;
	}

	m_fLoopRecoilTimer += fDeltaTime * 720.f;
	return S_OK;
}

void CMainCamera::Set_Recoil_Weapon_Power(_float fPower)
{
	m_fRecoilPower = fPower;
}

void CMainCamera::Set_MouseSpeedX(_float fAxisX)
{
	m_fAxisXSpeed = fAxisX;
}

void CMainCamera::Set_MouseSpeedY(_float fAxisY)
{
	m_fAxisYSpeed = fAxisY;
}

_float CMainCamera::Get_MouseSpeedX()
{
	return m_fAxisXSpeed;
}

_float CMainCamera::Get_MouseSpeedY()
{
	return m_fAxisYSpeed;
}

CMainCamera* CMainCamera::Create(_Device pDevice)
{
	
	CMainCamera* pInstance = new CMainCamera(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CMainCamera::Clone(void* pArg)
{
	CMainCamera* pClone = new CMainCamera(*this);

	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CMainCamera");
	}
	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}


	return pClone;
}

void CMainCamera::Free()
{
	//m_pPlayer에다 safe_release를 해야하는가?

	//Safe_Release(m_pPlayer);

	Engine::CGameObject::Free();
}
