#include "MeshletSprite.hlsli"

float32_t4 main(VertexOutPut input) : SV_TARGET
{
    // 最終的な出力結果
    float32_t4 output;
    
    // テクスチャのサンプリングを行う
    float32_t4 textureColor = gTexture[material.textureIndex].Sample(gSampler, input.texCoord);
    
    // テクスチャ色の透明度が0.5以下、マテリアル透明度が0であればそのピクセルを描画しない
    if (textureColor.a <= 0.5f || input.color.a <= 0.0f) { discard; }
    
   // ライティング計算を行わずにピクセル色を求める
    output = input.color * textureColor;
    
     // Dissolve強度が0.0以上の場合
    if (material.dissolveStrength > 0.0f)
    {
        // マスク用のテクスチャをサンプリングする
        float32_t mask = gMaskTexture.Sample(gSampler, input.texCoord);
        // マスクの値が強度を超過している場合ピクセルを棄却する
        if (mask <= material.dissolveStrength)
        {
            discard;
        }
        
        // エッジの処理を行う
        float32_t edge = 1.0f - smoothstep(material.dissolveStrength, material.dissolveStrength + material.dissolveEdgeThreshold, mask);
        output.rgb += edge * material.dissolveEdgeColor;
    }
    
    // 計算結果を返す
    return output;
}