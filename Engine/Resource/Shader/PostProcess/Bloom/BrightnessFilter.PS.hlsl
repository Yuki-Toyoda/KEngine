// VSからの出力を受け取る
struct PSInput
{
    float32_t4 position : SV_POSITION0;
    float32_t2 texcoord : TEXCOORD0;
};

// ポストプロセス用データ
struct Data
{
    float32_t thresold; // 閾値
};

// シグネチャバインド用設定
ConstantBuffer<Data>    gData       : register(b0);
Texture2D<float32_t4>   gTexture    : register(t0);
SamplerState            gSampler    : register(s0);

// 定数定義
static const float32_t epsilon = 1e-10;

// RGB入力をHCVに変換する
float32_t3 RGBtoHCV(float32_t3 RGB)
{
    // RGBの中で一番要素の大きい値を求め、明度を求める
    float32_t4 P = (RGB.g < RGB.b) ? float32_t4(RGB.bg, -1.0f, 2.0f / 3.0f) : float32_t4(RGB.gb, 0.0f, -1.0f / 3.0f);
    float32_t4 Q = (RGB.r < P.x) ? float32_t4(P.xyw, RGB.r) : float32_t4(RGB.r, P.yzx);
    
    // 明度、色相を求める
    float32_t C = Q.x - min(Q.w, Q.y);
    float32_t H = abs((Q.w - Q.y) / (6.0f * C + epsilon) + Q.z);

    // 求めた値を返還する
    return float32_t3(H, C, Q.x);
}

float32_t4 main(PSInput input) : SV_TARGET {
    // 出力用
    float32_t4 output;
    // UV座標を取得
    float32_t2 uv = input.texcoord;
    
    // テクスチャサンプリング
    output = gTexture.Sample(gSampler, uv);
    
    // HCVを取得
    float32_t3 hcv = RGBtoHCV(output.rgb);
    
    // 閾値以下の値は黒で返す
    if (hcv.z <= gData.thresold)
    {
        return float32_t4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    // 閾値以上の場合はそのまま返す
    return output;
}