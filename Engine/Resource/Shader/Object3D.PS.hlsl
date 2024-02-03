#include "Object3D.hlsli"

float4 main(VertexShaderOutput input) : SV_TARGET
{
    int m = gIndex[input.id].material;
    int t = gIndex[input.id].tex2d;


    float4 output;
    float4 transformUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial[m].uvTransform);
    float4 textureColor = gTexture[t].Sample(gSampler, transformUV.xy);

    if (textureColor.a <= 0.5f)
        discard;
    
    if (gMaterial[m].enableLighting != 0)
    {
        
        float NdotL = dot(normalize(input.normal), -normalize(gDirectionalLight.direction));
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        
        if (gMaterial[m].enableReflection != 0){
            
            float3 toEye = normalize(input.cameraWorldPos - input.worldPos);
            
            float3 reflectLight = reflect(normalize(gDirectionalLight.direction), normalize(input.normal));
            
            float rDotE = dot(reflectLight, toEye);
            float specularPow = pow(saturate(rDotE), gMaterial[m].shininess);

            float3 diffuse =
            input.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
            
            float3 specular = 
            gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float3(1.0f, 1.0f, 1.0f);

            output.rgb = diffuse + specular;
            output.w = input.color.a * gTexture[t].Sample(gSampler, transformUV.xy).a;
        }
        else
        {
            output.rgb = input.color.rgb * gTexture[t].Sample(gSampler, transformUV.xy).rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
            output.w = input.color.a * textureColor.a;
        }
        
    }
    else
    {
        output = input.color * gTexture[t].Sample(gSampler, transformUV.xy);
    }
    return output;
}