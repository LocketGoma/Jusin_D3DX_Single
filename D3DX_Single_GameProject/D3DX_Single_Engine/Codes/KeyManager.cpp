#include "..\Headers\KeyManager.h"

USING(Engine)

IMPLEMENT_SINGLETON(CKeyManager)

CKeyManager::CKeyManager()
{
    ZeroMemory(m_bState, sizeof(m_bState));
}

CKeyManager::CKeyManager(const CKeyManager& other)
{
    ZeroMemory(m_bState, sizeof(m_bState));
}

bool CKeyManager::Key_Pressing(int _key)
{
    return (GetAsyncKeyState(_key) & 0x8000) ? true : false;		//누르는지 인식	
}

bool CKeyManager::Key_Down(int _key)
{
    if (!m_bState[_key] && Key_Pressing(_key)) 
    {
        m_bState[_key] = !m_bState[_key];
        return true;
    }
    else
    {
        return false;
    }
}

bool CKeyManager::Key_Up(int _key)
{
    if (m_bState[_key] && !Key_Pressing(_key)) 
    {
        m_bState[_key] = !m_bState[_key];
        return true;
    }
    else
    {
        return false;
    }
}

void CKeyManager::Key_Update()
{
    for (int i = 0; i < VK_MAX; ++i)
    {
        if (m_bState[i] && !(Key_Pressing(i)))
        {
            m_bState[i] = !m_bState[i];
        }
        if (!m_bState[i] && (Key_Pressing(i)))
        {
            m_bState[i] = !m_bState[i];
        }
    }
}

void CKeyManager::Free()
{
    ZeroMemory(m_bState, sizeof(m_bState));
}
