#pragma once

#ifndef __BASE_AI_H__
#define __BASE_AI_H__

//ภ๛ต้ AI

#include "Base.h"
#include "GameObject.h"

class CBaseAI : public Engine::CBase
{
private:
	explicit CBaseAI();
	explicit CBaseAI(const CBaseAI& other);
	virtual ~CBaseAI();

public:




public:
	//static CBaseAI* Create();
	virtual void Free() PURE;

private:
	CGameObject* pControlUnit;
	CGameObject* pTargetUnit;


};

#endif