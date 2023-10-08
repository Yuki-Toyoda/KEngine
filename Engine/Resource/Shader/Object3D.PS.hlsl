#include "Object3D.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

// 
PixelShaderOutput main(VertexShaderOutput input)
{
    
    // 
    PixelShaderOutput output;
    
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    // テクスチャの透明度が0.5以下ならピクセルを棄却
    if (textureColor.a <= 0.5f)
        discard;
    
    // 
    for (int i = 0; i < kDirectionalLight; i++)
    {
        if (directionalLight[i].active)
        {
            if (enableLighting)
            {
                float NdotL = dot(normalize(input.normal), -normalize(directionalLight[i].direciton));
                float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
                output.color.rgb = color.rgb * textureColor.rgb * directionalLight[i].color.rgb * cos * directionalLight[i].intensity;
                output.color.a = color.a * textureColor.a;
            }
            else
            {
                 // 
                output.color = color * textureColor;
            }
        }
        else
        {
         // 
            output.color = color * textureColor;
        }
    }
    
    // 
    return output;
    
}