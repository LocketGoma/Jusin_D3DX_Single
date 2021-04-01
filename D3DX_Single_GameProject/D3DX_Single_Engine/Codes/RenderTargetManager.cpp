#include "RenderTargetManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderTargetManager)

Engine::CRenderTargetManager::CRenderTargetManager()
{

}

HRESULT CRenderTargetManager::Ready_RenderTarget(const _tchar* pTargetTag, LPDIRECT3DDEVICE9 pDevice, const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);
	if (pRenderTarget != nullptr)
	{
		return E_FAIL;
	}
	pRenderTarget = CRenderTarget::Create(pDevice, iWidth, iHeight, Format, Color);
	NULL_CHECK_RETURN(pRenderTarget, E_FAIL);

	m_mapRenderTarget.emplace(pTargetTag, pRenderTarget);


	return S_OK;
}
//멀티 랜더 타겟
HRESULT CRenderTargetManager::Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);

	if (nullptr == pRenderTarget)
		return E_FAIL;

	std::list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);

	if (nullptr == pMRTList)
	{
		std::list<CRenderTarget*>		MRTLIST;
		MRTLIST.push_back(pRenderTarget);
		m_mapMRT.emplace(pMRTTag, MRTLIST);
	}

	else
		pMRTList->push_back(pRenderTarget);

	return S_OK;
}

HRESULT CRenderTargetManager::Begin_MRT(const _tchar* pMRTTag)
{
	std::list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	NULL_CHECK_RETURN(pMRTList, E_FAIL);

	for (auto& iter : *pMRTList)
		iter->Clear_RenderTarget();

	_uint iIndex = 0;

	for (auto& iter : *pMRTList)
		iter->SetUp_OnGraphicDevice(iIndex++);

	return S_OK;
}

HRESULT CRenderTargetManager::End_MRT(const _tchar* pMRTTag)
{
	std::list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	NULL_CHECK_RETURN(pMRTList, E_FAIL);

	_uint iIndex = 0;

	for (auto& iter : *pMRTList)
		iter->Release_OnGraphicDevice(iIndex++);

	return S_OK;
}

HRESULT CRenderTargetManager::Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	if (FAILED(pRenderTarget->Ready_DebugBuffer(fX, fY, fSizeX, fSizeY)))
		return E_FAIL;

	return S_OK;
}

void CRenderTargetManager::Render_DebugBuffer(const _tchar* pMRTTag)
{
	std::list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	NULL_CHECK(pMRTList);

	for (auto& iter : *pMRTList)
		iter->Render_DebugBuffer();
}

void CRenderTargetManager::SetUp_OnShader(LPD3DXEFFECT& pEffect, const _tchar* pTargetTag, const char* pConstantTable)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pTargetTag);

	if (nullptr == pRenderTarget)
		return;

	pRenderTarget->SetUp_OnShader(pEffect, pConstantTable);
}

CRenderTarget* CRenderTargetManager::Find_RenderTarget(const _tchar* pTargetTag)
{
	auto	iter = find_if(m_mapRenderTarget.begin(), m_mapRenderTarget.end(), CTag_Finder(pTargetTag));

	if (iter == m_mapRenderTarget.end())
		return nullptr;

	return iter->second;
}

std::list<CRenderTarget*>* CRenderTargetManager::Find_MRT(const _tchar* pMRTTag)
{
	auto	iter = find_if(m_mapMRT.begin(), m_mapMRT.end(), CTag_Finder(pMRTTag));

	if (iter == m_mapMRT.end())
		return nullptr;

	return &iter->second;
}

void CRenderTargetManager::Free(void)
{
	for_each(m_mapRenderTarget.begin(), m_mapRenderTarget.end(), CDeleteMap());
	m_mapRenderTarget.clear();

	for (auto& iter : m_mapMRT)
		iter.second.clear();

	m_mapMRT.clear();
}
