#include "Object3D.hlsli"

VertexShaderOutput main(uint vertexID : SV_VertexID, uint instanceID : SV_InstanceID)
{
    // インデックスを抽出
    int v = gIndex[vertexID + (instanceID * 3)].vertex;
    int c = gIndex[vertexID + (instanceID * 3)].cameraVP;
    int w = gIndex[vertexID + (instanceID * 3)].worldMatrix;

    VertexShaderOutput output;
    output.position = mul(mul(gVertex[v].position, gWorldMatrix[w]), gCameraVP[c]);
    output.texcoord = gVertex[v].texcoord;
    output.normal = normalize(mul(gVertex[v].normal, (float3x3) gWorldMatrix[w]));
    output.color = gVertex[v].color;
    
    float4 Pos = mul(mul(gVertex[v].position, gWorldMatrix[w]), gDirectionalLight.viewProjection);
    Pos.xyz = Pos.xyz / Pos.w;

    output.id = vertexID + (instanceID * 3);

    return output;
}