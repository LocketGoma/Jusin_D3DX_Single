#pragma once
#ifndef __VIBUFFER_H__
#define __VIBUFFER_H__

#include "Engine_Include.h"
#include "GraphicResources.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CVIBuffer : public CGraphicResources
{
protected:
	explicit CVIBuffer(_Device pDevice);
	explicit CVIBuffer(const CVIBuffer& other);
	virtual ~CVIBuffer() = default;

public :
	//폴리곤 개수 리턴
	_ulong Get_TriCount();

public :
	virtual HRESULT Ready_Buffer();
	virtual HRESULT Render_Buffer();

protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;

	_ulong							m_dwVTXSize;		//버텍스 사이즈
	_ulong							m_dwVTXCount;		//버텍스 개수
	_ulong							m_dwFVF;			//FVF
	_ulong							m_dwIndexSize;		//인덱스 개수
	_ulong							m_dwTriCount;		//폴리곤 개수
	D3DFORMAT						m_IndexFormat;		//인덱스 포맷


	//필요하면 살려서 쓰세요
	//void* m_pVertexArr;
	//void* m_pIndexArr;

public:
	virtual void Free(void);

};

END_NAMESPACE

#endif // !__VIBUFFER_H__
