#include "CommandManager.h"
#include "../DescriptorHeaps/RTV.h"
#include "../DescriptorHeaps/SRV.h"
#include "../DescriptorHeaps/DSV.h"
#include "../../Resource/Texture/TextureManager.h"
#include "../WinApp.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

void CommandManager::Init(ID3D12Device2* device)
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
	result = device_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_.Get(), nullptr, IID_PPV_ARGS(&commandList_)); // 生成
	assert(SUCCEEDED(result));																													// 生成確認

	// 初期値0でフェンスの生成
	fenceVal_ = 0;																		    // フェンス値のリセット
	result = device_->CreateFence(fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_)); // 生成
	assert(SUCCEEDED(result));																// 生成確認

	// HLSLコードコンパイラの生成
	InitializeDXC();
	
}

void CommandManager::DrawCall()
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = commandList_.Get();

	// 
	for (int i = 0; i < (int)commands_.size(); i++) {
		// 描画前処理を行う
		commands_[i]->PreDraw(cmdList);

		// コマンドリストにルートシグネチャの設定
		cmdList->SetGraphicsRootSignature(rootSignature_.Get());
		// 形状を設定する
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		// PSOを取得、コマンドリストにセット
		cmdList->SetPipelineState(commands_[i]->GetPSOState());

		// ルートシグネチャにバッファをセットする
		cmdList->SetGraphicsRootConstantBufferView(0, generalCBuffer_->View);

		// メッシュシェーダーを実行
		commandList_->DispatchMesh(0, 1, 1);

		// メイン描画コマンドの場合処理を一旦抜ける
		if (i == (int)commands_.size() - 1) {
			break;
		}

		// 描画後処理
		commands_[i]->PostDraw(commandList_.Get());

		// GPUにコマンドリストの命令を実行するよう指示する
		ID3D12CommandList* commandLists[] = { commandList_.Get() };
		commandQueue_->ExecuteCommandLists(1, commandLists);

		// GPUの処理がここまでたどり着いた時にFenceに指定した値を代入するようSignalを送る
		commandQueue_->Signal(fence_.Get(), ++fenceVal_);
		// FenceのSignal値が指定した値かどうかを確認する
		if (fence_->GetCompletedValue() != fenceVal_) {
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
		result;
		// 次のコマンドリストを準備
		result = commandAllocator_->Reset();
		assert(SUCCEEDED(result));
		result = commandList_->Reset(commandAllocator_.Get(), nullptr);
		assert(SUCCEEDED(result));
	}
}

void CommandManager::PostDraw()
{
	// 描画後処理
	commands_[(int)commands_.size() - 1]->PostDraw(commandList_.Get());
	// GPUにコマンドリストの命令を実行するよう指示する
	ID3D12CommandList* commandLists[] = { commandList_.Get() };
	commandQueue_->ExecuteCommandLists(1, commandLists);

	// GPUの処理がここまでたどり着いた時にFenceに指定した値を代入するようSignalを送る
	commandQueue_->Signal(fence_.Get(), ++fenceVal_);
	// FenceのSignal値が指定した値かどうかを確認する
	if (fence_->GetCompletedValue() != fenceVal_) {
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
		commands_[i]->SetDescriptorHeap(rtv_, srv_, dsv_);			    // ヒープをセット
		commands_[i]->Init(device_, dxc_.get(), rootSignature_.Get()); // 初期化
	}

	// ルートシグネチャの生成
	CreateRootSignature();

	// バッファを生成
	CreateBuffers();

	// サンプルテクスチャの読み込み
	//defaultTexture_ = TextureManager::Load("white2x2.png");
}

Matrix4x4* const CommandManager::GetWorldMatrixAddress() const
{
	// 書き込み用のアドレスを返す
	return &generalCBuffer_->Data->World;
}

Matrix4x4* const CommandManager::GetViewMatrixAddress() const
{
	// 書き込み用のアドレスを返す
	return &generalCBuffer_->Data->WorldView;
}

Matrix4x4* const CommandManager::GetViewProjectionAddress() const
{
	// 書き込み用のアドレスを返す
	return &generalCBuffer_->Data->WorldViewProj;
}

void CommandManager::SetDrawData()
{
	
}

int CommandManager::createTextureResource(const DirectX::ScratchImage& image)
{
	// バッファにテクスチャのメタデータを追加
	textureBuffer_->Resource.push_back(CreateTextureBuffer(image.GetMetadata()));
	// テクスチャをアップロード
	UploadTextureData(image);
	// テクスチャバッファの番号を返す
	return textureBuffer_->UsedCount++;
}

void CommandManager::DisplayImGui()
{
	
}

void CommandManager::InitializeDXC()
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// DXCのインスタンス生成
	dxc_ = std::make_unique<DXC>();

	// DXCUtilsを初期化
	result = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxc_->dxcUtils_)); // 初期化
	assert(SUCCEEDED(result));													// 初期化確認
	// DXCCompilerを初期化
	result = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxc_->dxcCompiler_)); // 初期化
	assert(SUCCEEDED(result));													      // 初期化確認
	// 現在は使用しないがIncludeに対応させるための設定を行う
	result = dxc_->dxcUtils_->CreateDefaultIncludeHandler(&dxc_->dxcIncludeHandler_); // 設定を行う
	assert(SUCCEEDED(result));														  // 設定確認

}

void CommandManager::CreateRootSignature()
{
	// 結果確認用
	HRESULT result = S_FALSE;

	//シェーダーバイナリからRootSignatureの部分をフェッチ
	result = D3DGetBlobPart(commands_[0]->GetShaderBlob()->GetBufferPointer(), commands_[0]->GetShaderBlob()->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &signatureBlob_);
	// フェッチ出来たか確認
	assert(SUCCEEDED(result));

	// 取得したバイナリを元にルートシグネチャを生成
	result = device_->CreateRootSignature(0, signatureBlob_->GetBufferPointer(), signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_)); // 生成
	assert(SUCCEEDED(result));																													  // 生成確認
}

void CommandManager::CreateBuffers()
{
	// 汎用データバッファ生成
	generalCBuffer_ = std::make_unique<GeneralCBuffer>();										  // バッファの生成
	generalCBuffer_->Resource = CreateBuffer(sizeof(GeneralData));								  // バッファの生成
	generalCBuffer_->Resource->Map(0, nullptr, reinterpret_cast<void**>(&generalCBuffer_->Data)); // 生成したバッファのマッピングを行う
	generalCBuffer_->View = generalCBuffer_->Resource->GetGPUVirtualAddress();					  // GPU上のアドレスの取得

	// テクスチャデータ
	textureBuffer_ = std::make_unique<TextureBuffer>();

}

ID3D12Resource* CommandManager::CreateBuffer(size_t size)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// 返すリソース
	ID3D12Resource* Resource;

	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;	// UploadHeapを使う
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = size; // リソースのサイズ
	// バッファの場合はこれらは1にする決まり
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際に頂点リソースを作る
	result = device_->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&Resource));
	assert(SUCCEEDED(result));

	// 生成したバッファリソースを返す
	return Resource;
}

ID3D12Resource* CommandManager::CreateTextureBuffer(const DirectX::TexMetadata& metaData)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// 1. metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metaData.width);
	resourceDesc.Height = UINT(metaData.height);
	resourceDesc.MipLevels = UINT16(metaData.mipLevels);
	resourceDesc.DepthOrArraySize = UINT16(metaData.arraySize);
	resourceDesc.Format = metaData.format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metaData.dimension);

	// 2. 利用するHeapの設定。非常に特殊な運用。
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM; // 細かい設定を行う
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // WriteBackポリシーでCPUアクセス可能
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0; // プロセッサの近くに配置

	// 3. Resourceを生成する
	ID3D12Resource* Resource = nullptr;
	result = device_->CreateCommittedResource(
		&heapProperties,					// Heapの設定
		D3D12_HEAP_FLAG_NONE,				// Heapの特殊な設定。特になし。
		&resourceDesc,						// Resourceの設定
		D3D12_RESOURCE_STATE_GENERIC_READ,	// 初回のResourceState。Textureは基本読むだけ
		nullptr,							// Clear最適地。使わないでnullptr
		IID_PPV_ARGS(&Resource)				// 作成するResourceポインタへのポインタ
	);
	assert(SUCCEEDED(result));
	return Resource;
}

void CommandManager::UploadTextureData(const DirectX::ScratchImage& mipImages)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// Meta情報を取得
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	// 全MipMapについて
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel) {
		// MipMapLevelを指定して各Imageを取得
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		// Textureに転送
		result = textureBuffer_->Resource[textureBuffer_->UsedCount]->WriteToSubresource(
			UINT(mipLevel),
			nullptr,
			img->pixels,
			UINT(img->rowPitch),
			UINT(img->slicePitch)
		);
		assert(SUCCEEDED(result));
	}

	// metaDataを元にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	// SRVを作成するDescriptorHeapの場所を決める（ImGuiとStructuredBufferたちが先頭を使っているので + ）
	D3D12_CPU_DESCRIPTOR_HANDLE textureSRVHandleCPU = srv_->GetCPUHandle(textureBuffer_->UsedCount + srv_->GetUsedCount());
	// 初めてのテクスチャ生成ならviewを保存
	if (textureBuffer_->UsedCount == 0) {
		textureBuffer_->View = srv_->GetGPUHandle(textureBuffer_->UsedCount + srv_->GetUsedCount());
	}
	// SRVの生成
	device_->CreateShaderResourceView(textureBuffer_->Resource[textureBuffer_->UsedCount].Get(), &srvDesc, textureSRVHandleCPU);
}
