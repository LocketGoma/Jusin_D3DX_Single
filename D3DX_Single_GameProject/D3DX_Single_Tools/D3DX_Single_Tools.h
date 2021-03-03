
// D3DX_Single_Tools.h: D3DX_Single_Tools 애플리케이션의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.


// CD3DXSingleToolsApp:
// 이 클래스의 구현에 대해서는 D3DX_Single_Tools.cpp을(를) 참조하세요.
//

class CD3DXSingleToolsApp : public CWinApp
{
public:
	CD3DXSingleToolsApp() noexcept;


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CD3DXSingleToolsApp theApp;
