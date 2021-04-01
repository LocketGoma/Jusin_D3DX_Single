#include "RenderTarget.h"

USING(Engine)

CRenderTarget::CRenderTarget(_Device pDevice)
	: m_pDevice(pDevice)
	, m_pTargetSurface(nullptr)
	, m_pOldTargetSurface(nullptr)
	, m_pTargetTexture(nullptr)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
{
	m_pDevice->AddRef();
}

HRESULT CRenderTarget::Ready_RenderTarget(const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color)
{
	if (FAILED(D3DXCreateTexture(m_pDevice,
		iWidth,
		iHeight,
		1,	// mipmap
		D3DUSAGE_RENDERTARGET,
		Format,
		D3DPOOL_DEFAULT,
		&m_pTargetTexture)))
		return E_FAIL;

	m_ClearColor = Color;

	// �ؽ�ó�κ��� ����Ÿ�� ��Ҹ� �����Ѵ�. ù��° : ���� �Ӹ��� ������� ��� ���� ������ �����ϳ� �츮�� �ϳ��� ���� 0������ ����
	m_pTargetTexture->GetSurfaceLevel(0, &m_pTargetSurface);
	Safe_Release(m_pTargetTexture);

	return S_OK;
}

void CRenderTarget::SetUp_OnGraphicDevice(const _uint& iIndex)
{
	m_pDevice->GetRenderTarget(iIndex, &m_pOldTargetSurface);
	m_pDevice->SetRenderTarget(iIndex, m_pTargetSurface);
}

void CRenderTarget::Release_OnGraphicDevice(const _uint& iIndex)
{
	m_pDevice->SetRenderTarget(iIndex, m_pOldTargetSurface);
	Safe_Release(m_pOldTargetSurface);
}

void CRenderTarget::Clear_RenderTarget(void)
{
	m_pDevice->GetRenderTarget(0, &m_pOldTargetSurface);

	m_pDevice->SetRenderTarget(0, m_pTargetSurface);

	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET, m_ClearColor, 1.f, 0);

	m_pDevice->SetRenderTarget(0, m_pOldTargetSurface);

	Safe_Release(m_pOldTargetSurface);
}

//���� �� ����� ��� �ϴ°�?
HRESULT CRenderTarget::Ready_DebugBuffer(const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{
	FAILED_CHECK_RETURN(m_pDevice->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,
		0, // ���� ���� ��� �� ���� 0(D3DUSAGE_DYNAMIC : ��ƼŬ�� ������ ��)
		FVF_SCREEN,
		D3DPOOL_MANAGED, // �Ϲ������� ���� ���� ��� �� Managed, �������� ��� �� Default ���
		&m_pVB, NULL), E_FAIL);

	FAILED_CHECK_RETURN(m_pDevice->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB, NULL), E_FAIL);


	VTXSCREEN* pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPosition = _vec4(fX - 0.5f, fY - 0.5f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec4((fX + fSizeX) - 0.5f, fY - 0.5f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec4((fX + fSizeX) - 0.5f, (fY + fSizeY) - 0.5f, 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec4(fX - 0.5f, (fY + fSizeY) - 0.5f, 0.f, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();


	INDEX16* pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CRenderTarget::Render_DebugBuffer(void)
{
	m_pDevice->SetTexture(0, m_pTargetTexture);

	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pDevice->SetFVF(FVF_SCREEN);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

}

void CRenderTarget::SetUp_OnShader(LPD3DXEFFECT& pEffect, const char* pConstantTable)
{
	pEffect->SetTexture(pConstantTable, m_pTargetTexture);
}

CRenderTarget* CRenderTarget::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color)
{
	CRenderTarget* pInstance = new CRenderTarget(pGraphicDev);

	if (FAILED(pInstance->Ready_RenderTarget(iWidth, iHeight, Format, Color)))
		Safe_Release(pInstance);

	return pInstance;
}

void CRenderTarget::Free(void)
{
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	Safe_Release(m_pOldTargetSurface);
	Safe_Release(m_pTargetSurface);
	Safe_Release(m_pDevice);
}
