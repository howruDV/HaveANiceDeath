#ifndef UPSAMPLING
#define UPSAMPLING

#define INTERPOL_LEVEL 3
SamplerState g_sam_0 : register(s0);
SamplerState g_sam_1 : register(s1);
Texture2D<float4> inputTexture : register(t0);      // Resource Tex (작은거)
RWTexture2D<float4> outputTexture : register(u0);   // Target Tex (큰거)

[numthreads(32, 32, 1)]
void CS_UpSampling(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    // 입력 텍스처의 너비와 높이
    uint inputWidth, inputHeight;
    inputTexture.GetDimensions(inputWidth, inputHeight);
    
    // 출력 텍스처의 너비와 높이 (입력 텍스처의 두 배)
    uint outputWidth, outputHeight;
    outputTexture.GetDimensions(outputWidth, outputHeight);
    
    if (outputWidth <= dispatchThreadID.x || outputHeight <= dispatchThreadID.y)
    {
        return;
    }
    
    // 입력 텍스처에서 샘플링할 위치 계산
    float2 inputUV = float2(dispatchThreadID.xy) / float2(outputWidth, outputHeight);
    float2 Location = inputUV * float2(inputWidth, inputHeight);
    Location = round(Location);
    
    // 입력 텍스처에서 샘플링하여 새로운 텍스처에 쓰기
    float4 color = inputTexture.Load(int3(Location.x, Location.y, 0));
    
    for (uint i = 1; i <= INTERPOL_LEVEL; ++i)
    {
        if (i / 2 == 0)
        {
            color += inputTexture.Load(int3(Location.x - i, Location.y - i, 0));
            color += inputTexture.Load(int3(Location.x - i, Location.y + i, 0));
            color += inputTexture.Load(int3(Location.x + i, Location.y - i, 0));
            color += inputTexture.Load(int3(Location.x + i, Location.y + i, 0));
        }
        else
        {
            color += inputTexture.Load(int3(Location.x + i, Location.y, 0));
            color += inputTexture.Load(int3(Location.x - i, Location.y, 0));
            color += inputTexture.Load(int3(Location.x, Location.y - i, 0));
            color += inputTexture.Load(int3(Location.x, Location.y + i, 0));
        }
    }
    
    color /= (INTERPOL_LEVEL * 4 + 1);
    
    
    outputTexture[dispatchThreadID.xy] = color;
}

#endif