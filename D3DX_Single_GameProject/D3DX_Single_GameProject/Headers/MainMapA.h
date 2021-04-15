#pragma once

#ifndef __MAIN_MAP_A_OBJECT_H__
#define __MAIN_MAP_A_OBJECT_H__

#include "MapObject.h"

class CMainMapA final : public CMapObject
{
private:
	explicit CMainMapA(_Device pDevice);
	explicit CMainMapA(const CMainMapA& other);
	virtual ~CMainMapA() = default;

public:
	// CMapObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;


public:
	static CMainMapA* Create(_Device pDevice, const _tchar* pMapName);
	virtual CGameObject* Clone(void* pArg) override;

private:
	HRESULT Add_Component();
	HRESULT SetUp_Material();

	virtual void Free();
};


#endif // !__MAIN_MAP_A_OBJECT_H__