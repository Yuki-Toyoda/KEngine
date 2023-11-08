#include "Object3D.hlsli"

float4 main(VertexShaderOutput input) : SV_TARGET
{
    // インデックスを抽出
    int m = gIndex[input.id].material;
    int t = gIndex[input.id].tex2d;


    // 最終的な結果
    float4 output;

    if (gMaterial[m].enableLighting != 0)
    { // Lightingの計算を行う
        // シェーディング
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        // uvTransform
        float4 transformUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial[m].uvTransform);
        output.rgb = input.color.rgb * gTexture[t].Sample(gSampler, transformUV.xy).rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        output.w = input.color.a * gTexture[t].Sample(gSampler, transformUV.xy).a; // 透明度を保持
    }
    else
    { // Lightingの計算を行わない
        float4 transformUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial[m].uvTransform);
        output = input.color * gTexture[t].Sample(gSampler, transformUV.xy);
    }
    return output;
}