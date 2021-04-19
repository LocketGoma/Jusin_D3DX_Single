#pragma once


#include "BaseItem.h"

class CItemHealthKit : public CBaseItem
{
private:
	explicit CItemHealthKit(_Device pDevice);
	explicit CItemHealthKit(const CItemHealthKit& other);
	virtual ~CItemHealthKit() = default;


public:
	// CBaseItem을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;


public:
	static CItemHealthKit* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	HRESULT Add_Component();
	virtual void Free();


	// CBaseItem을(를) 통해 상속됨
	virtual HRESULT Interaction(Engine::CGameObject* pTarget) override;

};

