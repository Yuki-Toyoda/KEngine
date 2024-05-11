#include "MeshletSkin.hlsli"

float32_t4 main(VertexOutPut input) : SV_TARGET
{
    // 最終的な出力結果
    float32_t4 output;
    
    // テクスチャのサンプリングを行う
    float32_t4 textureColor = gTexture.Sample(gSampler, input.texCoord);
    
    // テクスチャ色の透明度が0.5以下、マテリアル透明度が0であればそのピクセルを描画しない
    if (textureColor.a <= 0.5f || input.color.a <= 0.0f) { discard; }
    
    // マテリアルでライティングが有効化されている場合
    if (material.enableLighting)
    {
        // ライティングの計算を行う
        float32_t NdotL = dot(input.normal, -normalize(light.direction));
        float32_t cos   = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        // 出力する色の要素を求める
        output.rgb = input.color.rgb * textureColor.rgb * light.color.rgb * cos * light.intensity; // RGB要素のみ
        output.w   = input.color.a * textureColor.a;                                               // 透明度
    }
    else
    {
        // ライティング計算を行わずにピクセル色を求める
        output = input.color * textureColor;
    }
    
    // 計算結果を返す
    return output;
}