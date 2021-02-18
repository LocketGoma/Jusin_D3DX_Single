#include "GraphicResources.h"

USING(Engine)

CGraphicResources::CGraphicResources(_Device pDevice)
	: m_pDevice(pDevice)	
{
	m_bIsPrototype = true;

	m_pDevice->AddRef();
}
CGraphicResources::CGraphicResources(const CGraphicResources& other)
	:m_pDevice(other.m_pDevice)
{
	m_bIsPrototype = false;

	m_pDevice->AddRef();

}

void CGraphicResources::Free()
{
	Safe_Release(m_pDevice);
}
