#include "../../Particle.hlsli"

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
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
                float32_t s = 1.0f * (generator.Generate1d()) + 0.35f;
                
                // 青色を指定
                float32_t b = 0.65f * (generator.Generate1d()) + 0.1f;
                
                // 先に速度ベクトルをランダムに求め、生存時間設定
                gParticles[particleIndex].velocity = generator.Generate3D() / 4.0f + float32_t3(-0.125f, -0.125f, -0.125f);
                gParticles[particleIndex].lifeTime = 0.5f;
                
                // 取得
                Particle p = gParticles[particleIndex];
                
                // 速度ベクトルと生存時間をかけた値をエミッタ座標と掛け合わせる
                gParticles[particleIndex].translate = t;
                
                // 各値の初期化
                gParticles[particleIndex].scale = float32_t3(s, s, s);
                gParticles[particleIndex].color.rgb = float32_t3(0.0f, 0.15f, b);
                gParticles[particleIndex].color.a = 1.0f;
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