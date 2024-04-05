#ifndef COMBIND
#define COMBIND


Texture2D<float4> RenderTargetCopy : register(t0);
Texture2D<float4> BloomTexture : register(t1);

RWTexture2D<float4> RenderTarget : register(u0);

// 1901 950 475 237 118

[numthreads(32, 32, 1)]
void CS_Combind(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint RTWidth, RTHeight;
    RenderTarget.GetDimensions(RTWidth, RTHeight);
    
    uint BloomWidth, BloomHeight;
    BloomTexture.GetDimensions(BloomWidth, BloomHeight);
    
    if (RTWidth <= dispatchThreadID.x || RTHeight <= dispatchThreadID.y)
    {
        return;
    }
    
    if (BloomWidth <= dispatchThreadID.x || BloomHeight <= dispatchThreadID.y)
    {
        return;
    }
    
    RenderTarget[dispatchThreadID.xy] = saturate(RenderTargetCopy[dispatchThreadID.xy] + BloomTexture[dispatchThreadID.xy]);
}

#endif