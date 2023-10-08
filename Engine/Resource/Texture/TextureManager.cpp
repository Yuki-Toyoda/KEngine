#include <cassert>
#include "TextureManager.h"
#include "../../externals/DirectXTex/DirectXTex.h"
#include "../../Debug/Debug.h"

// 省略用
using namespace Microsoft::WRL;
using namespace DirectX;

D3D12_CPU_DESCRIPTOR_HANDLE TextureManager::GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index) {
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

uint32_t TextureManager::Load(const std::string& fileName)
{
	// テクスチャロード -> テクスチャハンドルを返す
	return TextureManager::GetInstance()->LoadInternal(fileName);
}

TextureManager* TextureManager::GetInstance()
{
	static TextureManager textureManager;
	return &textureManager;
}

void TextureManager::Intialize(ID3D12Device* device, std::string directoryPath)
{
	// NULLチェック
	assert(device);
	
	// 引数をメンバ変数に代入
	device_ = device;
	directoryPath_ = directoryPath;

	// ディスクリプタハンドルのサイズを取得する
	sDescriptorHandleIncrementSize_ =
		device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// 全テクスチャリセット
	ResetAll();
}

void TextureManager::ResetAll()
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// ディスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; // タイプ設定
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE; // フラッグ
	descHeapDesc.NumDescriptors = kDescriptorSize;
	// ディスクリプタヒープの生成
	result = device_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descriptorHeap_));
	// 生成出来ているか確認
	assert(SUCCEEDED(result));

	// 次のディスクリプタヒープの番号を指定
	indexNextDescriptorHeap_ = 0;

	// 全てのテクスチャをリセットする
	for (size_t i = 0; i < kDescriptorSize; i++) {
		textures_[i].resource.Reset(); // リソース
		textures_[i].cpuDescriptorHandleSRV.ptr = 0; // ディスクリプタハンドル（CPU）
		textures_[i].gpuDescriptorHnadleSRV.ptr = 0; // ディスクリプタハンドル（GPU）
		textures_[i].name.clear(); // ファイル名
	}
}

const D3D12_RESOURCE_DESC TextureManager::GetResourceDesc(uint32_t textureHandle)
{
	// 指定したテクスチャがテクスチャサイズを超過していないか確認する
	assert(textureHandle < textures_.size());

	// 指定したテクスチャの要素取得
	Texture& texture = textures_.at(textureHandle);
	// テクスチャ情報を返す
	return texture.resource->GetDesc();
}

const std::string TextureManager::GetTextureName(uint32_t textureHandle) {
	// 指定したテクスチャがテクスチャサイズを超過していないか確認する
	assert(textureHandle < textures_.size());

	// 指定したテクスチャの要素取得
	Texture& texture = textures_.at(textureHandle);
	// テクスチャ情報を返す
	return texture.name;
}

const D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetTextureGPUHandle(uint32_t textureHandle) {
	// 指定したテクスチャがテクスチャサイズを超過していないか確認する
	assert(textureHandle < textures_.size());

	// 指定したテクスチャの要素取得
	Texture& texture = textures_.at(textureHandle);
	// テクスチャ情報を返す
	return texture.gpuDescriptorHnadleSRV;
}

void TextureManager::SetGraphicsDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT rotParamIndex, uint32_t textureHandle)
{
	// テクスチャハンドルがテクスチャサイズを超過していないか確認する
	assert(textureHandle < textures_.size());

	// ディスクリプタヒープを取得
	ID3D12DescriptorHeap* ppHeaps[] = { descriptorHeap_.Get() };
	// 取得したディスクリプタヒープをコマンドリストにセット
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// ディスクリプタテーブルをコマンドリストにセット
	commandList->SetGraphicsRootDescriptorTable(
		rotParamIndex, textures_[textureHandle].gpuDescriptorHnadleSRV
	);
}

uint32_t TextureManager::LoadInternal(const std::string& fileName)
{
	// 次のディスクリプタヒープの番号が最大サイズを超過していないか確認
	assert(indexNextDescriptorHeap_ < kDescriptorSize);
	// 次のディスクリプタヒープ番号の取得
	uint32_t handle = indexNextDescriptorHeap_;

	// テクスチャのコンテナ内に目的のテクスチャがあるかを確認する
	// コンテナ内の最初から最後までに引数のテクスチャがあるかを確認する
	auto it = std::find_if(textures_.begin(), textures_.end(), [&](const auto& texture) {
		// 発見できた場合ファイル名をそのまま返す
		return texture.name == fileName;
	});
	// コンテナ内にテクスチャが存在すればそのハンドルを返す
	if (it != textures_.end()) {
		// テクスチャハンドルを返す
		handle = static_cast<uint32_t>(std::distance(textures_.begin(), it));
		return handle;
	}

	// テクスチャをコンテナ内から取得
	Texture& texture = textures_.at(handle);
	texture.name = fileName;

	// . と / がファイルパス内に含まれているか確認
	bool currentRelative = false;
	if (2 < fileName.size()) {
		currentRelative = (fileName[0] == '.' && (fileName[1] == '.'));
	}
	// 含まれていた場合は .　と / を含めたフルパスを入れる
	std::string fullPath = currentRelative ? fileName : directoryPath_ + fileName;

	// ファイルパスをユニコード文字列に変換する
	wchar_t wfilePath[256];
	MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, wfilePath, _countof(wfilePath));

	// 結果確認用
	HRESULT result = S_FALSE;

	// テクスチャ
	ScratchImage scratchImg{};
	// メタデータ
	TexMetadata metadata{};

	// テクスチャをCPUで読み込む
	result = LoadFromWICFile(wfilePath, WIC_FLAGS_NONE, &metadata, scratchImg);
	// 読み込んだテクスチャを確認する
	assert(SUCCEEDED(result));

	// ミップマップ
	ScratchImage mipChain{};
	// ミップマップ生成
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);

	// ミップマップ生成に成功したら
	if (SUCCEEDED(result)) {
		// 生成したミップマップの所有権をテクスチャにを渡す
		scratchImg = std::move(mipChain);
		// テクスチャのメタデータを取得する
		metadata = scratchImg.GetMetadata();
	}
	// メタデータのフォーマットをSRGBに変更する
	metadata.format = MakeSRGB(metadata.format);

	// メタデータを元にリソースの設定を行う
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Width = UINT(metadata.width); // テクスチャ幅
	textureResourceDesc.Height = UINT(metadata.height); // テクスチャ高さ
	textureResourceDesc.MipLevels = UINT16(metadata.mipLevels); // mipmapの数
	textureResourceDesc.DepthOrArraySize = UINT16(metadata.arraySize); // 奥行き or 配列テクスチャの配列数
	textureResourceDesc.Format = metadata.format; // テクスチャのフォーマット
	textureResourceDesc.SampleDesc.Count = 1; // サンプリングカウント 1固定
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension); // テクスチャの次元数　普段使っているのは二次元

	// 利用するヒープの設定を行う
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM; // 細かい設定を行う
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // WriteBackポリシーでCPUアクセスが可能
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0; // プロセッサの近くに配置する

	// テクスチャリソースを生成する
	result = device_->CreateCommittedResource(
		&heapProperties,					// ヒープ設定
		D3D12_HEAP_FLAG_NONE,				// Heapの特殊な設定
		&textureResourceDesc,				// リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,  // アップロードヒープに必要な開始状態
		nullptr,							// クリアの最適値、今回は使わないのでnullptr
		IID_PPV_ARGS(&texture.resource)		// 作成するリソースポインタでへのポインタ
	);
	// テクスチャリソースが生成出来たかどうか確認する
	assert(SUCCEEDED(result));

	// 全体のミップマップについて設定
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		// ミップマップレベルを設定し、各画像を取得する
		const Image* img = scratchImg.GetImage(i, 0, 0);
		// テクスチャの転送を行う
		result = texture.resource->WriteToSubresource(
			UINT(i),
			nullptr,			  // 全領域へコピーする
			img->pixels,		  // 元データアドレス
			(UINT)img->rowPitch,  // 1ラインアドレス
			(UINT)img->slicePitch // 1枚サイズ
		);
		// 転送出来たか確認する
		assert(SUCCEEDED(result));
	}

	// SRVを作成するDescriptorHeapの場所を決める
	texture.cpuDescriptorHandleSRV = GetCPUDescriptorHandle(
		descriptorHeap_.Get(), handle, sDescriptorHandleIncrementSize_); // CPU
	texture.gpuDescriptorHnadleSRV = GetGPUDescriptorHandle(
		descriptorHeap_.Get(), handle, sDescriptorHandleIncrementSize_); // GPU

	// テクスチャのリソース設定を取得する
	D3D12_RESOURCE_DESC resourceDesc = texture.resource->GetDesc();
	// SRVの設定を行う
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = resourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D; // 2Dテクスチャの設定
	srvDesc.Texture2D.MipLevels = (UINT)metadata.mipLevels;

	// SRVの生成を行う
	device_->CreateShaderResourceView(
		texture.resource.Get(),
		&srvDesc,
		texture.cpuDescriptorHandleSRV
	);

	// 次のディスクリプタヒープ番号を指定
	indexNextDescriptorHeap_++;

	// ハンドルを返す
	return handle;
}