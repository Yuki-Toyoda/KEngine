#include "PassThrough.hlsli"
#include "GrayScale.hlsli"

ConstantBuffer<IntensityData> gIntensity : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t4> gDepth : register(t1);
SamplerState gSampler : register(s0);

float32_t4 main(VertexOutput input) : SV_TARGET
{
    // テクスチャ解像度取得
    uint32_t width, height;
    gTexture.GetDimensions(width, height);
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
    
	// 最終結果
    float32_t4 output;
	
    // 出力色リセット
    output.rgb = float32_t3(0.0f, 0.0f, 0.0f);
    output.a = 1.0f;
    
    // カーネルサイズを調整
    int kernelHalfWidth = int(gIntensity.intensity);
    int kernelSize = kernelHalfWidth * 2 + 1;
    float kernelWeight = 1.0 / (kernelSize * kernelSize);
    
    // フィルタリング結果の色
    float4 color = float4(0, 0, 0, 0);

    // カーネルの各サンプルを加算
    for (int y = -kernelHalfWidth; y <= kernelHalfWidth; ++y)
    {
        for (int x = -kernelHalfWidth; x <= kernelHalfWidth; ++x)
        {
            float2 offset = float2(x, y) * uvStepSize;
            color += gTexture.Sample(gSampler, input.texcoord + offset) * kernelWeight;
        }
    }
    
    output.rgb = color.rgb;
    
    // 出力する
    return output;
}