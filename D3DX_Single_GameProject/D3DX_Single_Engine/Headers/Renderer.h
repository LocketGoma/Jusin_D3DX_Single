#pragma once

#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "Base.h"
#include "GameObject.h"


//enum class RENDERID { RENDER_PRIORITY, RENDER_NOALPHA, RENDER_PRIORITY_ALPHA, RENDER_HALFALPHA, RENDER_ALPHA, RENDER_WIREFRAME, RENDER_UI, RENDER_END };

BEGIN_NAMESPACE(Engine)

class CManagement;

class ENGINE_DLL CRenderer : public CBase
{
	DECLARE_SINGLETON(CRenderer)

private:
	CRenderer();
	virtual ~CRenderer() = default;

public:
	HRESULT Set_Device(_Device pDevice, LPD3DXSPRITE pSprite = nullptr, LPD3DXFONT pFont = nullptr);

public:
	HRESULT Add_RenderList(RENDERID eRenderID, class CGameObject* pGameObject);
	HRESULT Render_RenderList(HWND hWND = nullptr);

	void Set_Visualble_DebugBuffer(_bool bVisual);

	HRESULT Clear_RenderList();

private:
	HRESULT Render_Priority();
	HRESULT Render_NoAlpha();
	HRESULT Render_Priority_Alpha();
	HRESULT Render_HalfAlpha();
	HRESULT Render_Alpha();
	HRESULT Render_Wireframe();
	HRESULT Render_Effect();
	HRESULT Render_NO_ZEnable_NoAlpha();
	HRESULT Render_Terminal_NoAlpha();
	HRESULT Render_UI();
	HRESULT Render_UI_AlphaBlend();
	HRESULT Render_Scene();

	//∑£¥ı≈∏∞Ÿ, Ω¶¿Ã¥ı
	HRESULT	Render_Original();
	HRESULT	Render_Deferred();
	HRESULT	Render_LightAcc();
	HRESULT	Render_Blend();


public:
	virtual void Free() override;

private:
	typedef std::list<class CGameObject*>	GAMEOBJECTS;	
	GAMEOBJECTS	m_GameObjects[(_uint)RENDERID::RENDER_END];

	_Device m_pDevice;
	LPD3DXSPRITE m_pSprite;
	LPD3DXFONT	m_pFont;

	Engine::CManagement* m_pManagement = nullptr;

	_bool m_bVisuableDebug;

	//µ∆€µÂ Ω¶¿Ã¥ı øÎ
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

};

END_NAMESPACE

#endif // !__RENDERER_H__