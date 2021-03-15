#pragma once

#ifndef __ANIMATOIN_CONTROLLER_H__
#define __ANIMATOIN_CONTROLLER_H__

#include "Engine_Include.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)

//이름 중복임. AniController로 바꿀것

class ENGINE_DLL CAnimationController : public CBase
{
private:
	explicit CAnimationController(LPD3DXANIMATIONCONTROLLER pAniControl);
	explicit CAnimationController(const CAnimationController& other);
	virtual ~CAnimationController() = default;

public:
	LPD3DXANIMATIONCONTROLLER	Get_AnimationControl();
	_bool						End_AnimationSet();

public:
	HRESULT Ready_AnimationController();
	void Set_AnimationSet(const _uint& iIndex);
	void Play_AnimationSet(const _float& fDeltaTime);

private:
	LPD3DXANIMATIONCONTROLLER			m_pAniControl;

	_uint								m_iCurrentTrack;
	_uint								m_iNewTrack;

	_float								m_fAccTime;
	_uint								m_iOldAniIdx;
	_double								m_dPeriod;


public:
	static CAnimationController* Create(LPD3DXANIMATIONCONTROLLER pAniControl);
	static CAnimationController* Create(const CAnimationController& other);
	virtual void			Free(void);
};

END_NAMESPACE

#endif // !__ANIMATOIN_CONTROLLER_H__