#pragma once

#ifndef __TEST_MAP_OBJECT_H__
#define __TEST_MAP_OBJECT_H__

#include "MapObject.h"
class CTestMapObject final : public CMapObject
{
private:
	explicit CTestMapObject(_Device pDevice);
	explicit CTestMapObject(const CTestMapObject& other);
	virtual ~CTestMapObject() = default;

public:
	// CMapObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

	HRESULT Add_Component();

public:
	static CTestMapObject* Create(_Device pDevice, const _tchar* pMapName);
	virtual CGameObject* Clone(void* pArg) override;

private:
	virtual void Free();

};


#endif // !__TEST_MAP_OBJECT_H__