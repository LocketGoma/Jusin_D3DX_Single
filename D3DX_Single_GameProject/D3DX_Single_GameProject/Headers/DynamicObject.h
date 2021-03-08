#pragma once

//몬스터 등. AI를 따르는 다이나믹 오브젝트들
#ifndef __DYNAMIC_OBJECT_H__

#include "GameObject.h"

class CDynamicObject abstract : public Engine::CGameObject
{
protected:
	explicit CDynamicObject(_Device pDevice);
	explicit CDynamicObject(const CDynamicObject& other);
	virtual ~CDynamicObject() = default;


//기본 액션들
public:
	virtual void Go_Stright() PURE;				//전진
	virtual void Go_Side() PURE;				//사이드 이동
	virtual void Do_Run() PURE;					//달림
	virtual void Do_Walk() PURE;				//걸음
	virtual void Do_Rotate() PURE;				//회전
	virtual void Do_Attack() PURE;				//공격
	
	//virtual void Do_Tracking() PURE;			//적 추적

public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;

	virtual void	Free(void);

};


#endif // !__DYNAMIC_OBJECT_H__