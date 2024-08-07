#pragma once
#ifndef __CLINENT_DEFINE_H__
#define __CLINENT_DEFINE_H__

extern HWND g_hWnd;
extern bool g_bViewCollider;
extern bool g_bViewNavimesh;
extern bool g_bViewDebugTarget;
extern bool g_bEndingTimeDelay;
extern float g_fEndingTimeDelay;

#define WINCX 1280
#define WINCY 720

//수정할거임
#define TERRAIN_VTX_COUNTX 129
#define TERRAIN_VTX_COUNTZ 129
#define TERRAIN_VTX_INTERVAL 1.f

#define PLAYER_BASE_HITBOX 5.f

#define WEAPON_REDUCION_SIZE		0.0005f		//무기 관련 이미지 축소 비율
#define BASE_ITEM_REDUCION_SIZE		0.0045f		//아이템 관련 이미지 축소 비율
#define BASE_ENEMY_REDUCION_SIZE	0.0025f		//적 관련 이미지 축소 비율
#define BOSS_ENEMY_REDUCION_SIZE	0.0015f		//스트라이더 이미지 축소 비율
#define ITEM_REDUCION_VECTOR		_vec3(BASE_ITEM_REDUCION_SIZE,BASE_ITEM_REDUCION_SIZE,BASE_ITEM_REDUCION_SIZE)		//아이템 관련 이미지 축소 벡터
#define WEAPON_REDUCION_VECTOR		_vec3(WEAPON_REDUCION_SIZE,WEAPON_REDUCION_SIZE,WEAPON_REDUCION_SIZE)		//무기 관련 이미지 축소 벡터
#define BASE_ENEMY_REDUCION_VECTOR	_vec3(BASE_ENEMY_REDUCION_SIZE,BASE_ENEMY_REDUCION_SIZE,BASE_ENEMY_REDUCION_SIZE)		//적 관련 이미지 축소 벡터
#define BOSS_ENEMY_REDUCION_VECTOR	_vec3(BOSS_ENEMY_REDUCION_SIZE,BOSS_ENEMY_REDUCION_SIZE,BOSS_ENEMY_REDUCION_SIZE)		//적 관련 이미지 축소 벡터

#define RESET_VECTOR _vec3(1.0f,1.0f,1.0f)
#define ZERO_VECTOR _vec3(0.0f,0.0f,0.0f)

#define ONEMINUTE					60.00f		//60초

//옵션 넣을거 있으면 넣으세요
#define NO_EVENT		0x00
#define OBJ_DEAD		0x01
#define MANAGER_OUT		0x02
#define READY_EVENT		0x04
#define OBJ_DOWN		0x08		//사용불능
#define CHANGE_SCENE	0x10
#define FATAL_ERROR		0xFF




//디버그 전용
#define DEBUG_MODE
#define DEBUG_TIMESPEED	1

#endif
