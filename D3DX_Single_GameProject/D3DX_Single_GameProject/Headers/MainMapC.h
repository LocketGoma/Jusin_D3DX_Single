#pragma once

#ifndef __MAIN_MAP_C_OBJECT_H__
#define __MAIN_MAP_C_OBJECT_H__



#include "MapObject.h"
class CMainMapC : public CMapObject
{
private:
	explicit CMainMapC(_Device pDevice);
	explicit CMainMapC(const CMainMapC& other);
	virtual ~CMainMapC() = default;


public:
	// CMapObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;



public:
	static CMainMapC* Create(_Device pDevice, const _tchar* pMapName);
	virtual CGameObject* Clone(void* pArg) override;

private:
	HRESULT Add_Component();
	HRESULT SetUp_Material();


	virtual void Free();
};


#endif // !__MAIN_MAP_C_OBJECT_H__