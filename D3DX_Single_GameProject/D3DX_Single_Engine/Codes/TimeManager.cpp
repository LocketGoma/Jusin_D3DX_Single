#include "TimeManager.h"

USING(Engine)

IMPLEMENT_SINGLETON(CTimeManager)

CTimeManager::CTimeManager()
{
	
}

HRESULT CTimeManager::Ready_TimeManager()
{
	m_StartTime = std::chrono::system_clock::now();
	m_GoalTime = m_StartTime;

    return S_OK;
}

_float CTimeManager::Update_TimeManager()
{
	m_GoalTime = std::chrono::system_clock::now();
	std::chrono::duration<float> dulation = m_GoalTime - m_StartTime;

	m_StartTime = m_GoalTime;

    return dulation.count();
}
