#include "Object3D.hlsli"

VertexShaderOutput main(uint vertexID : SV_VertexID, uint instanceID : SV_InstanceID)
{
    // それぞれのインデックスを抽出
    int v = gIndex[vertexID + (instanceID * 3)].vertex;// 頂点
    int c = gIndex[vertexID + (instanceID * 3)].cameraVP;// ビュープロジェクション行列
    int w = gIndex[vertexID + (instanceID * 3)].worldMatrix;// ワールド行列

    VertexShaderOutput output;
    output.position = mul(mul(gVertex[v].position, gWorldMatrix[w]), gCameraVP[c].mat);// 頂点座標の計算
    output.texcoord = gVertex[v].texcoord;// テクスチャ座標取得
    output.normal = normalize(mul(gVertex[v].normal, (float3x3) gWorldMatrix[w]));// 法線の計算
    output.color = gVertex[v].color;// 色の取得
    output.worldPos = mul(mul(gVertex[v].position, gWorldMatrix[w]), gCameraVP[c].worldPosition); // 頂点のワールド座標の計算
    
    output.id = vertexID + (instanceID * 3);

    return output;
}