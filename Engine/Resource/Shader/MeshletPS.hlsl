#include "Meshlet.hlsli"

float4 main(VertexOutPut input) : SV_TARGET
{
    float ambientIntensity = 0.1;
    float3 lightColor = float3(1, 1, 1);
    float3 lightDir = -normalize(float3(1, -1, 1));

    float3 diffuseColor;
    if (Globals.DrawMeshlets)
    {
        uint meshletIndex = input.MeshletIndex;
        diffuseColor = float3(
            float(meshletIndex & 1),
            float(meshletIndex & 3) / 4,
            float(meshletIndex & 7) / 8);
    }
    else
    {
        diffuseColor = 0.8;
    }

    float3 finalColor = diffuseColor;

    return float4(finalColor, 1);
}