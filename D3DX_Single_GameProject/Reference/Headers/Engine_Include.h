#pragma once

#ifndef __ENGINE_INCLUDE_H__
#define __ENGINE_INCLUDE_H__

//D3DX9
#include "d3d9.h"
#include "d3dx9.h"

//Base Headers
#include <map>
#include <list>
#include <vector>
#include <algorithm>

#include <string>
#include <tchar.h>
#include <fstream>
#include <iostream>

#include <chrono>			//for DeltaTime Timer

#include <memory>
#include <assert.h>

#pragma warning(disable : 4251)	//dllexport를 사용하지 않은 클래스 발생시 생기는 경고문구 제어(제거)용. 주로 STL 사용시 발생함.	

#pragma comment(lib,"user32.lib")


//User Headers
#include "Engine_Define.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"
#include "Engine_Enum.h"
#include "Engine_Struct.h"

#include <io.h>
#include "fmod.h"
#pragma comment (lib, "fmodex_vc.lib")

////Debug
//#ifdef _DEBUG
//	#define _CRTDBG_MAP_ALLOC
//	#include <stdlib.h>
//	#include <crtdbg.h>
//
//	#ifndef DBG_NEW 
//
//	#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
//	#define new DBG_NEW 
//
//	#endif
//#endif

#endif // !__ENGINE_INCLUDE_H__
