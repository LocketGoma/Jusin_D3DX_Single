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
	//������ ���� ����
	_ulong Get_TriCount();

public :
	virtual HRESULT Ready_Buffer();
	virtual HRESULT Render_Buffer();

protected:
	LPDIRECT3DVERTEXBUFFER9			m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB;

	_ulong							m_dwVTXSize;		//���ؽ� ������
	_ulong							m_dwVTXCount;		//���ؽ� ����
	_ulong							m_dwFVF;			//FVF
	_ulong							m_dwIndexSize;		//�ε��� ����
	_ulong							m_dwTriCount;		//������ ����
	D3DFORMAT						m_IndexFormat;		//�ε��� ����


	//�ʿ��ϸ� ����� ������
	//void* m_pVertexArr;
	//void* m_pIndexArr;

public:
	virtual void Free(void);

};

END_NAMESPACE

#endif // !__VIBUFFER_H__
