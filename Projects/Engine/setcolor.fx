#ifndef _SETCOLOR
#define _SETCOLOR

#include "value.fx"
#define TEX_WIDTH g_int_0
#define TEX_HEIGHT g_int_1
RWTexture2D<float4> g_TargetTex : register(u0);

// 그룹의 스레드 수 : 한 그룹이 가질 수 있는 최대 스레드는 1024 (HLSL 5.0)
[numthreads(32, 32, 1)]
void CS_SetColor(uint3 id : SV_DispatchThreadID)
{
    if (TEX_WIDTH <= (int) id.x || TEX_HEIGHT <= (int) id.y)
        return;
    
    g_TargetTex[id.xy] = float4(g_vec4_0.rgb, 1.f);
}

#endif