#include "GaussianBlur.hlsli"

float32_t4 main(PSInput input) : SV_TARGET0
{
    // ウェイト
    float32_t weight = 0.0f;
    // カーネル
    float32_t kernel[kIndexSize];
    // BoxFilter処理の変数格納用
    uint32_t width, height;
    // テクスチャ座標を取得
    gTexture.GetDimensions(width, height);
    // UVサイズを算出する
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
    
    // 出力色保存用
    float32_t4 output = float32_t4(0.0f, 0.0f, 0.0f, 1.0f);
    // BoxFilter処理
    for (int32_t y = 0; y < kIndexSize; y++)
    {
        // カーネルインデックスの取得
        float32_t index = GetIndex(y);
        // ガウス処理をかける
        kernel[y] = Gauss(0.0f, index, kStandardDeviation);
        // ウェイトを加算
        weight += kernel[y];
        
        // 現在のuv座標取得
        float32_t2 uv = input.texcoord;
        // y方向のuv取得
        uv.y = input.texcoord.y + index * uvStepSize.y;
        // 色に1/9掛けて足す
        float32_t3 fetchColor = gTexture.Sample(gSampler, uv).rgb;
        // 出力色に加算する
        output.rgb += fetchColor * kernel[y];
    }
    // 畳み込み後の正規化する
    output.rgb *= rcp(weight);
    
    // 出力
    return output;
}