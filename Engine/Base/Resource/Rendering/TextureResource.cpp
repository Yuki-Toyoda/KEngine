#include "TextureResource.h"
#include "../../../Debug/Debug.h"
#include "../../DirectXCommon.h"

void TextureResource::Init(DirectXDevice* device, HeapManager* heaps, std::string filePath)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// テクスチャを読み込む
	Load(filePath);

	// メタデータの読み込み
	const DirectX::TexMetadata metadata(mipImages_.GetMetadata());
	
	// 解像度を取得
	width_  = static_cast<int>(metadata.width);  // 横幅
	height_ = static_cast<int>(metadata.height); // 縦幅

	// Reourceの設定
	desc_.Width			   = UINT16(metadata.width);						   // 解像度横幅
	desc_.Height		   = UINT16(metadata.height);						   // 解像度縦幅
	desc_.MipLevels		   = UINT16(metadata.mipLevels);					   // ミップレベル
	desc_.DepthOrArraySize = UINT16(metadata.arraySize);					   // 配列サイズ
	desc_.Format		   = metadata.format;								   // フォーマット設定
	desc_.SampleDesc.Count = 1;												   // カウント設定
	desc_.Dimension		   = D3D12_RESOURCE_DIMENSION(metadata.dimension);	   // ディメンション設定

	// 利用するヒープの設定
	properties_.Type = D3D12_HEAP_TYPE_DEFAULT;

	// リソースを生成する
	result = device->GetDevice()->CreateCommittedResource(
		&properties_,					   // ヒープ設定
		D3D12_HEAP_FLAG_NONE,			   // フラッグ設定
		&desc_,							   // リソース設定	
		D3D12_RESOURCE_STATE_COPY_DEST,	   // 読み取り専用
		nullptr,						   // 使わないためnullptr
		IID_PPV_ARGS(&resource_)		   // 作成するResourceのポインタ
	);

	// 生成に失敗した場合停止させる
	assert(SUCCEEDED(result));

	// テクスチャデータをコマンドリストに渡す用指示する
	UploadTextureData(device);

	// 登録したSRV上のインデックスを取得する
	info_ = heaps->srv()->RegisterTexture(resource_.Get(), mipImages_);
}

void TextureResource::UploadTextureData(DirectXDevice* device)
{
	// サブリソースデータ格納用配列
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	// 読み込んだテクスチャ情報を元に配列を作成
	DirectX::PrepareUpload(device->GetDevice(), mipImages_.GetImages(), mipImages_.GetImageCount(), mipImages_.GetMetadata(), subresources);
	
	// 中間リソースのサイズを取得する
	uint64_t intermediateSize = GetRequiredIntermediateSize(resource_.Get(), 0, UINT(subresources.size()));
	// 中間リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;	// UploadHeapを使う
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	// バッファリソース。テクスチャの場合はまた別の設定をする
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = intermediateSize; // リソースのサイズ
	// バッファの場合はこれらは1にする決まり
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 中間リソースを生成
	device->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&intermediateResource_));

	// コマンドリストの取得
	Command* cmd = DirectXCommon::GetInstance()->GetRendererManager()->GetCommand();
	// データ転送のためにコマンドリストに命令する
	UpdateSubresources(cmd->List(), resource_.Get(), intermediateResource_, 0, 0, UINT(subresources.size()), subresources.data());

	// Texture転送後、利用できるようにリソースのステートを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = resource_.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	// リソースバリアを生成
	cmd->List()->ResourceBarrier(1, &barrier);

	// コマンドリストの実行後、中間リソースを削除するようにリストに追加する
	cmd->AddReleaseFunction([this]() {
		if (intermediateResource_ != nullptr) {
			intermediateResource_->Release();
		}
	});
}

void TextureResource::Load(const std::string& filePath)
{
	// テクスチャ読み込みデータ一時格納用
	DirectX::ScratchImage image{};
	// 後に呼び出す関数用にwstring形式に変換する
	std::wstring filePathW = Debug::ConvertString(filePath);
	
	// 読み込み結果確認用
	HRESULT result = S_FALSE;
	
	// ファイル拡張子によって読み込み処理を変更する
	if (filePath.ends_with(".dds")) { // .ddsの場合
		// dds形式の読み込みを行う
		result = DirectX::LoadFromDDSFile(filePathW.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, image);
	}
	else { // それ以外の場合
		// 通常のテクスチャ読み込む
		result = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	}

	// 読み込みに失敗した場合停止させる
	assert(SUCCEEDED(result));

	// 圧縮形式であるかどうかで処理を変更する
	if (DirectX::IsCompressed(image.GetMetadata().format)) { // 圧縮フォーマットである場合
		// 圧縮形式である場合そのまま代入する
		mipImages_ = std::move(image);
		
	}
	else { // それ以外である場合
		// ミップマップの作成
		result = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages_);
	}
	
	// 生成に失敗した場合停止させる
	assert(SUCCEEDED(result));
}
