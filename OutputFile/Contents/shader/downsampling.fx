#ifndef DOWNSAMPLING
#define DOWNSAMPLING

// 큰 사이즈 텍스쳐
Texture2D<float4> inputTexture : register(t0);
// 작은 사이즈 텍스쳐
RWTexture2D<float4> outputTexture : register(u0);


[numthreads(32, 32, 1)]
void CS_DownSampling(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint inputWidth, inputHeight;
    inputTexture.GetDimensions(inputWidth, inputHeight);

    uint outputWidth, outputHeight;
    outputTexture.GetDimensions(outputWidth, outputHeight);
    
    if (outputWidth <= dispatchThreadID.x || outputHeight <= dispatchThreadID.y)
    {
        return;
    }
    
    // 입력 텍스처에서 픽셀 값을 읽어올 위치 계산
    uint2 inputPixelPos = uint2(dispatchThreadID.xy * 2);
    
    float2 inputUV = float2(dispatchThreadID.xy) / float2(outputWidth, outputHeight);
    float2 Location = inputUV * float2(inputWidth, inputHeight);
    
    // 입력 텍스처에서 샘플링하여 새로운 텍스처에 쓰기
    float4 color = inputTexture.Load(int3(Location.x, Location.y, 0));
    
    outputTexture[dispatchThreadID.xy] = color;
}

#endif