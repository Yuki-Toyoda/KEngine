#include "MeshletToon.hlsli"

float32_t4 main(VertexOutPut input) : SV_TARGET
{
    // 最終的な出力結果
    float32_t4 output;
    
    // テクスチャのサンプリングを行う
    float32_t4 textureColor = gTexture[materials[input.mIndex].textureIndex].Sample(gSampler, input.texCoord);
    
    // テクスチャ色の透明度が0.5以下、マテリアル透明度が0であればそのピクセルを描画しない
    if (textureColor.a <= 0.5f || input.color.a <= 0.0f) { discard; }
    
    // マテリアルでライティングが有効化されている場合
    if (materials[input.mIndex].enableLighting)
    {
        // ライティングの計算を行う
        float32_t NdotL = dot(input.normal, -normalize(light.direction));
        float32_t cos   = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        // トゥーンマップから影の色を取得
        float32_t4 toonMapColor = gTexture[ConstantData.ToonMapIndex].Sample(toonMapSampler, float32_t2(cos, 0.0f));
        
        // 出力する色の要素を求める
        output.rgb = input.color.rgb * textureColor.rgb * light.color.rgb * toonMapColor.rgb * light.intensity; // RGB要素のみ
        output.w   = input.color.a * textureColor.a;                                                            // 透明度
    }
    else
    {
        // ライティング計算を行わずにピクセル色を求める
        output = input.color * textureColor;
    }
    
    // 環境マップの適用
    if (materials[input.mIndex].environmentCoefficient_ > 0.0f)
    {
        float32_t3 cameraToPosition = normalize(input.world - ConstantData.WorldPosition);
        float32_t3 reflectVector = reflect(cameraToPosition, normalize(input.normal));
        float32_t4 environmentColor = gEnvironmentMap.Sample(gSampler, reflectVector);
        environmentColor.rgb = environmentColor.rgb * materials[input.mIndex].environmentCoefficient_;
        
        output.rgb += environmentColor.rgb;
    }
    
    // Dissolve強度が0.0以上の場合
    if (materials[input.mIndex].dissolveStrength > 0.0f)
    {
        // マスク用のテクスチャをサンプリングする
        float32_t mask = gMaskTexture.Sample(gSampler, input.texCoord);
        // マスクの値が強度を超過している場合ピクセルを棄却する
        if (mask <= materials[input.mIndex].dissolveStrength)
        {
            discard;
        }
        
        // エッジの処理を行う
        float32_t edge = 1.0f - smoothstep(materials[input.mIndex].dissolveStrength, materials[input.mIndex].dissolveStrength + materials[input.mIndex].dissolveEdgeThreshold, mask);
        output.rgb += edge * materials[input.mIndex].dissolveEdgeColor;
    }
    
    // 計算結果を返す
    return output;
}