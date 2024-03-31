#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"

struct VS_IN
{
	float3 vPos : POSITION;
	float2 vUV : TEXCOORD;
};

struct VS_OUT
{
	float4 vPos : SV_Position;
	float2 vUV : TEXCOORD;
};

// GrayFilter
VS_OUT VS_GrayFilter(VS_IN _in)
{
	VS_OUT output = (VS_OUT) 0.f;
	output.vPos = float4(_in.vPos * 2.f, 1.f);	// rect mesh의 uv 가 0.5 단위이므로 2배
	output.vUV = _in.vUV;
	
	return output;
}

float4 PS_GrayFilter(VS_OUT _in) : SV_Target
{
	float4 vColor = g_postprocess.Sample(g_sam_0, _in.vUV);
    float aver = (vColor.r + vColor.g + vColor.b) / 3.f;
	vColor.rgb = float3(aver, aver, aver);
	
	return vColor;
}

// Distortion
VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
	
    return output;
}

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    float2 vScreenUV = _in.vPos.xy / g_RenderResolution;	// SV_Position으로 입력된 값은 PS를 호출한 pixel의 좌표 담음
	
	// distortion 1
	//vScreenUV.y += cos((vScreenUV.x + (g_time * (speed))) * (frequency)) * (vibration);
    //vScreenUV.y += cos((vScreenUV.x + (g_time * 0.1f)) * 40.f) * 0.1f;
	
	// distortion 2
	if (g_btex_0)
    {
        float2 vUV = _in.vUV;
        vUV.x += g_time * 0.1f;
		
		// Nosie Texture에서 값 Sampling해와 UV값 노이즈에 사용
        float2 vNosie = g_tex_0.Sample(g_sam_0, vUV).xy;
        vNosie = (vNosie.xy - 0.5f) * 0.1f;	// UV 노이즈 범위를 -0.n~0.n으로 옮김
        vScreenUV += vNosie;
    }
	
    vColor = g_postprocess.Sample(g_sam_0, vScreenUV);
    return vColor;
}

#endif