#include "Trail.hlsli"

VertexOutPut main(uint vertexID : SV_VertexID)
{
    // 出力用
    VertexOutPut output;
    
	 // 頂点情報配列からインデックスを用いて頂点座標を取得
    VertexData vertex = Vertices[Indices[vertexID]];

    // 出力座標を求める
    output.pos = mul(vertex.pos, cData.WorldViewProj);
    output.texCoord = vertex.texCoord;
    
    // 求めた情報の出力
    return output;
}