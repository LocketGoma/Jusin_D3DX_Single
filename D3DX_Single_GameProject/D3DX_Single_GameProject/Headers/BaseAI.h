#pragma once

#ifndef __BASE_AI_H__
#define __BASE_AI_H__

//ภ๛ต้ AI

#include "Base.h"
#include "GameObject.h"
#include "DynamicObject.h"

class CBaseAI : public CBase
{
private:
	explicit CBaseAI();
	explicit CBaseAI(const CBaseAI& other);
	virtual ~CBaseAI() = default;

public:


	void Set_ControlUnit(CDynamicObject* pUnit);
	void Set_Target(Engine::CGameObject* pTarget);

public:
	//static CBaseAI* Create();
	virtual void Free() PURE;

private:
			CDynamicObject*	pControlUnit;
	Engine::CGameObject*	pTargetUnit;


};

#endif