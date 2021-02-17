#pragma once

#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

#include "Base.h"
//Chrono 패키지 이용. 
BEGIN_NAMESPACE(Engine)

class CTimeManager : public CBase
{
	DECLARE_SINGLETON(CTimeManager)

private:
	CTimeManager();
	virtual ~CTimeManager() = default;

public:
	HRESULT Ready_TimeManager();
	_float Update_TimeManager();

	virtual void Free() override;

private:	
	std::chrono::system_clock::time_point m_StartTime;
	std::chrono::system_clock::time_point m_GoalTime;

};

END_NAMESPACE

#endif // !__TIME_MANAGER_H__