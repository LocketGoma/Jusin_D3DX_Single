#pragma once

#ifndef __KEY_MANAGER_H__
#define __KEY_MANAGER_H__

//const expression?
constexpr auto VK_MAX = 0xFF;

#include "Engine_Include.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CKeyManager final : public CBase
{
	DECLARE_SINGLETON(CKeyManager)

private:
	explicit CKeyManager();	
	~CKeyManager() = default;

public:
	bool Key_Pressing(int _key);
	bool Key_Down(int _key);
	bool Key_Up(int _key);

	void Key_Update();					//모든 업데이트 돈 뒤에 자동으로 돌게 만들것.

public:
	virtual void Free() override;

private:
	bool				m_bState[VK_MAX];

};

END_NAMESPACE

#endif // !__KEY_MANAGER_H__