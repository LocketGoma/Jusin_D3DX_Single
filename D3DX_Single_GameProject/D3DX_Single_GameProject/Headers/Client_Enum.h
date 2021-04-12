#pragma once

#ifndef __CLIENT_ENUM_H__
#define __CLIENT_ENUM_H__

enum class ESceneType{ SCENE_TEST, SCENE_STAGE1, SCENE_STAGE2, SCENE_STAGE3, SCENE_LOADING,  STAGE_LOGO, SCENE_END };

enum class LOADINGID { LOADING_TEST, LOADING_STAGE1, LOADING_STAGE2, LOADING_BOSS, LOADING_END };

enum class RESOURCETYPE { RESOURCE_BUFFER, RESOURCE_TEXTURE, RESOURCE_MESH, RESOURCE_END };

//����Ʈ ��¿�. ��� ��ĥ���ΰ�.
enum class eEffectAxis{	AXIS_X, AXIS_Y, AXIS_Z, END };

//enum class LAYERID { LAYER_ENVIRONMENT, LAYER_GAMELOGIC, LAYER_UI, LAYER_END };


#endif // !__CLIENT_ENUM_H__
