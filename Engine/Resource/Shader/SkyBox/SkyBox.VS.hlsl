#include "SkyBox.hlsli"

ConstantBuffer<ConstantData> constantData : register(b0);
ConstantBuffer<WorldTransform> worldMat   : register(b1);
StructuredBuffer<float32_t4> vertices     : register(t0);
StructuredBuffer<int32_t>    indices      : register(t1);

VertexShaderOutput main(uint32_t vertexID : SV_VertexID)
{
    // 出力用
    VertexShaderOutput output;

    // 頂点情報配列からインデックスを用いて頂点座標を取得
    float32_t4 vertex = vertices[indices[vertexID]];
    
    // 出力座標を求める
    output.position = mul(mul(vertex, worldMat.world), constantData.WorldViewProj).xyww;
    // テクスチャ座標を求める
    output.texcoord = vertex.xyz;
    
    // 求めた情報を出力する
    return output;
}