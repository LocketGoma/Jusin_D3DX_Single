#pragma once

#ifndef __BASE_AI_H__
#define __BASE_AI_H__

//���� AI

#include "Base.h"
#include "GameObject.h"

class CBaseAI : public CBase
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
	Engine::CGameObject* pControlUnit;
	Engine::CGameObject* pTargetUnit;


};

#endif