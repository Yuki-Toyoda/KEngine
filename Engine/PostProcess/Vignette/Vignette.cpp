#include "Vignette.h"
#include "Engine/Base/DirectXCommon.h"

void Vignette::Init()
{
	// かけ具合リセット
	intensity_ = 0.0f;

	// バッファ初期化
	buffer_.Init(DirectXCommon::GetInstance()->GetDirectXDevice());
}

void Vignette::Update()
{
	// バッファにデータを書き込み
	*buffer_.data_ = intensity_;
}

void Vignette::WriteBinding(std::ofstream* stream, IRootSignature* root, int* i)
{
	// ルートシグネチャにバインドされる番号を保持する
	bindIndex_ = *i;

	// シェーダー内の処理を記述
	std::string str = R"(
struct VignetteData {
	float32_t intensity;
};
ConstantBuffer<VignetteData> vData : register(b${v});

float32_t Vignette(float32_t2 texcoord) {
	float32_t2 center = float2(0.5f, 0.5f);
    float32_t2 delta = center - texcoord;
    float vignette = saturate(1.0f - length(delta) * vData.intensity);
	return vignette;
})";

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

void Vignette::WriteProcess(std::ofstream* stream)
{
	*stream << R"(
	float v = Vignette(input.texcoord);
	output = float32_t4(output.rgb * v, output.a);
)";
}

void Vignette::SetToCommandList(ID3D12GraphicsCommandList6* cmdList, int* offset)
{
	// コマンドリストに定数バッファのアドレスを書き込む
	cmdList->SetGraphicsRootConstantBufferView(bindIndex_ + *offset, buffer_.GetGPUView());
}

void Vignette::DisplayImGui()
{
	// ImGuiを表示
	if (ImGui::TreeNode("Vignette")) {
		// 有効化フラグ
		ImGui::Checkbox("Enable", &use_);

		// 強度をImGuiでいじる
		ImGui::DragFloat("Intensity", &intensity_, 0.01f, 0.0f, 5.0f);

		// ツリーノード終了
		ImGui::TreePop();
	}
}
