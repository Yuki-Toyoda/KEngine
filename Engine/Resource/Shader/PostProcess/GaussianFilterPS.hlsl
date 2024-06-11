#include "PassThrough.hlsli"
#include "GrayScale.hlsli"

ConstantBuffer<IntensityData> gIntensity : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t4> gDepth : register(t1);
SamplerState gSampler : register(s0);

// ガウス関数を計算する関数
float Gaussian(float x, float sigma)
{
    return exp(-0.5 * (x * x) / (sigma * sigma)) / (sqrt(2.0 * 3.1415926535897932384626433832795) * sigma);
}

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
    
    // intensityが1より小さい場合はフィルタを適用せず、元のピクセルを返す
    if (gIntensity.intensity < 1.0f)
    {
        output = gTexture.Sample(gSampler, input.texcoord);
        return output;
    }
    
    // カーネルサイズを定義
    int kernelRadius = int(ceil(2.0 * gIntensity.intensity));
    int kernelSize = kernelRadius * 2 + 1;
    
    // ガウシアンカーネルの重みを計算
    float kernel[49];
    float totalWeight = 0.0;
    for (int i = -kernelRadius; i <= kernelRadius; ++i)
    {
        kernel[i + kernelRadius] = Gaussian(float(i), gIntensity.intensity);
        totalWeight += kernel[i + kernelRadius];
    }
    
    // フィルタリング結果の色
    float4 color = float4(0, 0, 0, 0);

     // カーネルの各サンプルを加算
    for (int y = -kernelRadius; y <= kernelRadius; ++y)
    {
        for (int x = -kernelRadius; x <= kernelRadius; ++x)
        {
            float2 offset = float2(float(x), float(y)) * uvStepSize;
            float weight = kernel[x + kernelRadius] * kernel[y + kernelRadius];
            color += gTexture.Sample(gSampler, input.texcoord + offset) * weight;
        }
    }

    // 合計重みで正規化
    color /= totalWeight;
    
    output = color;
    
    // 出力する
    return output;
}