#pragma once
#ifndef __CLINENT_DEFINE_H__

extern HWND g_hWnd;

#define WINCX 1280
#define WINCY 720

//수정할거임
#define TERRAIN_VTX_COUNTX 129
#define TERRAIN_VTX_COUNTZ 129
#define TERRAIN_VTX_INTERVAL 1.f

//옵션 넣을거 있으면 넣으세요
#define NO_EVENT		0x00
#define OBJ_DEAD		0x01
#define MANAGER_OUT		0x02
#define FATAL_ERROR		0xFF

#define CHANGE_SCENE	0x10

#define __CLINENT_DEFINE_H__
#endif
