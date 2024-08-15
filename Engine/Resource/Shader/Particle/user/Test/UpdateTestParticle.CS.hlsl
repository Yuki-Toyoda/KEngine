#include "../../Particle.hlsli"

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    // 現在処理中のスレッド番号を取得
    uint32_t particleIndex = DTid.x;
    
    // パーティクル番号が最大パーティクル数を超えていなければ
    if (particleIndex < info.instanceCount)
    {
        Particle p = gParticles[particleIndex];
        
        // パーティクルの現在時間が生存秒数を超過していないとき、以下の処理を行う
        if (p.currentTime < p.lifeTime)
        {
            // 座標に速度を加算する
            p.translate += p.velocity;
            // 生存時間加算
            p.currentTime += perFrame.deltaTime;
            
            // 透明度を生存時間で徐々に下げる
            float32_t alpha = 1.0f - (p.currentTime * rcp(p.lifeTime));
            p.color.a = saturate(alpha);
            
        }
        else // それ以外の場合
        {
            // パーティクルのスケールを0にする
            p.scale = float32_t3(0.0f, 0.0f, 0.0f);
            
            // 使用可能番号確認用
            int32_t fIndex;
            // 生存時間を超過している時点で使用可能状態にする
            InterlockedAdd(freeListIndex[0], 1, fIndex);
            
            // 取得した番号が最大パーティクル数を超過していなければ
            if ((fIndex + 1) < info.instanceCount)
            {
                // リスト内の該当番号を使用可能とする
                freeList[fIndex + 1] = particleIndex;
            }
            else
            {
                // ここまで来た時点でお前は何かを間違えている
                InterlockedAdd(freeListIndex[0], -1, fIndex);
            }

        }
    }
}