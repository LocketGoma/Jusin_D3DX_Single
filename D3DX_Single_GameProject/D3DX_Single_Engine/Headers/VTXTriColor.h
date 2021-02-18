#pragma once

#ifndef __VIBUFFER_TRICOLOR_H__
#define __VIBUFFER_TRICOLOR_H__

#include "VIBuffer.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CVTXTriColor : public CVIBuffer
{
private:
	explicit CVTXTriColor(_Device pDevice );
	explicit CVTXTriColor(const CVTXTriColor& other);
	virtual ~CVTXTriColor() = default;

public:
	// CVIBuffer을(를) 통해 상속됨
	virtual HRESULT Ready_Buffer(void) override;
	virtual void Render_Buffer(void) override;

	
	static CVTXTriColor* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;

};

END_NAMESPACE

#endif // !__VIBUFFER_TRICOLOR_H__