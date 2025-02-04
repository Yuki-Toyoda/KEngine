#include "MeshletSprite.hlsli"

uint32_t3 UnpackPrimitive(uint primitive)
{
    // 10ビットごとにプリミティブのインデックスを読み取る
    return uint32_t3(primitive & 0x3FF, (primitive >> 10) & 0x3FF, (primitive >> 20) & 0x3FF);
}

uint32_t3 GetPrimitive(Meshlet m, uint index)
{
    // 面情報取得
    return UnpackPrimitive(PrimitiveIndices[m.PrimOffset + index]);
}

uint32_t GetVertexIndex(Meshlet m, uint localIndex)
{
    // 頂点のインデックスを求める
    localIndex = m.VertOffset + localIndex;

    // 4バイトずつで読み込む
    return UniqueVertexIndices.Load(localIndex * 4);
}

[NumThreads(128, 1, 1)]      // スレッド数最大128
[OutputTopology("triangle")] // 出力形状は三角形
void main(
    in uint32_t gid  : SV_GroupID,
    in uint32_t gtid : SV_GroupThreadID,
    out vertices VertexOutPut outVerts[128],
    out indices uint32_t3     outIndices[128]
)
{
    // Meshlet取得
    Meshlet meshlet = Meshlets[gid];

    // メッシュレット出力数を求める
    SetMeshOutputCounts(meshlet.VertCount, meshlet.PrimCount);

    if (gtid < meshlet.VertCount)
    {   
        // 頂点インデックスの取得
        uint32_t vertexIndex = GetVertexIndex(meshlet, gtid);
        
        // 取得したインデックスから頂点座標を求める
        VertexData vertex = Vertices[vertexIndex];
        
        // 出力する頂点座標を求める
        outVerts[gtid].pos      = TransformPosition(vertex.pos);
        outVerts[gtid].texCoord = vertex.texCoord;
        outVerts[gtid].normal   = normalize(vertex.normal);
        outVerts[gtid].mIndex   = vertex.materialIndex;
        
        // 出力する頂点色を求める
        if (ConstantData.DrawMeshlets == true)
        {
           // 出力する頂点色を求める
            outVerts[gtid].color = float4(
            float(gid & 1),
            float(gid & 3) / 4,
            float(gid & 7) / 8,
            material.color.a);
        }
        else
        {
            // 頂点色はマテリアルから取得
            outVerts[gtid].color = material.color;
        }
    }
    if (gtid < meshlet.PrimCount)
    {
        // プリミティブ情報のインデックス情報を取得
        uint32_t3 packedIndices = GetPrimitive(meshlet, gtid);
        
        // 出力するプリミティブを求める
        outIndices[gtid] = packedIndices;
    }

}