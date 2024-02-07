#include "Object3D.hlsli"

VertexShaderOutput main(uint vertexID : SV_VertexID, uint instanceID : SV_InstanceID)
{
    int v = gIndex[vertexID + (instanceID * 3)].vertex;
    int c = gIndex[vertexID + (instanceID * 3)].cameraVP;
    int w = gIndex[vertexID + (instanceID * 3)].worldMatrix;

    VertexShaderOutput output;
    output.position = mul(mul(gVertex[v].position, gWorldMatrix[w]), gCameraVP[c]);
    output.texcoord = gVertex[v].texcoord;
    output.normal = normalize(mul(gVertex[v].normal, (float3x3) gWorldMatrix[w]));
    output.color = gVertex[v].color;

    output.id = vertexID + (instanceID * 3);

    return output;
}