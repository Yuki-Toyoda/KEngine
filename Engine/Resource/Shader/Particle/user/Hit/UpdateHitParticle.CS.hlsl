#include "../../Particle.hlsli"

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    // 現在処理中のスレッド番号を取得
    uint32_t particleIndex = DTid.x;
    
    // パーティクル番号が最大パーティクル数を超えていなければ
    if (particleIndex < info.instanceCount)
    {   
        // パーティクルの現在時間が生存秒数を超過していないとき、以下の処理を行う
        if (gParticles[particleIndex].currentTime < gParticles[particleIndex].lifeTime)
        {
            float scale = Lerp(0.0f, gParticles[particleIndex].velocity.x, (gParticles[particleIndex].currentTime / gParticles[particleIndex].lifeTime));
            gParticles[particleIndex].scale = float32_t3(scale, scale, 1.0f);
            
            // 生存時間加算
            gParticles[particleIndex].currentTime += (perFrame.deltaTime * perFrame.timeScale);
            
            // 透明度を生存時間で徐々に下げる
            if (gParticles[particleIndex].currentTime > gParticles[particleIndex].lifeTime / 2.0f)
            {
                float32_t alpha = Lerp(1.0f, 0.0f, (gParticles[particleIndex].currentTime / 2.0f) / (gParticles[particleIndex].lifeTime / 2.0f));
                gParticles[particleIndex].color.a = alpha;
            }
        }
        else // それ以外の場合
        {
            // パーティクルのスケールを0にする
            gParticles[particleIndex].scale = float32_t3(0.0f, 0.0f, 0.0f);
            
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