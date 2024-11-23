#include "../../Particle.hlsli"

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    // 射出可能な状態である場合
    if (emitter.emit != false)
    {
        // ランダム番号生成用
        RandomGenerator generator;
        // シード値の生成
        generator.seed = (DTid + perFrame.time) * perFrame.time;
        
        // カウント分ループを行う
        for (uint32_t i = 0; i < emitter.count; i++)
        {
            // 取得番号確認用
            int32_t fIndex;
            // リスト内の以前のインデックスを取得
            InterlockedAdd(freeListIndex[0], -1, fIndex);
            
            // 使用可能番号が0以上かつ使用可能番号が最大パーティクル数以下の場合
            if (0 <= fIndex && fIndex < info.instanceCount)
            {
                // 使用可能リストから使用可能インデックスを取得
                int32_t particleIndex = freeList[fIndex];
                
                // スケールを指定
                float32_t s = 0.5f * (generator.Generate1d()) + 0.5f;
                
                // 青色を指定
                float32_t b = 1.0f * (generator.Generate1d()) + 0.15f;
                
                // 各値の初期化
                gParticles[particleIndex].scale       = float32_t3(s, s, s);
                gParticles[particleIndex].translate   = emitter.translate;
                gParticles[particleIndex].color.rgb   = float32_t3(0.0f, 1.0f, b);
                gParticles[particleIndex].color.a     = 1.0f;
                gParticles[particleIndex].velocity = generator.Generate3D() / 10.0f + float32_t3(-0.05f, -0.05f, -0.05f);
                gParticles[particleIndex].lifeTime    = 0.5f;
                gParticles[particleIndex].currentTime = 0.0f;
            }
            else
            {
                // インデックスの状態を元に戻す
                InterlockedAdd(freeListIndex[0], 1);
                // 生成中にパーティクルは消滅しないため、この時点でループ処理を抜ける
                break;
            }
        }
    }
}