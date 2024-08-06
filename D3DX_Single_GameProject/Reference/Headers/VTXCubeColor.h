#pragma once

#ifndef __VIBUFFER_CUBECOLOR_H__
#define __VIBUFFER_CUBECOLOR_H__

#include "VIBuffer.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CVTXCubeColor : public CVIBuffer
{
private:
	explicit CVTXCubeColor(_Device pDevice);
	explicit CVTXCubeColor(const CVTXCubeColor& other);
	virtual ~CVTXCubeColor() = default;

public:
	// CVIBuffer을(를) 통해 상속됨
	virtual HRESULT Ready_Buffer(void) override;
	virtual HRESULT Render_Buffer(void) override;
	virtual HRESULT Change_Color(D3DXCOLOR _Color) override;


	static CVTXCubeColor* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;

};

END_NAMESPACE

#endif // !__VIBUFFER_CUBETEXTURE_H__
