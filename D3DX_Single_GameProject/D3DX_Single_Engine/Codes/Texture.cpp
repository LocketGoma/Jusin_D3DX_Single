#include "Texture.h"

USING(Engine)

CTexture::CTexture(_Device pDevice)
    : CGraphicResources(pDevice)
{
}

CTexture::CTexture(const CTexture& other)
    : CGraphicResources(other)
{
	_uint	iContainerSize = other.m_vecTexture.size();
	m_vecTexture.reserve(iContainerSize);

	m_vecTexture = other.m_vecTexture;

	for (auto& iter : m_vecTexture)
	{
		iter->AddRef();
	}
}

HRESULT CTexture::Ready_Texture(const _tchar* pPath, TEXTYPE eType, const _uint& iCount)
{
	m_vecTexture.reserve(iCount);

	IDirect3DBaseTexture9* pTexture = nullptr;

	for (_uint i = 0; i < iCount; ++i)
	{
		TCHAR	szFileName[256] = L"";

		wsprintf(szFileName, pPath, i);

		switch (eType)
		{
		case TEXTYPE::TEX_NORMAL:
			if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFileName, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;
			break;

		case TEXTYPE::TEX_CUBE:
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pDevice, szFileName, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		}

		m_vecTexture.push_back(pTexture);
	}

	return S_OK;
}
//여기다가 인덱스 값 넣으면 인덱스에 맞는 텍스쳐 리턴해줘요
HRESULT CTexture::Set_Texture(const _uint& iIndex , const _uint& iChannel)
{
	if (m_vecTexture.size() < iIndex)
	{
		PRINT_LOG(L"Error", L"Texture size is 0");
		return E_FAIL;
	}

	// 텍스쳐를 그리기 하는 함수
	m_pDevice->SetTexture(iChannel, m_vecTexture[iIndex]);
	
	return S_OK;
}


CTexture* CTexture::Create(_Device pDevice, const _tchar* pPath, TEXTYPE eType, const _uint& iCnt)
{
	CTexture* pInstance = new CTexture(pDevice);

	if (FAILED(pInstance->Ready_Texture(pPath, eType, iCnt)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
	return new CTexture(*this);
}

void CTexture::Free()
{
	CGraphicResources::Free();

	for (auto& iter : m_vecTexture)
	{
		Safe_Release(iter);
	}

	m_vecTexture.clear();
}

