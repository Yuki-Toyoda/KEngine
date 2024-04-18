#include "CommandManager.h"
#include "../DescriptorHeaps/RTV.h"
#include "../DescriptorHeaps/SRV.h"
#include "../DescriptorHeaps/DSV.h"
#include "../../Resource/Texture/TextureManager.h"
#include "../WinApp.h"
#include <d3dcompiler.h>

#include "../../Primitive/3d/Mesh.h"

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

	// ルートシグネチャの生成
	CreateRootSignature();
}

void CommandManager::DrawCall()
{
	// 結果確認用
	//HRESULT result = S_FALSE;

	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = commandList_.Get();

	// 
	for (int i = 0; i < (int)commands_.size(); i++) {
		// 描画前処理を行う
		commands_[i]->PreDraw(cmdList);

		// コマンドリストにルートシグネチャの設定
		cmdList->SetGraphicsRootSignature(rtsManager_->GetRootSignature(i));
		// 形状を設定する
		//cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		// PSOを取得、コマンドリストにセット
		cmdList->SetPipelineState(commands_[i]->GetPSOState());

		// ルートシグネチャにバッファをセットする
		cmdList->SetGraphicsRootConstantBufferView(0, generalCBuffer_->Resource->GetGPUVirtualAddress());

		// 描画処理
		mesh_->Draw();
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
		commands_[i]->Init(device_, dxc_.get(), rtsManager_->GetRootSignature(i)); // 初期化
	}

	// バッファを生成
	CreateBuffers();

	// サンプルテクスチャの読み込み
	//defaultTexture_ = TextureManager::Load("white2x2.png");
}

Matrix4x4* const CommandManager::GetWorldMatrixAddress() const
{
	// 書き込み用のアドレスを返す
	return &generalCBuffer_->Data->WorldViewProj;
}

Matrix4x4* const CommandManager::GetViewMatrixAddress() const
{
	// 書き込み用のアドレスを返す
	return &generalCBuffer_->Data->WorldViewProj;
}

Matrix4x4* const CommandManager::GetViewProjectionAddress() const
{
	// 書き込み用のアドレスを返す
	return &generalCBuffer_->Data->WorldViewProj;
}

void CommandManager::SetDrawData()
{
	
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
	//// ルートシグネチャマネージャのインスタンス取得
	//rtsManager_ = RootSignatureManager::GetInstance();

	//// デバイスを渡す
	//rtsManager_->SetDevice(device_);
	//// ルートシグネチャマネージャの初期化
	//rtsManager_->Init();

	// ルートシグネチャ生成
	rtsManager_ = RootSignatureManager::GetInstance();
	rtsManager_->SetDevice(device_);
	rtsManager_->Init();
}

void CommandManager::Finalize()
{
	// メッシュの削除
	delete mesh_;
}

void CommandManager::CreateBuffers()
{
	// 生成確認
	HRESULT result = S_FALSE;

	// 汎用データバッファ生成
	generalCBuffer_ = std::make_unique<GeneralCBuffer>();												   // バッファの生成
	generalCBuffer_->Resource = std::move(CreateBuffer(sizeof(GeneralData)));							   // バッファの生成
	result = generalCBuffer_->Resource->Map(0, nullptr, reinterpret_cast<void**>(&generalCBuffer_->Data)); // 生成したバッファのマッピングを行う
	generalCBuffer_->View = generalCBuffer_->Resource->GetGPUVirtualAddress();							   // GPU上のアドレスの取得
	generalCBuffer_->Data->DrawMeshlets = true;

	// マッピングに失敗した場合
	if (FAILED(result)) {
		assert(false);
	}

	// メッシュ生成
	mesh_ = new Mesh(this);
	// コマンドマネージャーをセット
	mesh_->SetCommandManager(this);
	// メッシュのロード
	mesh_->LoadFile("./Engine/Resource/Samples/Sphere", "Sphere.obj");

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

ID3D12Resource* CommandManager::CreateTextureResource(const DirectX::TexMetadata& metaData)
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
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; // デフォルト設定

	// 3. Resourceを生成する
	ID3D12Resource* Resource = nullptr;
	result = device_->CreateCommittedResource(
		&heapProperties,					// Heapの設定
		D3D12_HEAP_FLAG_NONE,				// Heapの特殊な設定。特になし。
		&resourceDesc,						// Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST,		// データ転送される設定
		nullptr,							// Clear最適値。使わないでnullptr
		IID_PPV_ARGS(&Resource)				// 作成するResourceポインタへのポインタ
	);
	assert(SUCCEEDED(result));
	return Resource;
}

[[nodiscard]]
ID3D12Resource* CommandManager::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages)
{
	// 読み込みデータ格納用のサブリソース配列
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;

	// PrepareUploadを利用し、読み込んだデータで配列を生成
	DirectX::PrepareUpload(
		device_,
		mipImages.GetImages(),
		mipImages.GetImageCount(),
		mipImages.GetMetadata(),
		subresources);

	// サブリソース数を元にコピー元リソースに必要なサイズを計算
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture, 0, UINT(subresources.size()));
	// リソース生成
	ID3D12Resource* intermediateResource = CreateBuffer(intermediateSize);
	// intermediateResourceにサブリソースのデータを書き込み、テクスチャに転送するコマンドを積む
	UpdateSubresources(commandList_.Get(), texture, intermediateResource, 0, 0, UINT(subresources.size()), subresources.data());

	// 転送した後、利用できるようにリソースのステートを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	// リソースのステートを変更
	commandList_->ResourceBarrier(1, &barrier);

	// 生成したintermediateResourceを返す
	return intermediateResource;
}
