#include "AnimationController.h"

USING(Engine)

CAnimationController::CAnimationController(LPD3DXANIMATIONCONTROLLER pAniControl)
	: m_pAniControl(pAniControl)
	, m_iCurrentTrack(0)
	, m_iNewTrack(1)
	, m_fAccTime(0.f)
	, m_iOldAniIdx(INT_MAX)
	, m_dPeriod(0.0)
{
	m_pAniControl->AddRef();
}

CAnimationController::CAnimationController(const CAnimationController& other)
	: m_pAniControl(nullptr)
	, m_iCurrentTrack(other.m_iCurrentTrack)
	, m_iNewTrack(other.m_iNewTrack)
	, m_fAccTime(other.m_fAccTime)
	, m_iOldAniIdx(other.m_iOldAniIdx)
	, m_dPeriod(0.0)
{
	other.m_pAniControl->CloneAnimationController(other.m_pAniControl->GetMaxNumAnimationOutputs(),	// 복제 시 원본 객체에서 제공되는 애니메이션 셋의 개수 
		other.m_pAniControl->GetMaxNumAnimationSets(),		// 구동 가능한 애니메이션 셋의 최대 개수(대개는 1인자와 값이 같음)
		other.m_pAniControl->GetMaxNumTracks(),				// 구동 가능한 트랙의 최대 개수
		other.m_pAniControl->GetMaxNumEvents(),				// 메쉬 안에 적용되있는 독특한 효과, 사용 불가.
		&m_pAniControl);
}

LPD3DXANIMATIONCONTROLLER CAnimationController::Get_AnimationControl()
{
	return m_pAniControl;
}

_bool CAnimationController::End_AnimationSet()
{
	D3DXTRACK_DESC		TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniControl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

	//현재 프레임이 실제 재생시간을 (아주 살짝이라도) 넘어가면
	if (TrackInfo.Position >= m_dPeriod - 0.25)
	{		
		return true;
	}

	return false;
}

_bool CAnimationController::End_Animation_Sequence()
{
	D3DXTRACK_DESC		TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	m_pAniControl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

	//현재 프레임이 실제 재생시간을 (아주 살짝이라도) 넘어가면
	if (TrackInfo.Position >= m_dPeriod - 0.05)
	{
		m_pAniControl->KeyTrackPosition(m_iCurrentTrack, 0.f, m_dPeriod);

		m_pAniControl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

		return true;
	}

	return false;
}


//뭔 의미야
HRESULT CAnimationController::Ready_AnimationController()
{
	if (m_pAniControl == nullptr)
	{
		return E_FAIL;
	}
	return S_OK;
}

void CAnimationController::Set_AnimationSet(const _uint& iIndex)
{
	if (m_iOldAniIdx == iIndex)
		return;

	//트랙 스위칭
	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	LPD3DXANIMATIONSET		pAS = nullptr;

	m_pAniControl->GetAnimationSet(iIndex, &pAS);
	
	m_dPeriod = pAS->GetPeriod();	//현재 애니메이션 셋의 전체 재생 시간 반환

	//0번 트랙에 재생하고자 하는 애니메이션 셋 설정
	m_pAniControl->SetTrackAnimationSet(m_iNewTrack, pAS);

	//(선형보간 실패하는 경우를 위해 호출)
	m_pAniControl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniControl->UnkeyAllTrackEvents(m_iNewTrack);

	///아래는 애니메이션 선형보간 파트

	//1. 종료될 애니메이션

	// 지정한 트랙의 사용 유무를 결정하는 함수.
	//1번 : 지정 트랙
	//2번 : True - 애니메이션 시작 / False - 애니메이션 종료
	//3번 : 언제부터 트랙을 사용 또는 해제할 것인지 결정
	m_pAniControl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + 0.25);

	// 트랙이 해제되는 시간동안 현재 애니메이션 셋은 어떤 속도로 움직일지 결정(속도의 상수 값은 각자 1)
	m_pAniControl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// 트랙이 해제되는 시간동안 현재 애니메이션 셋은 어떤 가중치를 갖게 할 지(속도의 상수 값은 각자 1)
	m_pAniControl->KeyTrackWeight(m_iCurrentTrack, 0.7f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// 트랙 활성화의 유무를 판단하는 함수
	m_pAniControl->SetTrackEnable(m_iNewTrack, TRUE);
	// 트랙이 시작하는 시간동안 현재 애니메이션 셋은 어떤 속도로 움직일지 결정(속도의 상수 값은 각자 1)
	m_pAniControl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);
	// 트랙이 시작하는 시간동안 현재 애니메이션 셋은 어떤 가중치를 갖게 할 지(속도의 상수 값은 각자 1)
	m_pAniControl->KeyTrackWeight(m_iNewTrack, 0.3f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);


	m_pAniControl->ResetTime(); // 애니메이션이 재생되던 시간을 다시 세팅(advanced함수 호출 시 내부적으로 누적되던 시간 리셋)

	m_fAccTime = 0.f;

	m_pAniControl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldAniIdx = iIndex;

	m_iCurrentTrack = m_iNewTrack;

	//애니메이션 변경 끝
}

void CAnimationController::Force_Change_AnimationSet(const _uint& iIndex)
{
	//트랙 스위칭
	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	LPD3DXANIMATIONSET		pAS = nullptr;

	m_pAniControl->GetAnimationSet(iIndex, &pAS);

	m_dPeriod = pAS->GetPeriod();	//현재 애니메이션 셋의 전체 재생 시간 반환

	//새 트랙에 재생하고자 하는 애니메이션 셋 설정
	m_pAniControl->SetTrackAnimationSet(m_iNewTrack, pAS);

	//키 이벤트 날림
	m_pAniControl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniControl->UnkeyAllTrackEvents(m_iNewTrack);

	///아래는 애니메이션 선형보간 파트
	//...를 무시 시켜야됨.

	//1. 종료될 애니메이션

	// 지정한 트랙의 사용 유무를 결정하는 함수.
	//1번 : 지정 트랙
	//2번 : True - 애니메이션 시작 / False - 애니메이션 종료
	//3번 : 언제부터 트랙을 사용 또는 해제할 것인지 결정
	m_pAniControl->KeyTrackEnable(m_iCurrentTrack, FALSE, 0.f);

	// 트랙이 해제되는 시간동안 현재 애니메이션 셋은 어떤 속도로 움직일지 결정(속도의 상수 값은 각자 1)
	m_pAniControl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// 트랙이 해제되는 시간동안 현재 애니메이션 셋은 어떤 가중치를 갖게 할 지(속도의 상수 값은 각자 1)
	m_pAniControl->KeyTrackWeight(m_iCurrentTrack, 0.0f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	m_fAccTime = 0.0f;

	// 트랙 활성화의 유무를 판단하는 함수
	m_pAniControl->SetTrackEnable(m_iNewTrack, TRUE);
	// 트랙이 시작하는 시간동안 현재 애니메이션 셋은 어떤 속도로 움직일지 결정(속도의 상수 값은 각자 1)
	m_pAniControl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);
	// 트랙이 시작하는 시간동안 현재 애니메이션 셋은 어떤 가중치를 갖게 할 지(속도의 상수 값은 각자 1)
	m_pAniControl->KeyTrackWeight(m_iNewTrack, 1.0f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);


	m_pAniControl->ResetTime(); // 애니메이션이 재생되던 시간을 다시 세팅(advanced함수 호출 시 내부적으로 누적되던 시간 리셋)



	m_pAniControl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldAniIdx = iIndex;

	m_iCurrentTrack = m_iNewTrack;

}


void CAnimationController::Play_AnimationSet(const _float& fDeltaTime)
{
	// 실제 애니메이션을 재생시키는 함수(fDeltaTime과는 별개로 AdvanceTime함수 호출 시 내부적으로 증가하는 시간 값이 따로 있음)
	m_pAniControl->AdvanceTime(fDeltaTime, NULL);// 2인자 : 애니메이션 동작에 따른 이펙트에 대한 처리를 담당하는 객체의 주소(대게는 사용하지 않고 직접 만들어서 사용)

	m_fAccTime += fDeltaTime;
}

_uint CAnimationController::Get_NowAnimationNumber()
{
	return m_iOldAniIdx;
}

CAnimationController* CAnimationController::Create(LPD3DXANIMATIONCONTROLLER pAniControl)
{
	CAnimationController* pInstance = new CAnimationController(pAniControl);

	if (FAILED(pInstance->Ready_AnimationController()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimationController* CAnimationController::Create(const CAnimationController& other)
{
	CAnimationController* pInstance = new CAnimationController(other);

	if (FAILED(pInstance->Ready_AnimationController())) 
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimationController::Free(void)
{
	Safe_Release(m_pAniControl);
}
