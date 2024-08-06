#pragma once

#ifndef __VIBUFFER_SPHERECOLOR_H__
#define __VIBUFFER_SPHERECOLOR_H__

#include "VIBuffer.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CVTXSphereColor : public CVIBuffer
{
private:
	explicit CVTXSphereColor(_Device pDevice, _float fRadius, _uint iSlice, _uint iStack, _vec4 vColor);
	explicit CVTXSphereColor(const CVTXSphereColor& other);
	virtual ~CVTXSphereColor() = default;

public:
	// CVIBuffer을(를) 통해 상속됨
	virtual HRESULT Ready_Buffer(void) override;
	virtual HRESULT Render_Buffer(void) override;
	virtual HRESULT Change_Color(D3DXCOLOR _Color) override;

	static CVTXSphereColor* Create(_Device pDevice, _float fRadius = 0.25f, _uint iSlice = 36, _uint iStack = 36, _vec4 vColor = _vec4(1.f,1.f,1.f,1.f));
	virtual CComponent* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;

	_float m_fRadius;
	_uint m_iSlice;
	_uint m_iStack;
	_vec4 m_vColor;
	LPD3DXMESH m_pMesh;
};
END_NAMESPACE

#endif // !__VIBUFFER_SPHERECOLOR_H__