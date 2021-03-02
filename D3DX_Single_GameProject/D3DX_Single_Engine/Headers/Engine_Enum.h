#pragma once
#ifndef __ENGINE_ENUM_H__
#define __ENGINE_ENUM_H__

BEGIN_NAMESPACE(Engine)

//컴포넌트 타입
// DYNAMIC의 경우 매 프레임마다 갱신이 필요한 컴포넌트
enum class COMPONENT_ID { ID_DYNAMIC, ID_STATIC, ID_END };

//트랜스폼 벡터 정보
enum class TRANSFORM_INFO { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };

//트랜스폼 축 정보
enum class ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

//윈도우 모드 (전체화면 / 창화면)
enum class WINMODE {MODE_FULL,MODE_WIN};

//기본 버퍼 타입 (삼각형, 사각형, 터레인, 큐브)
enum class BUFFERID { BUFFER_TRICOL, BUFFER_RCTEX, BUFFER_TERRAINTEX, BUFFER_CUBETEX, MESH_,BUFFER_END };

//텍스쳐 타입 (노멀 텍스쳐, 큐브 텍스쳐)
enum class TEXTYPE {TEX_NORMAL,TEX_CUBE,TEX_END};

//메시 타입 (스테틱, 다이나믹, 내비게이션)
enum class MESHTYPE { MESH_STATIC, MESH_DYNAMIC, MESH_NAVI, MESH_END };

//랜더 타입 (최우선, 알파없음, (알파처리: 알파-최우선, 반투명, 투명), 와이어프레임, UI
enum class RENDERID { RENDER_PRIORITY, RENDER_NOALPHA, RENDER_PRIORITY_ALPHA, RENDER_HALFALPHA, RENDER_ALPHA, RENDER_WIREFRAME, RENDER_UI, RENDER_END };


END_NAMESPACE


#endif // !__ENGINE_ENUM_H__
