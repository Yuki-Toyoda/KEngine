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
	
	// 引数パスのシェーダーをコンパイルする
	meshShaderBlob_ = CompileShader(L"Engine/Resource/Shader/MeshletMS.hlsl", L"ms_6_5");

	// ルートシグネチャの生成
	CreateRootSignature();

	// バッファを生成
	CreateBuffers();
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
		//cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		// PSOを取得、コマンドリストにセット
		cmdList->SetPipelineState(commands_[i]->GetPSOState());

		// ルートシグネチャにバッファをセットする
		cmdList->SetGraphicsRootConstantBufferView(0, generalCBuffer_->Resource->GetGPUVirtualAddress());

		mesh_->Draw();

		// メッシュシェーダーを実行
		commandList_->DispatchMesh(mesh_->GetMeshletCount(), 1, 1);

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

	// サンプルテクスチャの読み込み
	//defaultTexture_ = TextureManager::Load("white2x2.png");

	// メッシュ生成
	mesh_ = new Mesh(this);
	// コマンドマネージャーをセット
	mesh_->SetCommandManager(this);
	// メッシュのロード
	mesh_->LoadFile("./Engine/Resource/Samples/Sphere", "Sphere.obj");

	// トランスフォーム初期化
	transform_.Init();
	// メッシュにトランスフォームを渡す
	mesh_->transform_ = &transform_;

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
	ImGui::Begin("MeshTransform");
	transform_.DisplayImGui();
	ImGui::End();
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

	// ルートシグネチャ生成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};           // 設定用インスタンス生成
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE; // フラッグはなし
	// ルートパラメータ生成
	D3D12_ROOT_PARAMETER rootParameters[5] = {};

	// 配列用のRangeDesc
	D3D12_DESCRIPTOR_RANGE descRange[1] = {};    // DescriptorRangeを作成
	descRange[0].BaseShaderRegister = 0;         // 0から始まる
	descRange[0].NumDescriptors = 1;             // 数は1つ
	descRange[0].RegisterSpace = 0;
	descRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootSignatureにrootParametersを登録
	descriptionRootSignature.pParameters = rootParameters;                    // ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);         // 配列の長さ

	// 汎用データ
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // PixelとVertexで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;                   // レジスタ番号0とバインド

	// 頂点データ
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV; // SRVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // PixelとVertexで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;                  // レジスタ番号0とバインド

	// メッシュレットデータ
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV; // SRVを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // PixelとVertexで使う
	rootParameters[2].Descriptor.ShaderRegister = 1;                  // レジスタ番号1とバインド

	// 固有頂点データ
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV; // SRVを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // PixelとVertexで使う
	rootParameters[3].Descriptor.ShaderRegister = 2;                  // レジスタ番号2とバインド

	// プリミティブ頂点データ
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV; // SRVを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // PixelとVertexで使う
	rootParameters[4].Descriptor.ShaderRegister = 3;                  // レジスタ番号3とバインド

	// シリアライズを行う
	ID3DBlob* signatureBlob = nullptr; // シリアライズ後のバイナリオブジェクト
	ID3DBlob* errorBlob = nullptr;     // エラーログを出すためのバイナリオブジェクト
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
	assert(SUCCEEDED(result));                                                                                                                  // 生成確認

	// 使わないリソースを解放
	signatureBlob->Release();

	////シェーダーバイナリからRootSignatureの部分をフェッチ
	//result = D3DGetBlobPart(meshShaderBlob_->GetBufferPointer(), meshShaderBlob_->GetBufferSize(), D3D_BLOB_ROOT_SIGNATURE, 0, &signatureBlob_);
	//// フェッチ出来たか確認
	//assert(SUCCEEDED(result));

	//// 取得したバイナリを元にルートシグネチャを生成
	//result = device_->CreateRootSignature(0, signatureBlob_->GetBufferPointer(), signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_)); // 生成
	//assert(SUCCEEDED(result));																													  // 生成確認
}

void CommandManager::CreateBuffers()
{
	// 生成確認
	HRESULT result = S_FALSE;

	// 汎用データバッファ生成
	generalCBuffer_ = std::make_unique<GeneralCBuffer>();										  // バッファの生成
	generalCBuffer_->Resource = CreateBuffer(sizeof(GeneralData));								  // バッファの生成
	result = generalCBuffer_->Resource->Map(0, nullptr, reinterpret_cast<void**>(&generalCBuffer_->Data)); // 生成したバッファのマッピングを行う
	generalCBuffer_->View = generalCBuffer_->Resource->GetGPUVirtualAddress();					  // GPU上のアドレスの取得
	generalCBuffer_->Data->DrawMeshlets = true;

	// マッピングに失敗した場合
	if (FAILED(result)) {
		assert(false);
	}

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

IDxcBlob* CommandManager::CompileShader(const std::wstring& filePath, const wchar_t* profile)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// シェーダーコンパイルの開始をログに出す
	Debug::Log(Debug::ConvertString(std::format(L"Begin CompileShader, path:{}, profile{}\n", filePath, profile)));

	// HLSLファイルを読み込む
	IDxcBlobEncoding* shaderSource = nullptr;
	result = dxc_->dxcUtils_->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	// 読み込み出来ているかを確認する
	assert(SUCCEEDED(result));

	// 読み込んだファイルの内容の設定を行う
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	// UTF-8形式の文字コードとして設定する
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	// コンパイルオプションの設定を行う
	LPCWSTR arguments[] = {
		filePath.c_str(), // コンパイル対象のhlslファイル名
		L"-E", L"main", // エントリーポイントの指定 基本的にmain以外にはしない
		L"-T", profile, // shaderProfileの設定
		L"-Zi", L"-Qembed_debug", // デバック用の情報埋め込み
		L"-Od", // 最適化を外す
		L"-Zpr", // メモリレイアウトは行優先
	};

	// 実際にシェーダーのコンパイルを行う
	IDxcResult* shaderResult = nullptr;
	result = dxc_->dxcCompiler_->Compile(
		&shaderSourceBuffer,             // 読み込んだファイル
		arguments,                       // コンパイルオプション
		_countof(arguments),             // コンパイルオプションの数
		dxc_->dxcIncludeHandler_.Get(),  // include が含まれた諸々
		IID_PPV_ARGS(&shaderResult)      // コンパイル結果
	);

	// ここで吐くエラーはコンパイルエラーではなく、dxcが起動できないなどの致命的なもの
	assert(SUCCEEDED(result));

	// 警告やエラーが出た場合にはログに出して停止させる
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		// ログに警告、エラー情報を出力する
		Debug::Log(shaderError->GetStringPointer());
		// 警告やエラーが出ている場合停止させる
		assert(false);
	}

	// コンパイル結果から実行用のバイナリ部分を取得する
	IDxcBlob* shaderBlob = nullptr;
	result = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	// 取得出来たか確認する
	assert(SUCCEEDED(result));

	// 成功したことをログに出力
	Debug::Log(Debug::ConvertString(std::format(L"Compile Succeeded, path:{}, profile{}\n", filePath, profile)));

	// 使わないリソースの解放を行う
	shaderSource->Release();
	shaderResult->Release();

	// 実行用のバイナリを返す
	return shaderBlob;
}
