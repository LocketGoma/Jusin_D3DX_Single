#ifndef Engine_Struct_h__
#define Engine_Struct_h__

//여기는 메시 관리하게 될때 다시 볼것.

BEGIN_NAMESPACE(Engine)

typedef struct D3DVERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	FLOAT tu, tv;

}D3VERTEX;
const DWORD    FVF_D3DVERTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;



typedef struct tagVertexColor
{
	_vec3		vPosition;
	_ulong		dwColor;

}VTXCOL;

const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

typedef struct tagVertexTex
{
	_vec3		vPosition;		// 12
	_vec3		vNormal;		// 12
	_vec2		vTexUV;			// 8

}VTXTEX;

const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

typedef struct tagVertexCube
{
	_vec3		vPosition;
	_vec3		vTexUV;

}VTXCUBE;
//여기 Normal 값 추가해야하지 않음?
const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);	// 텍스쳐 UV를 FLOAT형 3개의 값으로 표현하겠다는 옵션

typedef struct tagVertexCubeColorNormal
{
	_vec3	vPosition;
	_vec3	vNormal;
	_ulong	wColor;
}VTX_CUBE_COLOR_N;

const _ulong	FVF_CUBECOLORN = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX0;


typedef struct tagVertex_Screen
{
	_vec4		vPosition;
	_vec2		vTexUV;

}VTXSCREEN;

const _ulong FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1; // RHW 윈도위 상태의 위치 값을 표현하는 자료형


typedef struct INDEX16
{
	_ushort		_0;
	_ushort		_1;
	_ushort		_2;

}INDEX16;

typedef struct INDEX32
{
	_ulong		_0;
	_ulong		_1;
	_ulong		_2;

}INDEX32;

typedef	struct D3DFRAME_DERIVED : public D3DXFRAME
{
	_matrix			CombinedTranformationMatrix;	// 부모 뼈와 곱셈을 끝낸 결과 값을 저장하기 위한 행렬

}D3DXFRAME_DERIVED;

typedef	struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9* ppTexture;

	LPD3DXMESH				pOriMesh;	// 최초 로드 시점의 상태를 가지고 있는 메쉬 객체(불변)

	_ulong					dwNumBones;	// 메쉬가 지닌 모든 뼈의 개수를 보관

	_matrix* pFrameOffsetMatrix;
	// 애니메이션 정보를 포함한 최초의 행렬 상태 

	_matrix** ppFrameCombinedMatrix;
	// 뼈 구조체가 갖고 있는 CombinedTransformationMatrix의 주소값들을 보관하는 포인터

	_matrix* pRenderingMatrix;
	// 최종적으로 출력을 위한 행렬의 상태
	// pFrameOffsetMatrix * (*ppFrameCombinedMatrix)의 결과물

}D3DXMESHCONTAINER_DERIVED;




END_NAMESPACE


#endif // Engine_Struct_h__
