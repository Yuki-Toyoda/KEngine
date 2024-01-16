#include "CommandManager.h"
#include "../DescriptorHeaps/RTV.h"
#include "../DescriptorHeaps/SRV.h"
#include "../DescriptorHeaps/DSV.h"
#include "../../Resource/Texture/TextureManager.h"
#include "../WinApp.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

void CommandManager::Init(ID3D12Device* device)
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

		// インデックスバッファが１つも入っていなければ描画しない
		if (commands_[i]->indexBuffer_->usedCount > 0) {
			// バッファ内の全3角形を描画
			cmdList->DrawInstanced(3, commands_[i]->indexBuffer_->usedCount / 3, 0, 0);
		}

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

	// 全ての描画コマンドのインデックス情報をクリア
	for (int i = 0; i < commands_.size(); i++) {
		commands_[i]->indexBuffer_->usedCount = 0;
	}

	// 全てのバッファのヒープ使用数をリセットする
	vertexBuffer_->usedCount = 0;		  // 頂点バッファ
	worldTransformBuffer_->usedCount = 0; // ワールドトランスフォームバッファ
	materialBuffer_->usedCount = 0;		  // マテリアルバッファ
}

void CommandManager::SetHeaps(RTV* rtv, SRV* srv, DSV* dsv, std::wstring vs, std::wstring ps)
{
	// ヒープのポインタをセット
	rtv_ = rtv; // レンダーターゲットビュー
	srv_ = srv; // シェーダーリソースビュー
	dsv_ = dsv; // 深度ステンシルビュー

	// 描画コマンドクラスの実体を宣言
	commands_.push_back(new MainCommand()); // メイン描画コマンドを追加
	commands_.push_back(new ParticleCommand()); // パーティクル描画コマンドを追加
	commands_.push_back(new SpriteCommand()); // パーティクル描画コマンドを追加
	//commands_.push_back(new MainCommand()); // スプライト描画コマンドを追加
	// 全ての描画コマンドの初期化
	for (int i = 0; i < commands_.size(); i++) {
		commands_[i]->SetDescriptorHeap(rtv_, srv_, dsv_); // ヒープをセット
		// バッファ配列の生成
		ID3D12Resource* buffer;
		// バッファをブレンドモードの数分生成
		buffer = CreateBuffer(sizeof(IndexInfoStruct) * commands_[i]->kMaxIndex);
		// リソースの生成を行う
		if (i == 1) {
			// パーティクルのみAddで生成
			commands_[i]->Init(device_, dxc_.get(), rootSignature_.Get(), buffer, vs, ps, BasePrimitive::kBlendAdd); // 初期化
		}
		else {
			commands_[i]->Init(device_, dxc_.get(), rootSignature_.Get(), buffer, vs, ps); // 初期化
		}
	}

	// ストラクチャーバッファを生成
	CreateStructuredBuffer();

	// サンプルテクスチャの読み込み
	defaultTexture_ = TextureManager::Load("white2x2.png");
}

Matrix4x4* const CommandManager::GetViewProjection() const {
	viewProjectionBuffer_->mat[1] = Math::MakeIdentity4x4() * Math::MakeOrthGraphicMatrix(0.0f, 0.0f, (float)WinApp::kWindowWidth, (float)WinApp::kwindowHeight, 0.0f, 100.0f);
	return viewProjectionBuffer_->mat;
}

void CommandManager::SetDrawData(BasePrimitive* primitive)
{
	// いろいろ最大数を超えていないかチェック
	assert(commands_[(int)primitive->primitiveType_]->indexBuffer_->usedCount < commands_[(int)primitive->primitiveType_]->kMaxIndex); // インデックス情報
	assert(vertexBuffer_->usedCount < kMaxVertex);							 // 頂点数
	assert(worldTransformBuffer_->usedCount < kMaxWorldTransform);			 // ワールドトランスフォーム
	assert(materialBuffer_->usedCount < kMaxMaterial);						 // マテリアル数

	// 頂点バッファの最初のインデックス番号の取得
	uint32_t startIndexNum = vertexBuffer_->usedCount;

	// 頂点データを登録
	for (int i = 0; i < primitive->GetVertexCount(); i++) {
		vertexBuffer_->vertex[vertexBuffer_->usedCount++] = primitive->vertices_[i]; // 頂点データをバッファに登録
		if (primitive->commonColor != nullptr)	// 共通の色があるときはcommonColorを適応
			vertexBuffer_->vertex[vertexBuffer_->usedCount - 1].color = *primitive->commonColor; // 共通色に変更
	}

	uint32_t useCamera = (uint32_t)primitive->isUI_;

	// ワールドトランスフォームをデータに登録
	uint32_t worldMatrix = worldTransformBuffer_->usedCount;											   // バッファの末尾を取得
	worldTransformBuffer_->mat[worldTransformBuffer_->usedCount++] = primitive->transform_->GetMatWorld(); // データを代入
	// マテリアルをデータに登録
	uint32_t material = materialBuffer_->usedCount;									// バッファの末尾を取得
	materialBuffer_->material[materialBuffer_->usedCount++] = primitive->material_; // データを代入
	// テクスチャのインデックスを貰う
	uint32_t texture = defaultTexture_->GetIndex();
	if (primitive->texture_ != nullptr) { // テクスチャがある場合
		texture = primitive->texture_->GetIndex();
	}

	// Indexの分だけIndexInfoを求める
	for (int i = 0; i < primitive->GetIndexCount(); i++) {
		commands_[(int)primitive->primitiveType_]->indexBuffer_->indexData[commands_[(int)primitive->primitiveType_]->indexBuffer_->usedCount++] = IndexInfoStruct{
			startIndexNum + primitive->indexes_[i],
			useCamera,
			worldMatrix,
			material,
			texture
		};
	}
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
	ImGui::Begin("Light");
	ImGui::ColorEdit4("Color", &lightBuffer_->light->color.x);
	ImGui::SliderFloat3("Direction", &lightBuffer_->light->direction.x, -1.0f, 1.0f);
	ImGui::DragFloat("Intensity", &lightBuffer_->light->intensity, 0.01f);
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
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};			 // 設定用インスタンス生成
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE; // フラッグはなし
	// ルートパラメータ生成
	D3D12_ROOT_PARAMETER rootParameters[7] = {};
	// サンプラー
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	// 配列用のRangeDesc
	D3D12_DESCRIPTOR_RANGE descRange[1] = {};	// DescriptorRangeを作成
	descRange[0].BaseShaderRegister = 0; // 0から始まる
	descRange[0].NumDescriptors = 1; // 数は1つ
	descRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; // Offsetを自動計算

	// RootSignatureにrootParametersを登録
	descriptionRootSignature.pParameters = rootParameters;					// ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);		// 配列の長さ

	// RootSignatureにサンプラーを登録
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	// ストラクチャーバッファーのインデックス
	D3D12_DESCRIPTOR_RANGE indexDesc[1] = { descRange[0] };						  // DescriptorRangeを作成
	indexDesc[0].BaseShaderRegister = 0;										  // レジスタ番号は0
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			  // 全てのシェーダーで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = indexDesc;			  // Tabelの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(indexDesc);  // Tableで利用する数

	// 平行光源
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;  // CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; // PixelとVertexで使う
	rootParameters[1].Descriptor.ShaderRegister = 0;				  // レジスタ番号0とバインド

	// 頂点データ
	D3D12_DESCRIPTOR_RANGE vertexDesc[1] = { descRange[0] };					  // DescriptorRangeを作成
	vertexDesc[0].BaseShaderRegister = 1;										  // レジスタ番号は1
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;		  // 頂点シェーダーで使う
	rootParameters[2].DescriptorTable.pDescriptorRanges = vertexDesc;			  // Tabelの中身の配列を指定
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(vertexDesc); // Tableで利用する数

	// カメラのViewProjection
	D3D12_DESCRIPTOR_RANGE vpDesc[1] = { descRange[0] };						  // DescriptorRangeを作成
	vpDesc[0].BaseShaderRegister = 2;											  // レジスタ番号は2
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;		  // 頂点シェーダーで使う
	rootParameters[3].DescriptorTable.pDescriptorRanges = vpDesc;				  // Tabelの中身の配列を指定
	rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(vpDesc);	  // Tableで利用する数

	// WorldTransform
	D3D12_DESCRIPTOR_RANGE wtDesc[1] = { descRange[0] };						  // DescriptorRangeを作成
	wtDesc[0].BaseShaderRegister = 3;											  // レジスタ番号は3
	rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // DescriptorTableを使う
	rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;		  // 頂点シェーダーで使う
	rootParameters[4].DescriptorTable.pDescriptorRanges = wtDesc;				  // Tabelの中身の配列を指定
	rootParameters[4].DescriptorTable.NumDescriptorRanges = _countof(wtDesc);	  // Tableで利用する数

	// マテリアル
	D3D12_DESCRIPTOR_RANGE materialDesc[1] = { descRange[0] };					    // DescriptorRangeを作成
	materialDesc[0].BaseShaderRegister = 1;										    // レジスタ番号は1
	rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   // DescriptorTableを使う
	rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			    // ピクセルシェーダーで使う
	rootParameters[5].DescriptorTable.pDescriptorRanges = materialDesc;			    // Tabelの中身の配列を指定
	rootParameters[5].DescriptorTable.NumDescriptorRanges = _countof(materialDesc); // Tableで利用する数

	// Samplerの設定
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR; // バイオリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP; // 0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER; // 比較しない
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX; // ありったけのMipmapを使う
	staticSamplers[0].ShaderRegister = 0; // レジスタ番号は0
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う

	// テクスチャ
	D3D12_DESCRIPTOR_RANGE textureDesc[1] = { descRange[0] };					   // DescriptorRangeを作成
	textureDesc[0].BaseShaderRegister = 2;										   // レジスタ番号は2
	textureDesc[0].NumDescriptors = kMaxTexture;								   // 最大数を定義
	rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;  // DescriptorTabelを使う
	rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;		       // ピクセルシェーダーで使う
	rootParameters[6].DescriptorTable.pDescriptorRanges = textureDesc;			   // Tabelの中身の配列を指定
	rootParameters[6].DescriptorTable.NumDescriptorRanges = _countof(textureDesc); // Tableで利用する数

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
	// 共通のSRV用Desc
	D3D12_SHADER_RESOURCE_VIEW_DESC commonDesc{};								   // 汎用設定用
	commonDesc.Format = DXGI_FORMAT_UNKNOWN;									   // フォーマット形式は不明
	commonDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING; // シェーダーからテクスチャにアクセスする際の値指定 
	commonDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;						   // SRVのバッファであることを指定
	commonDesc.Buffer.FirstElement = 0;											   // 最初の番号を指定
	commonDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;						   // フラッグ設定

	// 平行光源
	lightBuffer_ = std::make_unique<LightBuffer>();
	lightBuffer_->resource = CreateBuffer(sizeof(DirectionalLight));
	lightBuffer_->resource->Map(0, nullptr, reinterpret_cast<void**>(&lightBuffer_->light));
	lightBuffer_->view = lightBuffer_->resource->GetGPUVirtualAddress();
	// 平行光源だけ今は生成を変える
	lightBuffer_->light->color = { 1.0f,1.0f,1.0f,1.0f };
	lightBuffer_->light->direction = { 0.0f,-1.0f,0.0f };
	lightBuffer_->light->intensity = 1.0f;
	lightBuffer_->rotate = { 0.0f,0.0f,0.0f };

	// 頂点データ
	vertexBuffer_ = std::make_unique<VertexBuffer>();
	vertexBuffer_->resource = CreateBuffer(sizeof(Vertex) * kMaxVertex);
	vertexBuffer_->resource->Map(0, nullptr, reinterpret_cast<void**>(&vertexBuffer_->vertex));
	D3D12_SHADER_RESOURCE_VIEW_DESC vertexDesc = { commonDesc };
	vertexDesc.Buffer.NumElements = kMaxVertex;
	vertexDesc.Buffer.StructureByteStride = sizeof(Vertex);
	vertexBuffer_->view = srv_->GetGPUHandle(srv_->GetUsedCount());
	device_->CreateShaderResourceView(vertexBuffer_->resource.Get(), &vertexDesc, srv_->GetCPUHandle(srv_->GetUsedCount()));
	srv_->AddUsedCount();	// SRV使用数を+1
	// カメラのビュープロジェクション用
	viewProjectionBuffer_ = std::make_unique<MatrixBuffer>();
	viewProjectionBuffer_->resource = CreateBuffer(sizeof(Matrix4x4) * kMaxVP);
	viewProjectionBuffer_->resource->Map(0, nullptr, reinterpret_cast<void**>(&viewProjectionBuffer_->mat));
	D3D12_SHADER_RESOURCE_VIEW_DESC vpDesc = { commonDesc };
	vpDesc.Buffer.NumElements = kMaxVP;
	vpDesc.Buffer.StructureByteStride = sizeof(Matrix4x4);
	viewProjectionBuffer_->view = srv_->GetGPUHandle(srv_->GetUsedCount());
	device_->CreateShaderResourceView(viewProjectionBuffer_->resource.Get(), &vpDesc, srv_->GetCPUHandle(srv_->GetUsedCount()));
	srv_->AddUsedCount();	// SRV使用数を+1
	// WorldTransformデータ
	worldTransformBuffer_ = std::make_unique<MatrixBuffer>();
	worldTransformBuffer_->resource = CreateBuffer(sizeof(Matrix4x4) * kMaxWorldTransform);
	worldTransformBuffer_->resource->Map(0, nullptr, reinterpret_cast<void**>(&worldTransformBuffer_->mat));
	D3D12_SHADER_RESOURCE_VIEW_DESC wtDesc = { commonDesc };
	wtDesc.Buffer.NumElements = kMaxWorldTransform;
	wtDesc.Buffer.StructureByteStride = sizeof(Matrix4x4);
	worldTransformBuffer_->view = srv_->GetGPUHandle(srv_->GetUsedCount());
	device_->CreateShaderResourceView(worldTransformBuffer_->resource.Get(), &wtDesc, srv_->GetCPUHandle(srv_->GetUsedCount()));
	srv_->AddUsedCount();	// SRV使用数を+1

	// マテリアルデータ
	materialBuffer_ = std::make_unique<MaterialBuffer>();
	materialBuffer_->resource = CreateBuffer(sizeof(Material) * kMaxMaterial);
	materialBuffer_->resource->Map(0, nullptr, reinterpret_cast<void**>(&materialBuffer_->material));
	D3D12_SHADER_RESOURCE_VIEW_DESC materialDesc = { commonDesc };
	materialDesc.Buffer.NumElements = kMaxMaterial;
	materialDesc.Buffer.StructureByteStride = sizeof(Material);
	materialBuffer_->view = srv_->GetGPUHandle(srv_->GetUsedCount());
	device_->CreateShaderResourceView(materialBuffer_->resource.Get(), &materialDesc, srv_->GetCPUHandle(srv_->GetUsedCount()));
	srv_->AddUsedCount();	// SRV使用数を+1
	// テクスチャデータ
	textureBuffer_ = std::make_unique<TextureBuffer>();
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
