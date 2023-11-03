#include "CommandManager.h"
#include "../DescriptorHeaps/RTV.h"
#include "../DescriptorHeaps/SRV.h"
#include "../DescriptorHeaps/DSV.h"

void CommandManager::Initialize(ID3D12Device* device)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// デバイスの取得
	device_ = device;

	// コマンドキュー生成
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};										   // 設定用構造体
	result = device_->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue_)); // 生成
	assert(SUCCEEDED(result));															   // 生成確認
	// コマンドアロケーター生成
	result = device_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_)); // 生成
	assert(SUCCEEDED(result));																				    // 生成確認
	// コマンドリスト生成
	result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandAllocator_)); // 生成
	assert(SUCCEEDED(result));																													// 生成確認

	// 初期値0でフェンスの生成
	fenceVal_ = 0;																		    // フェンス値のリセット
	result = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_)); // 生成
	assert(SUCCEEDED(result));																// 生成確認

	// HLSLコードコンパイラの生成
	InitializeDXC();
	// ルートシグネチャの生成
	CreateRootSignature();
}

void CommandManager::DrawCall()
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// 描画コマンドの数だけループさせる
	for (size_t i = commands_.size() - 1; i >= 0; i--) {
		// コマンドリストの取得
		ID3D12GraphicsCommandList* cmdList = commandList_.Get();

		// 描画前処理を行う
		commands_[i]->PreDraw(cmdList);

		// コマンドリストにルートシグネチャの設定
		cmdList->SetGraphicsRootSignature(rootSignature_.Get());
		// 形状を設定する
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// PSOを取得、コマンドリストにセット
		cmdList->SetPipelineState(commands_[i]->GetPSOState());

		// シグネチャのテーブル設定
		// 0 : バッファのインデックス情報
		// 1 : 光源情報
		// 2 : 頂点データ
		// 3 : viewProjection行列
		// 4 : worldTransform
		// 5 : マテリアル情報
		// 6 : テクスチャ
		cmdList->SetGraphicsRootDescriptorTable(0, commands_[i]->indexBuffer_->view);
		cmdList->SetGraphicsRootConstantBufferView(1, lightBuffer_->view);
		cmdList->SetGraphicsRootDescriptorTable(2, vertexBuffer_->view);
		cmdList->SetGraphicsRootDescriptorTable(3, viewProjectionBuffer_->view);
		cmdList->SetGraphicsRootDescriptorTable(4, worldTransformBuffer_->view);
		cmdList->SetGraphicsRootDescriptorTable(5, materialBuffer_->view);
		cmdList->SetGraphicsRootDescriptorTable(6, textureBuffer_->view);

		// バッファ内の全3角形を描画
		cmdList->DrawInstanced(3, commands_[i]->indexBuffer_->usedCount / 3, 0, 0);
	}
}

void CommandManager::PostDraw()
{
	// 描画後処理
	commands_[0]->PostDraw(commandList_.Get());

	// GPUにコマンドリストの命令を実行するよう指示する
	ID3D12CommandList* commandLists[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandLists);

	// GPUの処理がここまでたどり着いた時にFenceに指定した値を代入するようSignalを送る
	commandQueue_->Signal(fence_.Get(), ++fenceVal_);
	// FenceのSignal値が指定した値かどうかを確認する
	if (fence_->GetCompletedValue() < fenceVal_) {
		// FenceのSignalを待つためのイベントを生成
		HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		// イベントが生成出来ているか確認
		assert(fenceEvent != nullptr);
		// 指定したSignal値にたどり着いていないため、待機するイベントを設定
		fence_->SetEventOnCompletion(fenceVal_, fenceEvent);
		// イベント終了まで待機
		WaitForSingleObject(fenceEvent, INFINITE);
		// 終了したら閉じる
		CloseHandle(fenceEvent);
	}

	// GPUとOSに画面の交換を行うように指示する
	rtv_->GetSwapChain()->Present(1, 0);
}

void CommandManager::Reset()
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// 次フレーム用にコマンドアロケーターをリセット
	result = commandAllocator_->Reset(); // リセット
	assert(SUCCEEDED(result)); // リセット確認
	// 次フレーム用にコマンドリストをリセット
	result = commandList_->Reset(commandAllocator_.Get(), nullptr); // リセット
	assert(SUCCEEDED(result)); // リセット確認

	// 全ての描画コマンドのインデックス情報をクリア
	for (int i = 0; i < commands_.size(); i++)
		commands_[i]->indexBuffer_->usedCount = 0;

	// 全てのバッファのヒープ使用数をリセットする
	vertexBuffer_->usedCount = 0;		  // 頂点バッファ
	worldTransformBuffer_->usedCount = 0; // ワールドトランスフォームバッファ
	materialBuffer_->usedCount = 0;		  // マテリアルバッファ
}

void CommandManager::SetHeaps(RTV* rtv, SRV* srv, DSV* dsv)
{
	// ヒープのポインタをセット
	rtv_ = rtv; // レンダーターゲットビュー
	srv_ = srv; // シェーダーリソースビュー
	dsv_ = dsv; // 深度ステンシルビュー

	// 描画コマンドクラスの実体を宣言
	commands_.push_back(new MainCommand()); // メイン描画コマンドを追加
	// 全ての描画コマンドの初期化
	for (int i = 0; i < commands_.size(); i++) {
		commands_[i]->SetDescriptorHeap(rtv_, srv_, dsv_); // ヒープをセット
		commands_[i]->Initialize(device_, dxc_.get(), rootSignature_.Get(), CreateBuffer(sizeof(IndexInfoStruct) * commands_[i]->kMaxIndex)); // 初期化
	}

	// ストラクチャーバッファを生成
	CreateStructuredBuffer();

	// サンプルテクスチャの読み込み
	// 後で
}

void CommandManager::SetViewProjection(Matrix4x4* vpMat)
{
	// バッファにビュープロジェクション行列をセット
	viewProjectionBuffer_->mat = vpMat;
}

void CommandManager::SetDrawData(BasePrimitive* primitive)
{
	// 最大数を超えていないかチェック
	assert(commands_[0]->indexBuffer_->usedCount < commands_[0]->kMaxIndex);
	assert(vertexBuffer_->usedCount < kMaxVertex);
	assert(worldTransformBuffer_->usedCount < kMaxWorldTransform);
	assert(materialBuffer_->usedCount < kMaxMaterial);

	// 開始のインデックス番号の取得
	uint32_t startIndexNum = vertexBuffer_->usedCount;

	// 頂点データを登録
	for (int i = 0; i < primitive->GetVertexCount(); i++) {
		vertexBuffer_->vertex[vertexBuffer_->usedCount++] = primitive->vertices_[i];
		if (primitive->commonColor != nullptr)	// 共通の色があるときはcommonColorを適応
			vertexBuffer_->vertex[vertexBuffer_->usedCount - 1].color = *primitive->commonColor;
	}
	// ワールドトランスフォームをデータに登録
	uint32_t worldMatrix = worldTransformBuffer_->usedCount;
	worldTransformBuffer_->mat[worldTransformBuffer_->usedCount++] = primitive->transform_.GetMatWorld();
	// マテリアルをデータに登録
	uint32_t material = materialBuffer_->usedCount;
	materialBuffer_->material[materialBuffer_->usedCount++] = primitive->material_;
	// テクスチャのインデックスを貰う
	uint32_t texture = defaultTexture_->GetIndex();
	if (primitive->texture_ != nullptr) {
		texture = primitive->texture_->GetIndex();
	}


	// Indexの分だけIndexInfoを求める
	for (int i = 0; i < primitive->GetIndexCount(); i++) {
		commands_[0]->indexBuffer_->indexData[commands_[0]->indexBuffer_->usedCount++] = IndexInfoStruct{
			startIndexNum + primitive->indexes_[i],
			0,
			worldMatrix,
			material,
			texture
		};
	}

}

int CommandManager::createTextureResource(const DirectX::ScratchImage& image)
{
	return 0;
}

void CommandManager::InitializeDXC()
{
}

void CommandManager::CreateRootSignature()
{
}

void CommandManager::CreateStructuredBuffer()
{
}

ID3D12Resource* CommandManager::CreateBuffer(size_t size)
{
	return nullptr;
}

ID3D12Resource* CommandManager::CreateTextureBuffer(const DirectX::TexMetadata& metaData)
{
	return nullptr;
}

void CommandManager::UploadTextureData(const DirectX::ScratchImage& mipImages)
{
}
