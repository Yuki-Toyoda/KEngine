#include "Meshlet.hlsli"

[NumThreads(128, 1, 1)]
[OutputTopology("triangle")]
void main(
    in uint gid  : SV_GroupID,
    in uint gtid : SV_GroupThreadID,
    out vertices VertexOutPut outVerts[128],
    out indices uint32_t3         outIndices[128]
)
{
    // Meshlet取得
    Meshlet meshlet = Meshlets[gid];

    // メッシュレット出力数を求める
    SetMeshOutputCounts(meshlet.VertCount, meshlet.PrimCount);

    if (gtid < meshlet.VertCount)
    {   
        // 頂点インデックスの取得
        uint vertexIndex = VertexIndices[meshlet.VertOffset + gtid];
        
        // 取得したインデックスから頂点座標を求める
        VertexData vertex = Vertices[vertexIndex];
        
        // 出力する頂点座標を求める
        outVerts[gtid].pos = TransformPosition(vertex.pos);
        // 出力する頂点色を求める
        outVerts[gtid].color = vertex.color;
    }
    if (gtid < meshlet.PrimCount)
    {
        // プリミティブ情報のインデックス情報を取得
        uint32_t3 packedIndices = PrimitiveIndices[meshlet.PrimOffset + gtid];
        
        // 出力するプリミティブを求める
        outIndices[gtid] = packedIndices;
    }

}