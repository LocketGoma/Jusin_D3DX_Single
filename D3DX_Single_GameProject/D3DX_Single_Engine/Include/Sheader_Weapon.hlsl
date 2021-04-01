matrix g_matWorld;
matrix g_matView;
matrix g_matProj;

texture g_WorldTexture;

sampler	BaseSampler = sampler_state
{
	texture = g_WorldTexture;

	minfilter = linear;
	magfilter = linear;
};


texture g_WeaponTexture;

sampler	WeaponSampler = sampler_state
{
	texture = g_WeaponTexture;

	minfilter = linear;
	magfilter = linear;
};

struct PS_IN
{
	vector vColor : COLOR0;
	float2 vTexUV : TEXCOORD0;

};

struct PS_OUT
{
	vector vColor : COLOR0;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;



	return Out;
}

technique Weapon_Render
{
	pass
	{
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};