#pragma once
#ifndef __ENGINE_TYPEDEF_H__

typedef		bool				_bool;

typedef		signed char			_char;
typedef		signed char			_byte;
typedef		unsigned char		_ubyte;

typedef		wchar_t				_tchar;

typedef		signed short		_short;
typedef		unsigned short		_ushort;

typedef		signed int			_int;
typedef		unsigned int		_uint;

typedef		signed long			_long;
typedef		unsigned long		_ulong;

typedef		float				_float;
typedef		double				_double;

typedef		D3DXVECTOR2			_float2, _vec2;
typedef		D3DXVECTOR3			_float3, _vec3;
typedef		D3DXVECTOR4			_float4, _vec4;
typedef		D3DXMATRIX			_float4x4, _mat, _matrix;
typedef		LPDIRECT3DDEVICE9	_Device;

#define __ENGINE_TYPEDEF_H__
#endif