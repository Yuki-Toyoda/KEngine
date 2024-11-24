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
            // 座標に速度を加算する
            gParticles[particleIndex].translate += gParticles[particleIndex].velocity;
            // 生存時間加算
            gParticles[particleIndex].currentTime += perFrame.deltaTime;
            
            // 透明度を生存時間で徐々に下げる
            float32_t alpha = 1.0f - (gParticles[particleIndex].currentTime * rcp(gParticles[particleIndex].lifeTime));
            gParticles[particleIndex].color.a = saturate(alpha);
            
            float32_t4 cameraSpaceDirection = mul(cData.ViewMatrix, float32_t4(gParticles[particleIndex].velocity, 0.0f));
            float32_t2 velocity2D = cameraSpaceDirection.xy;
                
            float32_t2 normVec = normalize(velocity2D);
            float32_t2 referenceVec = float32_t2(1.0f, 0.0f);
                
            float32_t cosTheta = dot(normVec, referenceVec);
            float32_t sinTheta = normVec.y * referenceVec.x - normVec.x * referenceVec.y;
                
            float angle = (sinTheta >= 0.0f) ? acos(cosTheta) : -acos(cosTheta);
                
            gParticles[particleIndex].rotate.z = angle;
            
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