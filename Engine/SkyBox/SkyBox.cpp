#include "SkyBox.h"
#include "Engine/Debug/Debug.h"
#include "Engine/Resource/Texture/TextureManager.h"
#include "Engine/Base/DirectXCommon.h"

void SkyBox::Init(const std::string& ddsPath, const std::string& ddsName)
{
	// DDSファイル以外を読み込もうとしている場合停止させる
	if (!ddsName.ends_with(".dds")) {
		// ログでDDSファイル以外であることを通知
		Debug::Log("This file is not .dds");
		// 停止
		assert(false);
	}

	// トランスフォームの初期化
	transform_.Init();

	// テクスチャ読み込み
	texture_ = TextureManager::Load(ddsPath, ddsName);

	// バッファ生成のためにデバイスとSRVを取得する
	DirectXDevice* device = DirectXCommon::GetInstance()->GetDirectXDevice();
	SRV* srv = DirectXCommon::GetInstance()->GetSRV();

	// バッファの初期化
	transformBuffer_ = std::make_unique<ConstantBuffer<Matrix4x4>>();
	transformBuffer_->Init(device);
	vertexBuffer_	 = std::make_unique<StructuredBuffer<Vector4>>(8);
	vertexBuffer_->Init(device,srv);
	indexBuffer_	 = std::make_unique<StructuredBuffer<int32_t>>(36);
	indexBuffer_->Init(device, srv);

	/// 頂点座標を初期化する
	// 右面
	vertexBuffer_->data_[0] = {100.0f, 100.0f, 100.0f, 1.0f};
	vertexBuffer_->data_[1] = { 100.0f, 100.0f, -100.0f, 1.0f};
	vertexBuffer_->data_[2] = { 100.0f, -100.0f, 100.0f, 1.0f};
	vertexBuffer_->data_[3] = { 100.0f, -100.0f, -100.0f, 1.0f};
	// 左面
	vertexBuffer_->data_[4] = {-100.0f, 100.0f, -100.0f, 1.0f};
	vertexBuffer_->data_[5] = {-100.0f, 100.0f, 100.0f, 1.0f};
	vertexBuffer_->data_[6] = {-100.0f, -100.0f, -100.0f, 1.0f};
	vertexBuffer_->data_[7] = {-100.0f, -100.0f, 100.0f, 1.0f};

	/// インデックス情報を初期化する
	// 右面
	indexBuffer_->data_[0] = 0;
	indexBuffer_->data_[1] = 1;
	indexBuffer_->data_[2] = 2;
	indexBuffer_->data_[3] = 2;
	indexBuffer_->data_[4] = 1;
	indexBuffer_->data_[5] = 3;

	// 左面
	indexBuffer_->data_[6]	= 4;
	indexBuffer_->data_[7]	= 5;
	indexBuffer_->data_[8]	= 6;
	indexBuffer_->data_[9]	= 6;
	indexBuffer_->data_[10] = 5;
	indexBuffer_->data_[11] = 7;

	// 前面
	indexBuffer_->data_[12] = 1;
	indexBuffer_->data_[13] = 4;
	indexBuffer_->data_[14] = 3;
	indexBuffer_->data_[15] = 3;
	indexBuffer_->data_[16] = 4;
	indexBuffer_->data_[17] = 6;

	// 後面
	indexBuffer_->data_[18] = 2;
	indexBuffer_->data_[19] = 7;
	indexBuffer_->data_[20] = 0;
	indexBuffer_->data_[21] = 0;
	indexBuffer_->data_[22] = 7;
	indexBuffer_->data_[23] = 5;

	// 上面
	indexBuffer_->data_[24] = 0;
	indexBuffer_->data_[25] = 5;
	indexBuffer_->data_[26] = 1;
	indexBuffer_->data_[27] = 1;
	indexBuffer_->data_[28] = 5;
	indexBuffer_->data_[29] = 4;

	// 下面
	indexBuffer_->data_[30] = 3;
	indexBuffer_->data_[31] = 6;
	indexBuffer_->data_[32] = 2;
	indexBuffer_->data_[33] = 2;
	indexBuffer_->data_[34] = 6;
	indexBuffer_->data_[35] = 7;

}

void SkyBox::Draw(ID3D12GraphicsCommandList6* cmdList)
{
	if (texture_.GetView() != -1) {
		// トランスフォームデータのコピー
		*transformBuffer_->data_ = transform_.GetMatWorld();

		// 描画用バッファをコマンドリストにセットする
		cmdList->SetGraphicsRootConstantBufferView(1, transformBuffer_->GetGPUView()); // トランスフォーム
		cmdList->SetGraphicsRootDescriptorTable(2, vertexBuffer_->GetGPUView());	   // 頂点情報
		cmdList->SetGraphicsRootDescriptorTable(3, indexBuffer_->GetGPUView());		   // インデックス情報
		cmdList->SetGraphicsRootDescriptorTable(4, texture_.GetAddress());			   // テクスチャ情報

		// 描画
		cmdList->DrawInstanced(36, 1, 0, 0);
	}
}

void SkyBox::DisplayImGui(const std::string& id)
{
	// ツリーノード開始
	if (ImGui::TreeNode(id.c_str())) {
		// トランスフォームのImGuiを表示
		transform_.DisplayImGui();

		// ツリーノード終了
		ImGui::TreePop();
	}
}
