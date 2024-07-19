#include "OutLine.h"
#include "../../Base/DirectXCommon.h"

void OutLine::Init()
{
	// 各種変数のリセット
	use_	   = true;
	threshold_ = 0.5f;
	color_	   = { 0.0f, 0.0f, 0.0f, 1.0f };

	// バッファの初期化を行う
	buffer_.Init(DirectXCommon::GetInstance()->GetDirectXDevice());
}

void OutLine::Update()
{
	// バッファ内のデータを更新する
	buffer_.data_->color	 = color_;
	buffer_.data_->threshold = threshold_;
}

void OutLine::WriteBinding(std::ofstream* stream, IRootSignature* root, int* i)
{
	// ルートシグネチャにバインドされる番号を保持する
	bindIndex_ = *i;

	// シェーダー内の処理を書き込む
	std::string str = R"(
struct OutLineData {
	float32_t4x4 projectionInverse;
	float32_t4 color;
	float32_t threshold;
};
ConstantBuffer<OutLineData> olData : register(b${v});

static const float32_t2 kIndex3x3[3][3] = {
	{{-1.0f,-1.0f},{ 0.0f,-1.0f},{ 1.0f,-1.0f}},
	{{-1.0f, 0.0f},{ 0.0f, 0.0f},{ 1.0f, 0.0f}},
	{{-1.0f, 1.0f},{ 0.0f, 1.0f},{ 1.0f, 1.0f}},
};
static const float32_t kPrewittHorizontalKernel[3][3] = {
	{ -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
	{ -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
	{ -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
};
static const float32_t kPrewittVerticalKernel[3][3] = {
	{ -1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f },
};

float32_t3 OutLine(float32_t2 uv, float32_t3 color) {
	float32_t2 difference = float32_t2(0.0f, 0.0f);
	uint32_t width, height;
	gTexture.GetDimensions(width, height);
	float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));

	for(int32_t x = 0; x < 3; x++) {
		for(int32_t y = 0; y < 3; y++) {
			float32_t2 texcoord = uv + kIndex3x3[x][y] * uvStepSize;
			float32_t ndcDepth = gDepth.Sample(gPointSampler, texcoord).r;

			float32_t4 viewSpace = mul(float32_t4(0.0f,0.0f,ndcDepth,1.0f), olData.projectionInverse);
			float32_t viewZ = viewSpace.z * rcp(viewSpace.w);
			difference.x += viewZ * kPrewittHorizontalKernel[x][y];
			difference.y += viewZ * kPrewittVerticalKernel[x][y];
		}
	}

	float32_t weight = length(difference);
	weight = saturate(weight);

	if (olData.threshold >= weight) {
		return color;
	}
	return lerp(color, olData.color.rgb, weight);

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

void OutLine::WriteProcess(std::ofstream* stream)
{
	// 
	*stream << R"(
	output.rgb = OutLine(uv, output.rgb);
)";
}

void OutLine::SetToCommandList(ID3D12GraphicsCommandList6* cmdList, int* offset)
{
	// コマンドリストに定数バッファのアドレスを書き込む
	cmdList->SetGraphicsRootConstantBufferView(bindIndex_ + *offset, buffer_.GetGPUView());
}

void OutLine::DisplayImGui()
{
	// ImGuiを表示
	if (ImGui::TreeNode("OutLine")) {
		// 有効化フラグ
		ImGui::Checkbox("Use", &use_);
		// しきい値
		ImGui::DragFloat("Threshold", &threshold_, 0.01f);
		// アウトライン色
		ImGui::ColorPicker4("color", &color_.x);
		ImGui::TreePop();
	}
}
