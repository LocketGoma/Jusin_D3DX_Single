#pragma once

#ifndef __SHADER_H__
#define __SHADER_H__

#include "Engine_Include.h"
#include "Base.h"
#include "Component.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CShader : public CComponent
{
private:
	explicit CShader(_Device pDevice);
	explicit CShader(const CShader& other);
	virtual ~CShader(void) = default;

public:
	LPD3DXEFFECT			Get_EffectHandle(void) { return m_pEffect; }
public:
	HRESULT					Ready_Shader(const _tchar* pFilePath);

private:
	_Device					m_pDevice;
	LPD3DXEFFECT			m_pEffect;		// 쉐이더 대표 컴객체
	LPD3DXBUFFER			m_pErrMsg;		// hlsl 언어를 컴파일 이후 발생하는 에러 메세지 상태를 저장하기 위한 메모리 공간

public:
	static CShader* Create(_Device pDevice, const _tchar* pFilePath);
	virtual CComponent* Clone(void* pArg = nullptr) override;	
	virtual void		Free(void);


	// CComponent을(를) 통해 상속됨

};

END_NAMESPACE

#endif // !__SHEADER_H__