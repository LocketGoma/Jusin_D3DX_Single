#pragma once

#ifndef __VIBUFFER_BATCH_TERRAIN_H__
#define __VIBUFFER_BATCH_TERRAIN_H__

#include "VIBuffer.h"
#include "VTXTerrain.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CVTXBatchTerrain : public CVTXTerrain
{
	explicit CVTXBatchTerrain(_Device pDevice);
	explicit CVTXBatchTerrain(const CVTXBatchTerrain& other);
	virtual ~CVTXBatchTerrain() = default;

public:
	const _vec3* Get_VTXPos(void) const;
	_ulong		Get_VTXCountX() const;
	_ulong		Get_VTXCountZ() const;
	void		Copy_Index(INDEX32* pIndex, const _ulong& dwTriCount);


public:
	// CVIBuffer을(를) 통해 상속됨
	virtual HRESULT Ready_Buffer(const _ulong& dwCountX, const _ulong& dwCountZ, const _ulong& dwVTXInterval);
	virtual HRESULT Render_Buffer(void) override;
	virtual HRESULT Change_Color(D3DXCOLOR _Color) override;


	static CVTXBatchTerrain* Create(_Device pDevice, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	virtual CComponent* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;

	//_ulong m_dwCountX;
	//_ulong m_dwCountZ;
	//_vec3* m_pPos;

};

END_NAMESPACE

#endif // !__VIBUFFER_BATCH_TERRAIN_H__