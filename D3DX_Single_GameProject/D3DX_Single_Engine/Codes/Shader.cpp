#include "Shader.h"

USING (Engine)

CShader::CShader(_Device pDevice)
	:m_pDevice(pDevice)
	, m_pEffect(nullptr)
	, m_pErrMsg(nullptr)
{
	m_pDevice->AddRef();
}

CShader::CShader(const CShader& other)
	: m_pDevice(other.m_pDevice)
	, m_pEffect(other.m_pEffect)
{
	m_pDevice->AddRef();
	m_pEffect->AddRef();
}

HRESULT CShader::Ready_Shader(const _tchar* pFilePath)
{
	/*���� m_pEffect ���� �ְ� m_pErrMsg ���� ���� ��� : ���������� ���̴��� ����
	���� m_pEffect ���� �ְ� m_pErrMsg ���� �ִ� ��� : ���̴� �ڵ� ��� ����, �������� ���ϴ� �� �ƴ�
	���� m_pEffect ���� ���� m_pErrMsg ���� �ִ� ��� : ���̴� �ڵ� ������ �־ ������ ���� ��Ȳ*/


	if (FAILED(D3DXCreateEffectFromFile(m_pDevice,
		pFilePath,
		NULL,	// ���̴����� ��� ������ ��ũ�� ����
		NULL,	// ���̴����� ��� ������ ���� ����
		D3DXSHADER_DEBUG,	//���̴��� �ܺο��� ������ϰڴٴ� �ǹ�(�ܺ� ���̺귯���� �̿��Ͽ� ������� �� ���� �ֱ� ������ ������ ����Ͽ� ����)
		NULL,	// ���� ���ҽ� ������ ��� ������ ���ΰ�(null�ΰ�� �׷��� �޸𸮿� �˾Ƽ� ����)
		&m_pEffect,
		&m_pErrMsg)))	//!!�߿�!! : ������� �Ұ��������� ���� �� ��� ���� ��� �� �޽����� ���ڿ��� ���·� �����ϱ� ���� ������ �޸� ����
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader Error", MB_OK);
		return E_FAIL;
	}

	else if (nullptr != m_pErrMsg)
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader warning", MB_OK);
	}

	return S_OK;
}

CShader* CShader::Create(_Device pDevice, const _tchar* pFilePath)
{
	CShader* pInstance = new CShader(pDevice);

	if (FAILED(pInstance->Ready_Shader(pFilePath)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
	return new CShader(*this);
}

void CShader::Free(void)
{
	Safe_Release(m_pEffect);
	Safe_Release(m_pDevice);
}

