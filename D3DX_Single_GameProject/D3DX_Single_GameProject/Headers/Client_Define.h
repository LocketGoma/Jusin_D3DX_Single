#pragma once
#ifndef __CLINENT_DEFINE_H__

extern HWND g_hWnd;

#define WINCX 1280
#define WINCY 720

//�����Ұ���
#define TERRAIN_VTX_COUNTX 129
#define TERRAIN_VTX_COUNTZ 129
#define TERRAIN_VTX_INTERVAL 1.f

#define PLAYER_BASE_HITBOX 5.f

#define WEAPON_REDUCION_SIZE		0.0005f		//���� ���� �̹��� ��� ����
#define BASE_ENEMY_REDUCION_SIZE	0.0025f		//�� ���� �̹��� ��� ����
#define WEAPON_REDUCION_VECTOR		_vec3(WEAPON_REDUCION_SIZE,WEAPON_REDUCION_SIZE,WEAPON_REDUCION_SIZE)		//���� ���� �̹��� ��� ����
#define BASE_ENEMY_REDUCION_VECTOR	_vec3(BASE_ENEMY_REDUCION_SIZE,BASE_ENEMY_REDUCION_SIZE,BASE_ENEMY_REDUCION_SIZE)		//�� ���� �̹��� ��� ����
#define ONEMINUTE					60.00f		//60��

//�ɼ� ������ ������ ��������
#define NO_EVENT		0x00
#define OBJ_DEAD		0x01
#define MANAGER_OUT		0x02
#define FATAL_ERROR		0xFF

#define CHANGE_SCENE	0x10

#define __CLINENT_DEFINE_H__
#endif
