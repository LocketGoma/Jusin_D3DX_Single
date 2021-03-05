#pragma once

#ifndef __VIBUFFER_CUBETEXTURE_H__
#define __VIBUFFER_CUBETEXTURE_H__

#include "VIBuffer.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CVTXCubeTexture : public CVIBuffer
{
private:
	explicit CVTXCubeTexture(_Device pDevice);
	explicit CVTXCubeTexture(const CVTXCubeTexture& other);
	virtual ~CVTXCubeTexture() = default;

public:
	// CVIBuffer��(��) ���� ��ӵ�
	virtual HRESULT Ready_Buffer(void) override;
	virtual HRESULT Render_Buffer(void) override;
	virtual HRESULT Change_Color(D3DXCOLOR _Color) override;			//�ؽ��� ���µ����� �Ⱦ�

	static CVTXCubeTexture* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;


};

END_NAMESPACE

#endif // !__VIBUFFER_CUBETEXTURE_H__