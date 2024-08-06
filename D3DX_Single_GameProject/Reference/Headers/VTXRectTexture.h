#pragma once

#ifndef __VIBUFFER_RECTTEXTURE_H__
#define __VIBUFFER_RECTTEXTURE_H__

#include "VIBuffer.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CVTXRectTexture : public CVIBuffer
{
private:
	explicit CVTXRectTexture(_Device pDevice);
	explicit CVTXRectTexture(const CVTXRectTexture& other);
	virtual ~CVTXRectTexture() = default;

public:
	// CVIBuffer을(를) 통해 상속됨
	virtual HRESULT Ready_Buffer(void) override;
	virtual HRESULT Render_Buffer(void) override;
	virtual HRESULT Change_Color(D3DXCOLOR _Color) override;


	static CVTXRectTexture* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;

};

END_NAMESPACE

#endif // !__VIBUFFER_RECTTEXTURE_H__