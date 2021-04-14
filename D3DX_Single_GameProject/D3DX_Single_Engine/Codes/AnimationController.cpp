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
	other.m_pAniControl->CloneAnimationController(other.m_pAniControl->GetMaxNumAnimationOutputs(),	// ���� �� ���� ��ü���� �����Ǵ� �ִϸ��̼� ���� ���� 
		other.m_pAniControl->GetMaxNumAnimationSets(),		// ���� ������ �ִϸ��̼� ���� �ִ� ����(�밳�� 1���ڿ� ���� ����)
		other.m_pAniControl->GetMaxNumTracks(),				// ���� ������ Ʈ���� �ִ� ����
		other.m_pAniControl->GetMaxNumEvents(),				// �޽� �ȿ� ������ִ� ��Ư�� ȿ��, ��� �Ұ�.
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

	//���� �������� ���� ����ð��� (���� ��¦�̶�) �Ѿ��
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

	//���� �������� ���� ����ð��� (���� ��¦�̶�) �Ѿ��
	if (TrackInfo.Position >= m_dPeriod - 0.05)
	{
		m_pAniControl->KeyTrackPosition(m_iCurrentTrack, 0.f, m_dPeriod);

		m_pAniControl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

		return true;
	}

	return false;
}


//�� �ǹ̾�
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

	//Ʈ�� ����Ī
	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	LPD3DXANIMATIONSET		pAS = nullptr;

	m_pAniControl->GetAnimationSet(iIndex, &pAS);
	
	m_dPeriod = pAS->GetPeriod();	//���� �ִϸ��̼� ���� ��ü ��� �ð� ��ȯ

	//0�� Ʈ���� ����ϰ��� �ϴ� �ִϸ��̼� �� ����
	m_pAniControl->SetTrackAnimationSet(m_iNewTrack, pAS);

	//(�������� �����ϴ� ��츦 ���� ȣ��)
	m_pAniControl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniControl->UnkeyAllTrackEvents(m_iNewTrack);

	///�Ʒ��� �ִϸ��̼� �������� ��Ʈ

	//1. ����� �ִϸ��̼�

	// ������ Ʈ���� ��� ������ �����ϴ� �Լ�.
	//1�� : ���� Ʈ��
	//2�� : True - �ִϸ��̼� ���� / False - �ִϸ��̼� ����
	//3�� : �������� Ʈ���� ��� �Ǵ� ������ ������ ����
	m_pAniControl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + 0.25);

	// Ʈ���� �����Ǵ� �ð����� ���� �ִϸ��̼� ���� � �ӵ��� �������� ����(�ӵ��� ��� ���� ���� 1)
	m_pAniControl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// Ʈ���� �����Ǵ� �ð����� ���� �ִϸ��̼� ���� � ����ġ�� ���� �� ��(�ӵ��� ��� ���� ���� 1)
	m_pAniControl->KeyTrackWeight(m_iCurrentTrack, 0.7f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// Ʈ�� Ȱ��ȭ�� ������ �Ǵ��ϴ� �Լ�
	m_pAniControl->SetTrackEnable(m_iNewTrack, TRUE);
	// Ʈ���� �����ϴ� �ð����� ���� �ִϸ��̼� ���� � �ӵ��� �������� ����(�ӵ��� ��� ���� ���� 1)
	m_pAniControl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);
	// Ʈ���� �����ϴ� �ð����� ���� �ִϸ��̼� ���� � ����ġ�� ���� �� ��(�ӵ��� ��� ���� ���� 1)
	m_pAniControl->KeyTrackWeight(m_iNewTrack, 0.3f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);


	m_pAniControl->ResetTime(); // �ִϸ��̼��� ����Ǵ� �ð��� �ٽ� ����(advanced�Լ� ȣ�� �� ���������� �����Ǵ� �ð� ����)

	m_fAccTime = 0.f;

	m_pAniControl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldAniIdx = iIndex;

	m_iCurrentTrack = m_iNewTrack;

	//�ִϸ��̼� ���� ��
}

void CAnimationController::Force_Change_AnimationSet(const _uint& iIndex)
{
	//Ʈ�� ����Ī
	m_iNewTrack = (m_iCurrentTrack == 0 ? 1 : 0);

	LPD3DXANIMATIONSET		pAS = nullptr;

	m_pAniControl->GetAnimationSet(iIndex, &pAS);

	m_dPeriod = pAS->GetPeriod();	//���� �ִϸ��̼� ���� ��ü ��� �ð� ��ȯ

	//�� Ʈ���� ����ϰ��� �ϴ� �ִϸ��̼� �� ����
	m_pAniControl->SetTrackAnimationSet(m_iNewTrack, pAS);

	//Ű �̺�Ʈ ����
	m_pAniControl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniControl->UnkeyAllTrackEvents(m_iNewTrack);

	///�Ʒ��� �ִϸ��̼� �������� ��Ʈ
	//...�� ���� ���Ѿߵ�.

	//1. ����� �ִϸ��̼�

	// ������ Ʈ���� ��� ������ �����ϴ� �Լ�.
	//1�� : ���� Ʈ��
	//2�� : True - �ִϸ��̼� ���� / False - �ִϸ��̼� ����
	//3�� : �������� Ʈ���� ��� �Ǵ� ������ ������ ����
	m_pAniControl->KeyTrackEnable(m_iCurrentTrack, FALSE, 0.f);

	// Ʈ���� �����Ǵ� �ð����� ���� �ִϸ��̼� ���� � �ӵ��� �������� ����(�ӵ��� ��� ���� ���� 1)
	m_pAniControl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// Ʈ���� �����Ǵ� �ð����� ���� �ִϸ��̼� ���� � ����ġ�� ���� �� ��(�ӵ��� ��� ���� ���� 1)
	m_pAniControl->KeyTrackWeight(m_iCurrentTrack, 0.0f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	m_fAccTime = 0.0f;

	// Ʈ�� Ȱ��ȭ�� ������ �Ǵ��ϴ� �Լ�
	m_pAniControl->SetTrackEnable(m_iNewTrack, TRUE);
	// Ʈ���� �����ϴ� �ð����� ���� �ִϸ��̼� ���� � �ӵ��� �������� ����(�ӵ��� ��� ���� ���� 1)
	m_pAniControl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);
	// Ʈ���� �����ϴ� �ð����� ���� �ִϸ��̼� ���� � ����ġ�� ���� �� ��(�ӵ��� ��� ���� ���� 1)
	m_pAniControl->KeyTrackWeight(m_iNewTrack, 1.0f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);


	m_pAniControl->ResetTime(); // �ִϸ��̼��� ����Ǵ� �ð��� �ٽ� ����(advanced�Լ� ȣ�� �� ���������� �����Ǵ� �ð� ����)



	m_pAniControl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldAniIdx = iIndex;

	m_iCurrentTrack = m_iNewTrack;

}


void CAnimationController::Play_AnimationSet(const _float& fDeltaTime)
{
	// ���� �ִϸ��̼��� �����Ű�� �Լ�(fDeltaTime���� ������ AdvanceTime�Լ� ȣ�� �� ���������� �����ϴ� �ð� ���� ���� ����)
	m_pAniControl->AdvanceTime(fDeltaTime, NULL);// 2���� : �ִϸ��̼� ���ۿ� ���� ����Ʈ�� ���� ó���� ����ϴ� ��ü�� �ּ�(��Դ� ������� �ʰ� ���� ���� ���)

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
