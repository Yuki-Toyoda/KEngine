#include "GaussianFilter.h"
#include "../../Base/DirectXCommon.h"

void GaussianFilter::Init()
{
	// かけ具合リセット
	intensity_ = 0.0f;

	// バッファ初期化
	buffer_.Init(DirectXCommon::GetInstance()->GetDirectXDevice());
}

void GaussianFilter::Update()
{
	// バッファにデータを書き込み
	*buffer_.data_ = intensity_;
}

void GaussianFilter::WriteBinding(std::ofstream* stream, IRootSignature* root, int* i)
{
	// ルートシグネチャにバインドされる番号を保持する
	bindIndex_ = *i;

	// シェーダー内の処理を記述
	std::string str = R"(
struct GaussianData {
	float32_t intensity;
};

ConstantBuffer<GaussianData> gaData : register(b${v});
    
float Gaussian(float x, float sigma)
{
    return exp(-0.5 * (x * x) / (sigma * sigma)) / (sqrt(2.0 * 3.1415926535897932384626433832795) * sigma);
}
)";

	// 変数でバインドの値を変更する
	size_t pos;
	while ((pos = str.find("${v}")) != std::string::npos) {
		str.replace(pos, 4, std::to_string(bindIndex_));
	}

	// シェーダーファイルに書き込む
	*stream << str;

	// 定数用パラメータ生成する
	root->CreateCBVParameter(bindIndex_, D3D12_SHADER_VISIBILITY_PIXEL);

	// iを加算
	*i += 1;
}

void GaussianFilter::WriteProcess(std::ofstream* stream)
{
	*stream << R"(
    uint32_t width, height;
    gTexture.GetDimensions(width, height);
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));

    if (gaData.intensity >= 1.0f)
    {
         int kernelRadius = int(ceil(2.0 * gaData.intensity));
        int kernelSize = kernelRadius * 2 + 1;

        float kernel[49];
        float totalWeight = 0.0;
        for (int i = -kernelRadius; i <= kernelRadius; ++i)
        {
            kernel[i + kernelRadius] = Gaussian(float(i), gaData.intensity);
            totalWeight += kernel[i + kernelRadius];
        }

        float4 color = float4(0, 0, 0, 0);

        for (int y = -kernelRadius; y <= kernelRadius; ++y)
        {
            for (int x = -kernelRadius; x <= kernelRadius; ++x)
            {
                float2 offset = float2(float(x), float(y)) * uvStepSize;
                float weight = kernel[x + kernelRadius] * kernel[y + kernelRadius];
                color += gTexture.Sample(gSampler, input.texcoord + offset) * weight;
        }

        color /= totalWeight;
    
        output = color;
    }
    }
)";
}

void GaussianFilter::SetToCommandList(ID3D12GraphicsCommandList6* cmdList, int* offset)
{
    // コマンドリストに定数バッファのアドレスを書き込む
    cmdList->SetGraphicsRootConstantBufferView(bindIndex_ + *offset, buffer_.GetGPUView());
}

void GaussianFilter::DisplayImGui()
{
    // ImGuiを表示
    if (ImGui::TreeNode("Gaussian")) {
        // 有効化フラグ
        ImGui::Checkbox("Enable", &use_);

        // 強度をImGuiでいじる
        ImGui::DragFloat("Intensity", &intensity_, 1.0f, 0.0f, 7.0f);

        // ツリーノード終了
        ImGui::TreePop();
    }
}
