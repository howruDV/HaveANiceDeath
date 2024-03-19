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
	output.vPos = float4(_in.vPos * 2.f, 1.f);	// rect mesh�� uv �� 0.5 �����̹Ƿ� 2��
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
    float2 vScreenUV = _in.vPos.xy / g_RenderResolution;	// SV_Position���� �Էµ� ���� PS�� ȣ���� pixel�� ��ǥ ����
	
	// distortion 1
	//vScreenUV.y += cos((vScreenUV.x + (g_time * (speed))) * (frequency)) * (vibration);
    //vScreenUV.y += cos((vScreenUV.x + (g_time * 0.1f)) * 40.f) * 0.1f;
	
	// distortion 2
	if (g_btex_0)
    {
        float2 vUV = _in.vUV;
        vUV.x += g_time * 0.1f;
		
		// Nosie Texture���� �� Sampling�ؿ� UV�� ����� ���
        float2 vNosie = g_tex_0.Sample(g_sam_0, vUV);
        vNosie = (vNosie.xy - 0.5f) * 0.1f;	// UV ������ ������ -0.n~0.n���� �ű�
        vScreenUV += vNosie;
    }
	
    vColor = g_postprocess.Sample(g_sam_0, vScreenUV);
    return vColor;
}

#endif