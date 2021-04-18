#pragma once

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Engine_Include.h"
#include "GraphicResources.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CTexture : public CGraphicResources
{
protected:
	explicit CTexture(_Device pDevice);
	explicit CTexture(const CTexture& other);
	virtual ~CTexture() = default;

public:
	HRESULT Ready_Texture(const _tchar* pPath, TEXTYPE eType, const _uint& iCount = 1);
	HRESULT Set_Texture(const _uint& iIndex = 0, const _uint& iChannel = 0);
	IDirect3DBaseTexture9* Get_Texture(const _uint& iIndex = 0);

	static CTexture* Create(_Device pDevice,
		const _tchar* pPath,
		TEXTYPE eType,
		const _uint& iCnt = 1);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	

private:
	virtual void Free() override;

	std::vector<IDirect3DBaseTexture9*> m_vecTexture;


	// CGraphicResources을(를) 통해 상속됨

};

END_NAMESPACE

#endif // !__TEXTURE_H__