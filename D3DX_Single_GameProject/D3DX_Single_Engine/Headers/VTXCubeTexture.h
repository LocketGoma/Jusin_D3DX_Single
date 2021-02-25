#pragma once

#ifndef __VIBUFFER_CUBETEXTURE_H__
#define __VIBUFFER_CUBETEXTURE_H__

#include "VIBuffer.h"

BEGIN_NAMESPACE(Engine)

class CVTXCubeTexture : public CVIBuffer
{
private:
	explicit CVTXCubeTexture(_Device pDevice);
	explicit CVTXCubeTexture(const CVTXCubeTexture& other);
	virtual ~CVTXCubeTexture() = default;

public:
	// CVIBuffer을(를) 통해 상속됨
	virtual HRESULT Ready_Buffer(void) override;
	virtual HRESULT Render_Buffer(void) override;


	static CVTXCubeTexture* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;

};

END_NAMESPACE

#endif // !__VIBUFFER_CUBETEXTURE_H__