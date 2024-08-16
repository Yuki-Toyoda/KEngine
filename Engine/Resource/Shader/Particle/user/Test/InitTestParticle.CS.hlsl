#include "../../Particle.hlsli"

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    // インデックス取得
    uint32_t particleIndex = DTid.x;
    
    // 最初のインデックスだった場合にのみインデックスの初期化を行う
    if (particleIndex == 0)
    {
        // 末尾の番号を指すように変更
        freeListIndex[0] = info.instanceCount - 1;
    }
    
    // パーティクルの最大数を超過していなければ
    if (particleIndex < info.instanceCount)
    {
        // パーティクルの全要素を0で初期化する
        gParticles[particleIndex] = (Particle)0;
        // 使用可能な番号リストの初期化
        freeList[particleIndex] = particleIndex;
    }
}