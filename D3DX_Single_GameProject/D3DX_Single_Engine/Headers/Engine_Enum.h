#pragma once
#ifndef __ENGINE_ENUM_H__
#define __ENGINE_ENUM_H__

BEGIN_NAMESPACE(Engine)

//������Ʈ Ÿ��
// DYNAMIC�� ��� �� �����Ӹ��� ������ �ʿ��� ������Ʈ
enum class COMPONENT_ID { ID_DYNAMIC, ID_STATIC, ID_END };

//Ʈ������ ���� ����
enum class TRANSFORM_INFO { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };

//������ ��� (��üȭ�� / âȭ��)
enum class WINMODE {MODE_FULL,MODE_WIN};

//�ؽ��� Ÿ�� (��� �ؽ���, ť�� �ؽ���)
enum class TEXTYPE {TEX_NORMAL,TEX_CUBE,TEX_END};

//�޽� Ÿ�� (����ƽ, ���̳���, ������̼�)
enum class MESHTYPE { MESH_STATIC, MESH_DYNAMIC, MESH_NAVI, MESH_END };

//���� Ÿ�� (�ֿ켱, ���ľ���, (����ó��: ����-�ֿ켱, ������, ����), ���̾�������, UI
enum class RENDERID { RENDER_PRIORITY, RENDER_NOALPHA, RENDER_PRIORITY_ALPHA, RENDER_HALFALPHA, RENDER_ALPHA, RENDER_WIREFRAME, RENDER_UI, RENDER_END };


END_NAMESPACE


#endif // !__ENGINE_ENUM_H__
