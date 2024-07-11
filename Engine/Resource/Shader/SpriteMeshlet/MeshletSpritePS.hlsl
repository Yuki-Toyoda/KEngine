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
    
    // 計算結果を返す
    return output;
}