#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"
#include "struct.fx"

// const Param
#define mapCol      g_int_0
#define mapRow      g_int_1
#define vSliceUV    g_vec2_0
#define TileAtlas   g_tex_0

StructuredBuffer<FTileInfo> g_TileInfo : register(t20);

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

VS_OUT VS_TileMap(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.vPos = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_TileMap(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (g_btex_0)
    {
        // tile info
        float2 vUV = _in.vUV * float2(mapCol, mapRow);  // 전체 tilemap 배율 uv
        int2 tilePos = (int2) floor(vUV);               // 속한 row, col 찾음
        vUV -= tilePos;                                 // 한 tile 내에서의 uv
        int idx = tilePos.y * mapCol + tilePos.x;       // 전체 tilemap에서 속한 tile idx
        
        // sampling
        if (!g_TileInfo[idx].bRender)
            discard;
        
        vUV = g_TileInfo[idx].vLeftTopUV + (vSliceUV * vUV);
        vColor = g_tex_0.Sample(g_sam_0, vUV);
    }
    
    return vColor;
}

#endif