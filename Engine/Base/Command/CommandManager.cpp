#include "CommandManager.h"
#include "../DescriptorHeaps/RTV.h"
#include "../DescriptorHeaps/SRV.h"
#include "../DescriptorHeaps/DSV.h"
#include "../../Resource/Texture/TextureManager.h"
#include "../WinApp.h"

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
	
	// メモリにバッファの内容をアップロード
	memcpy(cBufferBegin_ + sizeof(GeneralCBuffer) * rtv_->GetDescriptorSize(), &generalCBufferData_, sizeof(generalCBufferData_));

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

		SetDrawData();

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

	// ストラクチャーバッファを生成
	CreateStructuredBuffer();

	// ルートシグネチャの生成
	CreateRootSignature();

	// サンプルテクスチャの読み込み
	defaultTexture_ = TextureManager::Load("white2x2.png");
}

Matrix4x4* const CommandManager::GetWorldMatrixAddress() const
{
	// 書き込み用のアドレスを返す
	return &generalCBufferData_->World;
}

Matrix4x4* const CommandManager::GetViewMatrixAddress() const
{
	// 書き込み用のアドレスを返す
	return &generalCBufferData_->WorldView;
}

Matrix4x4* const CommandManager::GetViewProjectionAddress() const
{
	// 書き込み用のアドレスを返す
	return &generalCBufferData_->WorldViewProj;
}

void CommandManager::SetDrawData()
{
	
}

int CommandManager::createTextureResource(const DirectX::ScratchImage& image)
{
	// バッファにテクスチャのメタデータを追加
	textureBuffer_->resource.push_back(CreateTextureBuffer(image.GetMetadata()));
	// テクスチャをアップロード
	UploadTextureData(image);
	// テクスチャバッファの番号を返す
	return textureBuffer_->usedCount++;
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

	// ルートシグネチャの設定用構造体を生成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};			 // 設定用インスタンス生成
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE; // フラッグはなし
	
	// ルートパラメータ生成
	D3D12_ROOT_PARAMETER rootParameters[6] = {};
	
	// サンプラー
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	
	// StructuredBuffer用の範囲設定
	D3D12_DESCRIPTOR_RANGE descRange[1] = {};												// DescriptorRangeを作成
	descRange[0].BaseShaderRegister = 0;													// 0から始まる
	descRange[0].NumDescriptors = 1;														// 数は1つ
	descRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;								// SRVを使う
	descRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootSignatureにrootParametersを登録
	descriptionRootSignature.pParameters = rootParameters;					// ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);		// 配列の長さ

	// RootSignatureにサンプラーを登録
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	// 汎用定数バッファ用
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // ConstantBufferを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // PixelとVertexで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;				  // レジスタ番号0とバインド

	// インデックスバッファ用
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;  // CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			  // PixelとVertexで使う
	rootParameters[1].Descriptor.ShaderRegister = 1;							  // レジスタ番号0とバインド

	// 頂点バッファ用
	D3D12_DESCRIPTOR_RANGE vertexDesc[1] = { descRange[0] };					  // DescriptorRangeを作成
	vertexDesc[0].BaseShaderRegister = 0;										  // レジスタ番号は0
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;		      // PixelとVertexで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = vertexDesc;			  // Tabelの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(vertexDesc); // Tableで利用する数

	// メッシュレットバッファ用
	D3D12_DESCRIPTOR_RANGE meshletDesc[1] = { descRange[0] };					   // DescriptorRangeを作成
	meshletDesc[0].BaseShaderRegister = 1;										   // レジスタ番号は0
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;  // DescriptorTableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;		       // PixelとVertexで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = meshletDesc;			   // Tabelの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(meshletDesc); // Tableで利用する数

	// 固有頂点バッファ用
	D3D12_DESCRIPTOR_RANGE uniqueVertexDesc[1] = { descRange[0] };						// DescriptorRangeを作成
	uniqueVertexDesc[0].BaseShaderRegister = 2;											// レジスタ番号は0
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		// DescriptorTableを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					// PixelとVertexで使う
	rootParameters[4].DescriptorTable.pDescriptorRanges = uniqueVertexDesc;				// Tabelの中身の配列を指定
	rootParameters[4].DescriptorTable.NumDescriptorRanges = _countof(uniqueVertexDesc); // Tableで利用する数
	
	// プリミティブインデックスバッファ用
	D3D12_DESCRIPTOR_RANGE primitiveIndexDesc[1] = { descRange[0] };					  // DescriptorRangeを作成
	primitiveIndexDesc[0].BaseShaderRegister = 3;										  // レジスタ番号は0
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		  // DescriptorTableを使う
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					  // PixelとVertexで使う
	rootParameters[5].DescriptorTable.pDescriptorRanges = primitiveIndexDesc;			  // Tabelの中身の配列を指定
	rootParameters[5].DescriptorTable.NumDescriptorRanges = _countof(primitiveIndexDesc); // Tableで利用する数

	// Samplerの設定
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイオリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号は0
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う

	// シリアライズを行う
	ID3DBlob* signatureBlob = nullptr; // シリアライズ後のバイナリオブジェクト
	ID3DBlob* errorBlob = nullptr;	   // エラーログを出すためのバイナリオブジェクト
	// ルートシグネチャ用にシリアライズ
	result = D3D12SerializeRootSignature(&descriptionRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	// 生成に失敗した場合
	if (FAILED(result)) {
		// ログを出力
		Debug::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		// 停止
		assert(false);
	}
	// バイナリを元にルートシグネチャを生成
	result = device_->CreateRootSignature(0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_)); // 生成
	assert(SUCCEEDED(result));																													// 生成確認

	// 使わないリソースを解放
	signatureBlob->Release();
	//errorBlob->Release();
}

void CommandManager::CreateStructuredBuffer()
{
	// 汎用定数バッファ
	generalCBufferData_ = std::make_unique<GeneralCBuffer>();

	// バッファサイズを求める
	const UINT64 constantBufferSize = sizeof(GeneralCBuffer) * rtv_->GetDescriptorSize();

	// アップロード用のヒープを生成
	const CD3DX12_HEAP_PROPERTIES constantBufferHeapProps(D3D12_HEAP_TYPE_UPLOAD);
	// ヒープ設定を取得
	const CD3DX12_RESOURCE_DESC constantBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize);

	// リソース生成
	device_->CreateCommittedResource(
		&constantBufferHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&constantBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&generalCBuffer_));

	// バッファビューの生成
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	// バッファの場所を取得する
	cbvDesc.BufferLocation = generalCBuffer_->GetGPUVirtualAddress();
	// バッファのサイズを取得する
	cbvDesc.SizeInBytes = static_cast<UINT>(constantBufferSize);

	// 何も読み取らないように範囲指定
	CD3DX12_RANGE readRange(0, 0);
	// バッファのマッピングを行う
	generalCBuffer_->Map(0, &readRange, reinterpret_cast<void**>(&cBufferBegin_));

	// テクスチャデータ
	textureBuffer_ = std::make_unique<TextureBuffer>();

	// モデルのバイナリオブジェクトを読み込む
	model_.LoadFromFile(L"./Engine/Resource/Samples/Meshlets/Dragon_LOD0.bin");
	model_.UploadGpuResources(device_, commandList_.Get());
}

ID3D12Resource* CommandManager::CreateBuffer(size_t size)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// 返すリソース
	ID3D12Resource* resource;

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
	result = device_->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(result));

	// 生成したバッファリソースを返す
	return resource;
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
	ID3D12Resource* resource = nullptr;
	result = device_->CreateCommittedResource(
		&heapProperties,					// Heapの設定
		D3D12_HEAP_FLAG_NONE,				// Heapの特殊な設定。特になし。
		&resourceDesc,						// Resourceの設定
		D3D12_RESOURCE_STATE_GENERIC_READ,	// 初回のResourceState。Textureは基本読むだけ
		nullptr,							// Clear最適地。使わないでnullptr
		IID_PPV_ARGS(&resource)				// 作成するResourceポインタへのポインタ
	);
	assert(SUCCEEDED(result));
	return resource;
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
		result = textureBuffer_->resource[textureBuffer_->usedCount]->WriteToSubresource(
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
	D3D12_CPU_DESCRIPTOR_HANDLE textureSRVHandleCPU = srv_->GetCPUHandle(textureBuffer_->usedCount + srv_->GetUsedCount());
	// 初めてのテクスチャ生成ならviewを保存
	if (textureBuffer_->usedCount == 0) {
		textureBuffer_->view = srv_->GetGPUHandle(textureBuffer_->usedCount + srv_->GetUsedCount());
	}
	// SRVの生成
	device_->CreateShaderResourceView(textureBuffer_->resource[textureBuffer_->usedCount].Get(), &srvDesc, textureSRVHandleCPU);
}

bool CommandManager::ReadDataFile(LPCWSTR filename, byte** data, UINT* size)
{
	using namespace Microsoft::WRL;

#if WINVER >= _WIN32_WINNT_WIN8
	CREATEFILE2_EXTENDED_PARAMETERS extendedParams = {};
	extendedParams.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
	extendedParams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
	extendedParams.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
	extendedParams.dwSecurityQosFlags = SECURITY_ANONYMOUS;
	extendedParams.lpSecurityAttributes = nullptr;
	extendedParams.hTemplateFile = nullptr;

	Wrappers::FileHandle file(CreateFile2(filename, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, &extendedParams));
#else
	Wrappers::FileHandle file(CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN | SECURITY_SQOS_PRESENT | SECURITY_ANONYMOUS, nullptr));
#endif
	if (file.Get() == INVALID_HANDLE_VALUE)
	{
		throw std::exception();
	}

	FILE_STANDARD_INFO fileInfo = {};
	if (!GetFileInformationByHandleEx(file.Get(), FileStandardInfo, &fileInfo, sizeof(fileInfo)))
	{
		throw std::exception();
	}

	if (fileInfo.EndOfFile.HighPart != 0)
	{
		throw std::exception();
	}

	*data = reinterpret_cast<byte*>(malloc(fileInfo.EndOfFile.LowPart));
	*size = fileInfo.EndOfFile.LowPart;

	if (!ReadFile(file.Get(), *data, fileInfo.EndOfFile.LowPart, nullptr, nullptr))
	{
		throw std::exception();
	}

	return true;
}
