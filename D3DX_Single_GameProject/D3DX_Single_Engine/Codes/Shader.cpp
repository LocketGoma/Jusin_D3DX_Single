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
	/*만약 m_pEffect 값이 있고 m_pErrMsg 값이 없는 경우 : 정상적으로 쉐이더라 구현
	만약 m_pEffect 값이 있고 m_pErrMsg 값도 있는 경우 : 쉐이더 코드 경고 상태, 컴파일을 못하는 건 아님
	만약 m_pEffect 값이 없고 m_pErrMsg 값이 있는 경우 : 쉐이더 코드 문제가 있어서 컴파일 에러 상황*/


	if (FAILED(D3DXCreateEffectFromFile(m_pDevice,
		pFilePath,
		NULL,	// 쉐이더에서 사용 가능한 매크로 파일
		NULL,	// 쉐이더에서 사용 가능한 포함 파일
		D3DXSHADER_DEBUG,	//쉐이더를 외부에서 디버깅하겠다는 의미(외부 라이브러리를 이용하여 디버깅을 할 수도 있기 때문에 만약을 대비하여 포함)
		NULL,	// 각정 리소스 정보를 어디에 저장할 것인가(null인경우 그래픽 메모리에 알아서 저장)
		&m_pEffect,
		&m_pErrMsg)))	//!!중요!! : 디버깅은 불가능하지만 에러 및 경고가 있을 경우 그 메시지를 문자열에 형태로 저장하기 위해 마련한 메모리 공간
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

